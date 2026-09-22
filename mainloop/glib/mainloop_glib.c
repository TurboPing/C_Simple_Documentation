/**
 * @file
 * Example for main loop using glib 2
 *
 * @author Rico Pajarola
 *
 * This example uses glib 2 for handling events.
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <signal.h>
#include <fcntl.h>

#include "glib.h"

/** tcp port to listen for echo clients */
#define PORT_ECHO 5005

/** tcp port to listen for chargen clients */
#define PORT_CHARGEN 5006

/** heartbeat interval (in seconds) */
#define HEARTBEAT_INTERVAL 2

/** slow heartbeat interval (in seconds) */
#define SLOWHEARTBEAT_INTERVAL 15

/** buffer size for echo client */
#define BUFSIZE 16

#define MSG_HEARTBEAT 0
#define MSG_SLOWHEARTBEAT 1
#define MSG_MAINLOOP 2
#define MSG_ACCEPT 3
#define MSG_TOOMANY 4
#define MSG_CLOSE 5
#define MSG_READ 6
#define MSG_WRITE 7
#define MSG_FULL 8
#define MSG_EMPTY 9

/** characters to return in chargen service */
static char     chargen_buf[] = "0123456789abcdefghijklmnopqrstuv";

typedef GSource *(*getclientsourcefunc) ();

/**
 * listen source state
 */
typedef struct {
    getclientsourcefunc getclientsource;
} listen_source_t;

/**
 * echo source state
 */
typedef struct {
    int             r;	/**< read position */
    int		    w;  /**< write position */
    int		    n;  /**< number of bytes */
    char            buf[BUFSIZE];	/**< buffer */
} echo_source_t;

/**
 * chargen source state
 */
typedef struct {
    int             i;	/**< index into chargen_buf */
} chargen_source_t;

typedef struct {
    GSource         source;	/**< glib source: list of callbacks etc. */
    GPollFD         pollfd;	/**< pollfd for mainloop (g_source_add_poll())*/
    guint           id;		/**< id (g_source_remove()) */

    union {
	listen_source_t listen;	/**< state for listen source */
	echo_source_t echo;	/**< state for echo source */
	chargen_source_t chargen;	/**< state for chargen source */
    };
} source_t;

static void     message(int msg);

static int      listensocket(int port);
static void     setnonblock(int fd);

static source_t *listensource(getclientsourcefunc getclientsource,
			      int port);
static GSource *getechoclientsource();
static GSource *getchargenclientsource();

static gboolean heartbeat(gpointer data);
static gboolean slowheartbeat(gpointer data);

static gboolean check(GSource * source);
static void     source_close(source_t * source);

static gboolean accept_prepare(GSource * source, gint * timeout);
static gboolean accept_dispatch(GSource * source, GSourceFunc callback,
				gpointer user_data);

static gboolean echo_prepare(GSource * source, gint * timeout);
static gboolean echo_dispatch(GSource * source, GSourceFunc callback,
			      gpointer user_data);

static void     echo_dispatch_read(source_t * echosource);
static void     echo_dispatch_write(source_t * echosource);

static gboolean chargen_prepare(GSource * source, gint * timeout);
static gboolean chargen_dispatch(GSource * source, GSourceFunc callback,
				 gpointer user_data);

/**
 * print message describing current activity
 *
 * @param msg	id of message to print (MSG_XYZ)
 */
void
message(int msg)
{
    switch (msg) {
    case MSG_HEARTBEAT:
	printf("H");
	break;
    case MSG_SLOWHEARTBEAT:
	printf("S");
	break;
    case MSG_MAINLOOP:
	printf("M");
	break;
    case MSG_ACCEPT:
	printf("A");
	break;
    case MSG_TOOMANY:
	printf("T");
	break;
    case MSG_CLOSE:
	printf("C");
	break;
    case MSG_READ:
	printf("R");
	break;
    case MSG_WRITE:
	printf("W");
	break;
    case MSG_FULL:
	printf("F");
	break;
    case MSG_EMPTY:
	printf("E");
	break;
    }
    fflush(stdout);
}

/**
 * block SIGPIPE
 *
 * Trying to write to a socket when the other end has already closed
 * the connection results in SIGPIPE. Not usefull in this context.
 */
static void
blocksigpipe()
{
    struct sigaction act;

    act.sa_handler = SIG_IGN;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESTART;
    if (sigaction(SIGPIPE, &act, NULL) == -1) {
	perror("sigaction(SIGPIPE, <ignore>)");
	exit(EXIT_FAILURE);
    }
}

/**
 * return tcp socket listening on port specified
 *
 * @param port	port number in host byte order
 *
 * @return	file descriptor for listening socket
 *
 * The socket is set to be nonblocking
 */
static int
listensocket(int port)
{
    int             serverfd;
    struct sockaddr_in sain;
    int             one;

    /* set up tcp socket for listening */
    sain.sin_family = AF_INET;
    sain.sin_port = htons(port);
    sain.sin_addr.s_addr = INADDR_ANY;
    if ((serverfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	perror("socket(AF_INET, SOCK_STREAM, 0)");
	exit(EXIT_FAILURE);
    }
    one = 1;
    if (setsockopt
	(serverfd, SOL_SOCKET, SO_REUSEADDR, &one,
	 (int) sizeof(one)) == -1) {
	perror("setsockopt(SO_REUSEADDR)");
	exit(EXIT_FAILURE);
    }
    if (bind
	(serverfd, (struct sockaddr *) &sain,
	 sizeof(struct sockaddr_in)) == -1) {
	perror("bind()");
	exit(EXIT_FAILURE);
    }
    if (listen(serverfd, 5) == -1) {
	perror("listen()");
	exit(EXIT_FAILURE);
    }
    setnonblock(serverfd);

    printf("listening on port %d\n", port);

    return serverfd;
}

/**
 * set a file descriptor to be nonblocking
 *
 * @param fd	file descriptor
 *
 * Non-Blocking works only for Sockets, Pipes and slow devices, it has
 * no effect when used with regular files
 */
static void
setnonblock(int fd)
{
    int             flag;

    flag = fcntl(fd, F_GETFL);
    fcntl(fd, F_GETFL, flag | O_NONBLOCK);
}

/**
 * construct listensource listening on port specified
 *
 * @param getclientsource	function returning clientsource for accepted connection
 * @param port	tcp port to listen on in host byte order
 *
 * @return	listensource_t
 */
static source_t *
listensource(getclientsourcefunc getclientsource, int port)
{
    static GSourceFuncs listenfuncs;
    source_t       *listensource;

    listenfuncs.prepare = &accept_prepare;
    listenfuncs.check = &check;
    listenfuncs.dispatch = &accept_dispatch;
    listenfuncs.finalize = NULL;
    listensource =
	(source_t *) g_source_new(&listenfuncs, sizeof(source_t));
    listensource->listen.getclientsource = getclientsource;

    listensource->pollfd.fd = listensocket(port);
    g_source_add_poll((GSource *) listensource, &(listensource->pollfd));
    return listensource;
}

/**
 * print heartbeat message
 *
 * @return always TRUE
 */
static          gboolean
heartbeat(gpointer data)
{
    message(MSG_HEARTBEAT);
    return TRUE;
}

/**
 * print "slowheartbeat" message
 *
 * @return always TRUE
 */
static          gboolean
slowheartbeat(gpointer data)
{
    message(MSG_SLOWHEARTBEAT);
    return TRUE;
}

/**
 * check whether source is ready for processing
 *
 * @param source	GSource to check
 *
 * @return	TRUE if resource is ready
 */
static          gboolean
check(GSource * source)
{

    source_t       *xsource;

    xsource = (source_t *) source;

    return xsource->pollfd.revents ? TRUE : FALSE;
}

/**
 * Prepare GSource for polling a server socket for accept
 *
 * @param source	GSource to prepare
 * @param timeout	maximum timeout to set for poll() (out)
 *
 * @return	always FALSE (use poll)
 */
static          gboolean
accept_prepare(GSource * source, gint * timeout)
{
    source_t       *xsource;

    xsource = (source_t *) source;

    xsource->pollfd.events = G_IO_IN;
    *timeout = -1;		/* no timeout */
    return FALSE;
}

/**
 * dispatch (process) listening socket
 *
 * @param gsource	source to process
 * @param callback	callback function (unused)
 * @param user_data	I have absolutely no idea how to use this... nice idea though
 *
 * @return	always TRUE
 */
static          gboolean
accept_dispatch(GSource * gsource, GSourceFunc callback,
		gpointer user_data)
{
    struct sockaddr_in sain;
    int             fd;
    socklen_t       t;
    source_t       *source;
    source_t       *clientsource;

    source = (source_t *) gsource;

    if (source->pollfd.revents & (G_IO_HUP | G_IO_ERR)) {
	perror("accept_dispatch()");
	exit(EXIT_FAILURE);
    }

    /* accept new connection */
    t = sizeof(sain);
    if ((fd = accept(source->pollfd.fd, (void *) &sain, &t)) == -1) {
	if (errno == EWOULDBLOCK) {
	    return TRUE;
	}
	perror("accept(ECHO)");
	exit(EXIT_FAILURE);
    }
    setnonblock(fd);
    message(MSG_ACCEPT);

    clientsource = (source_t *) source->listen.getclientsource();
    clientsource->pollfd.fd = fd;
    g_source_add_poll((GSource *) clientsource, &(clientsource->pollfd));
    clientsource->id =
	g_source_attach((GSource *) clientsource,
			g_source_get_context((GSource *) clientsource));
    return TRUE;
}

/**
 * construct GSourceFuncs for echo client
 *
 * @return	GSource for echo client
 */
static GSource *
getechoclientsource()
{
    static GSourceFuncs funcs = { NULL, NULL, NULL, NULL, NULL, NULL };
    source_t       *echosource;

    /* cannot reference functions in static initializer */
    funcs.prepare = echo_prepare;
    funcs.check = check;
    funcs.dispatch = echo_dispatch;
    funcs.finalize = NULL;

    echosource = (source_t *) g_source_new(&funcs, sizeof(source_t));
    echosource->echo.r = 0;
    echosource->echo.w = 0;
    echosource->echo.n = 0;
    return (GSource *) echosource;
}

/**
 * construct GSourceFuncs for chargen client
 *
 * @return	GSource for chargen client
 */
static GSource *
getchargenclientsource()
{
    static GSourceFuncs funcs = { NULL, NULL, NULL, NULL, NULL, NULL };
    source_t       *chargensource;

    /* cannot reference functions in static initializer */
    funcs.prepare = chargen_prepare;
    funcs.check = check;
    funcs.dispatch = chargen_dispatch;
    funcs.finalize = NULL;

    chargensource = (source_t *) g_source_new(&funcs, sizeof(source_t));
    chargensource->chargen.i = 0;
    return (GSource *) chargensource;
}

/**
 * Prepare GSource for polling an echo client
 *
 * @param source	GSource to prepare
 * @param timeout	maximum timeout to set for poll() (out)
 *
 * @return	always FALSE (use poll)
 *
 * If the buffer is full, checking for read is turned off resp. if the
 * buffer is empty, checking for write is turned off.
 */
static          gboolean
echo_prepare(GSource * source, gint * timeout)
{
    source_t       *echosource;

    echosource = (source_t *) source;

    switch (echosource->echo.n) {
    case 0:
	echosource->pollfd.events = G_IO_IN;
	break;
    case BUFSIZE:
	echosource->pollfd.events = G_IO_OUT;
	break;
    default:
	echosource->pollfd.events = G_IO_IN | G_IO_OUT;
    }
    return FALSE;
}

/**
 * dispatch (process) echo client
 *
 * @param source	source to process
 * @param callback	callback function (unused)
 * @param user_data	?
 *
 * @return	always TRUE
 */
static          gboolean
echo_dispatch(GSource * source, GSourceFunc callback, gpointer user_data)
{
    source_t       *echosource;

    echosource = (source_t *) source;

    if (echosource->pollfd.revents & (G_IO_HUP | G_IO_ERR)) {
	source_close((source_t *) echosource);
    }
    if (echosource->pollfd.revents & G_IO_IN) {
	echo_dispatch_read(echosource);
    }
    if (echosource->pollfd.revents & G_IO_OUT) {
	echo_dispatch_write(echosource);
    }
    return TRUE;
}

/**
 * read data from echo client
 *
 * @param echosource	echo client source
 *
 * If the buffer is not full, tries to do one read from the
 * filedescriptor associated with this echo client.
 */
static void
echo_dispatch_read(source_t * echosource)
{
    ssize_t         nread;

    if (echosource->echo.n == BUFSIZE) {
	message(MSG_FULL);
	return;
    }

    if (echosource->echo.r >= echosource->echo.w) {
	nread =
	    read(echosource->pollfd.fd,
		 echosource->echo.buf + echosource->echo.r,
		 BUFSIZE - echosource->echo.r);
    } else {
	nread =
	    read(echosource->pollfd.fd,
		 echosource->echo.buf + echosource->echo.r,
		 echosource->echo.w - echosource->echo.r);
    }

    switch (nread) {
    case -1:
	if ((errno != EINTR) && (errno != EWOULDBLOCK)) {
	    perror("read()");
	    exit(EXIT_FAILURE);
	}
	break;
    case 0:
	source_close((source_t *) echosource);
	return;
    default:
	message(MSG_READ);
	echosource->echo.n += nread;
	echosource->echo.r += nread;
	echosource->echo.r %= BUFSIZE;
    }
}

/**
 * write data to an echo client
 *
 * @param echosource	echo client source
 *
 * If the buffer is not empty, tries to do one write to the
 * filedescriptor associated with the echo client.
 */
static void
echo_dispatch_write(source_t * echosource)
{
    ssize_t         nwrite;

    if (echosource->echo.n == 0) {
	message(MSG_EMPTY);
	return;
    }

    if (echosource->echo.r > echosource->echo.w) {
	nwrite =
	    write(echosource->pollfd.fd,
		  echosource->echo.buf + echosource->echo.w,
		  echosource->echo.r - echosource->echo.w);
    } else {
	nwrite =
	    write(echosource->pollfd.fd,
		  echosource->echo.buf + echosource->echo.w,
		  BUFSIZE - echosource->echo.w);
    }

    switch (nwrite) {
    case -1:
	if (errno == EPIPE) {
	    source_close((source_t *) echosource);
	} else if ((errno != EINTR) && (errno != EWOULDBLOCK)) {
	    perror("write()");
	    exit(EXIT_FAILURE);
	}
	break;
    case 0:
	break;
    default:
	message(MSG_WRITE);
	echosource->echo.n -= nwrite;
	echosource->echo.w += nwrite;
	echosource->echo.w %= BUFSIZE;
    }
}

/**
 * close a source and dispose of source
 *
 * @param source	source to close/dispose
 */
static void
source_close(source_t * source)
{
    message(MSG_CLOSE);
    (void) g_source_remove(source->id);
    close(source->pollfd.fd);
    g_source_unref((GSource *) source);
}

/**
 * Prepare GSource for polling an chargen client
 *
 * @param source	GSource to prepare
 * @param timeout	maximum timeout to set for poll() (out)
 *
 * @return	always FALSE (use poll)
 */
static          gboolean
chargen_prepare(GSource * source, gint * timeout)
{
    source_t       *chargensource;

    chargensource = (source_t *) source;

    chargensource->pollfd.events = G_IO_OUT;

    return FALSE;
}

/**
 * dispatch (process) chargen client
 *
 * @param source	source to process
 * @param callback	callback function (unused)
 * @param user_data	?
 *
 * @return	always TRUE
 *
 * there is no chargen_dispatch_write, writing is done directly in
 * chargen_dispatch
 */
static          gboolean
chargen_dispatch(GSource * source, GSourceFunc callback,
		 gpointer user_data)
{
    ssize_t         nwrite;
    source_t       *chargensource;

    chargensource = (source_t *) source;

    nwrite =
	write(chargensource->pollfd.fd,
	      chargen_buf + chargensource->chargen.i,
	      sizeof(chargen_buf) - chargensource->chargen.i);
    switch (nwrite) {
    case -1:
	if (errno == EPIPE) {
	    source_close((source_t *) chargensource);
	    return TRUE;
	} else if ((errno != EINTR) && (errno != EWOULDBLOCK)) {
	    perror("write()");
	    exit(EXIT_FAILURE);
	}
	break;
    case 0:
	break;
    default:
	message(MSG_WRITE);
	chargensource->chargen.i += nwrite;
	chargensource->chargen.i %= sizeof(chargen_buf);
    }
    return TRUE;
}

int
main()
{
    GMainLoop      *mainloop;
    GMainContext   *context;
    source_t       *listenecho, *listenchargen;

    /* check glib version */
    if (!GLIB_CHECK_VERSION(2, 0, 0)) {
	fprintf(stderr, "glib %d.%d.%d is too old\n", GLIB_MAJOR_VERSION,
		GLIB_MINOR_VERSION, GLIB_MICRO_VERSION);
	exit(EXIT_FAILURE);
    }

    printf("example: glib main loop\n");

    blocksigpipe();

    /* create main loop */
    context = g_main_context_default();
    mainloop = g_main_loop_new(context, FALSE);

    /* create echo service */
    listenecho = listensource(getechoclientsource, PORT_ECHO);
    listenecho->id = g_source_attach((GSource *) listenecho, context);

    /* create chargen service */
    listenchargen = listensource(getchargenclientsource, PORT_CHARGEN);
    listenchargen->id =
	g_source_attach((GSource *) listenchargen, context);

    /* install heartbeat */
    (void) g_timeout_add(1000 * HEARTBEAT_INTERVAL, &heartbeat, NULL);
    (void) g_timeout_add(1000 * SLOWHEARTBEAT_INTERVAL, &slowheartbeat, NULL);
    printf("heartbeat every %d seconds\n", HEARTBEAT_INTERVAL);
    printf("slow heartbeat every %d seconds\n", SLOWHEARTBEAT_INTERVAL);

    /* run the main loop */
    g_main_loop_run(mainloop);
    /* notreached */

    g_main_loop_unref(mainloop);
    exit(EXIT_SUCCESS);
}
