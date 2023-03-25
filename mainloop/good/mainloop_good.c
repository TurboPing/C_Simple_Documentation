/**
 * @file
 * Example for "good" main loop
 *
 * @author Rico Pajarola
 *
 * This example does essentially what glib would do: events are
 * abstracted using callbacks making the mainloop generic (but not as
 * generic as glib). Even though it is easy to add a new input or event
 * source, the whole mechanism is still very much tied to the structure
 * of this program).
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

/** maximum number of clients */
#define MAXCLIENTS 8

/** maximum number of different alarms */
#define MAXALARMS 8

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

#define MIN(a, b)       ((a)<(b)?(a):(b))
#define MAX(a, b)       ((a)>(b)?(a):(b))

/** characters to return in chargen service */
static char     chargen_buf[] = "0123456789abcdefghijklmnopqrstuv";

/**
 * echo client specific state
 */
typedef struct {
    int             r;		/**< read position */
    int             w;		/**< write position */
    int             n;		/**< number of bytes */
    char            buf[BUFSIZE];	/**< buffer */
} echo_client_t;

/**
 * chargen client specific state
 */
typedef struct {
    int             i;		/**< index into chargen_buf */
} chargen_client_t;

/**
 * event handler
 *
 * @param i index into clients
 */
typedef void    (*eventhandler_t) (int i);

/**
 * client state
 *
 * in a real program, a pointer to a type specific data area would be
 * preferrable to a union.
 */
typedef struct {
    int             fd;		/**< filedescriptor for poll() */
    eventhandler_t  read;	/**< read handler */
    eventhandler_t  write;	/**< write handler */
    eventhandler_t  except;	/**< exception (eg disconnect) handler */
    union {
	echo_client_t   echo;	/**< echo client state */
	chargen_client_t chargen;	/**< chargen client state */
    };
} client_t;

/**
 * alarm handler
 */
typedef void    (*alarmhandler_t) (void);

/**
 * alarm handler state
 *
 * handler, interval, next execution, pending. no handler specific
 * state...
 */
typedef struct {
    alarmhandler_t  handler;	/**< alarm handler */
    long            interval;	/**< interval in seconds */
    long            nexttime;	/**< next execution at */
    int             flag;	/**< 1 if handler should be called */
} alarm_t;

/**
 * there's a global (static) array for all client state (clients). this is
 * not as bad as it sounds because there's a static array for the struct
 * pollfd anyway (pollfds). using an array for the client states allows using 
 * the same index for pollfds and clients.
 * 
 * in a real application, the buffer would be bigger, and therefore wouldn't 
 * be placed into the array ;) for demonstration purposes, the buffer is
 * small to make it more interesting
 */
static unsigned long npollfd;	/**< number of file descriptors to check */
static client_t clients[MAXCLIENTS];	/**< array of client states */
static struct pollfd pollfds[MAXCLIENTS];	/**< pollfds for poll() */
static alarm_t  alarms[MAXALARMS];	/**< alarms */

static void     message(int msg);

static void     blocksigpipe(void);
static void     initalarms(void);
static void     addalarm(alarmhandler_t handler, long interval);
static void     checkalarms(void);
static void     alarmhandler(void);
static void     heartbeat(void);
static void     slowheartbeat(void);

static int      listensocket(int port);
static void     setnonblock(int fd);

static void     initclients();
static void     addclient(int fd, eventhandler_t read,
			  eventhandler_t write, eventhandler_t except);
static void     delclient(int i);
static void     mainloop();

static void     closeclient(int i);
static void     acceptecho(int i);
static void     readecho(int i);
static void     writeecho(int i);
static void     flowecho(int i);

static void     acceptchargen(int i);
static void     writechargen(int i);

/**
 * print message describing current activity
 *
 * @param msg id of message to print (MSG_XYZ)
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
	/* printf("M"); */
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
 * install non-resetting signal handler for alarms
 *
 * The sigaction interface allows installing non-resetting signal
 * handlers (ie not reset to SIG_DFL after disposition). This way,
 * there is no race condition when reinstalling the signal handler.
 */
static void
initalarms()
{
    struct sigaction act;
    int             i;

    act.sa_handler = alarmhandler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;		/* no SIG_RESTART! */
    if (sigaction(SIGALRM, &act, NULL) == -1) {
	perror("sigaction(SIGPIPE, <ignore>)");
	exit(EXIT_FAILURE);
    }
    for (i = 0; i < MAXALARMS; i++) {
	alarms[i].handler = NULL;
    }
    alarm(0);			/* cancel any previously made alarm request */
}

/**
 * add new alarm
 *
 * @param handler alarm handler procedure
 * @param interval interval in seconds
 *
 * @bug no error handling, if there are no more error handler slots,
 * the new alarm is ignored...
 */
static void
addalarm(alarmhandler_t handler, long interval)
{
    struct timeval  now;
    int             i;

    for (i = 0; i < MAXALARMS; i++) {
	if (alarms[i].handler == NULL) {
	    gettimeofday(&now, NULL);
	    alarms[i].handler = handler;
	    alarms[i].interval = interval;
	    alarms[i].nexttime = now.tv_sec + interval;
	    alarms[i].flag = 0;
	    alarmhandler();
	    return;
	}
    }
}

/**
 * check for pending alarms and execute alarm handlers
 */
static void
checkalarms(void)
{
    int             i;

    for (i = 0; i < MAXALARMS; i++) {
	if (alarms[i].flag) {
	    (*alarms[i].handler) ();
	    alarms[i].flag = 0;
	}
    }
}

/**
 * signal handler for alarms
 *
 * mark expired alarms for execution and set new alarm timer.
 *
 * the resolution of the alarm timer is one second, no attempt is made
 * to get timing beyond this one second resolution (any sub-second
 * timing information is discarded).
 */
static void
alarmhandler(void)
{
    struct timeval  now;
    long            nextalarm;
    int             i;

    gettimeofday(&now, NULL);

    nextalarm = 65536;
    for (i = 0; i < MAXALARMS; i++) {
	if (alarms[i].handler != NULL) {
	    if (alarms[i].nexttime <= now.tv_sec) {
		alarms[i].flag = 1;
		alarms[i].nexttime += alarms[i].interval;
	    }
	    nextalarm =
		MIN(nextalarm,
		    MAX(1,
			(unsigned int) alarms[i].nexttime - now.tv_sec));
	}
    }
    alarm((unsigned int) nextalarm);
}

/**
 * print heartbeat message
 */
static void
heartbeat()
{
    message(MSG_HEARTBEAT);
}

/**
 * print "slowheartbeat" message
 */
static void
slowheartbeat()
{
    message(MSG_SLOWHEARTBEAT);
}

/**
 * return tcp socket listening on port specified
 *
 * @param port  port number in host byte order
 *
 * @return file descriptor for listening socket
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
	(serverfd, (struct sockaddr *) &sain, sizeof(struct sockaddr_in))
	== -1) {
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
 * @param fd file descriptor
 *
 * Non-Blocking works only for Sockets, Pipes and slow devices, it has
 * no effect when used with regular files or "fast" devices.
 */
static void
setnonblock(int fd)
{
    int             flag;

    flag = fcntl(fd, F_GETFL);
    fcntl(fd, F_GETFL, flag | O_NONBLOCK);
}

/**
 * initialize client state array
 */
static void
initclients()
{
    int             i;

    for (i = 0; i < MAXCLIENTS; i++) {
	clients[i].fd = -1;
    }
    npollfd = 0;
}

/**
 * add new client
 *
 * @param fd file descriptor asociated with this client
 * @param read handler called if socket is readable
 * @param write handler called if socket is writable
 * @param except handler called on exceptions (HUP, close etc).
 */
static void
addclient(int fd, eventhandler_t read, eventhandler_t write,
	  eventhandler_t except)
{
    if (npollfd >= (MAXCLIENTS - 1)) {
	close(fd);
	message(MSG_TOOMANY);
	return;
    }
    clients[npollfd].fd = fd;
    clients[npollfd].read = read;
    clients[npollfd].write = write;
    clients[npollfd].except = except;
    pollfds[npollfd].fd = fd;
    pollfds[npollfd].events = 0;
    if (read) {
	pollfds[npollfd].events |= POLLIN;
    }
    if (write) {
	pollfds[npollfd].events |= POLLOUT;
    }
    npollfd++;
}

/**
 * delete client
 *
 * @param i index into pollfds/clients array
 */
static void
delclient(int i)
{
    clients[i].fd = -1;
    if (i != npollfd) {
	/* just copy the whole thing including the buffer... */
	memcpy(&clients[i], &clients[npollfd], sizeof(clients[i]));
	pollfds[i] = pollfds[npollfd];
	npollfd--;
    }
}

/**
 * execute one iteration of THE mainloop
 */
static void
mainloop()
{
    int             i;

    message(MSG_MAINLOOP);
    i = poll(pollfds, npollfd, -1);

    /* check for alarm handlers to be executed */
    checkalarms();

    if (i == -1) {
	if ((errno != EAGAIN) && (errno != EINTR)) {
	    perror("poll()");
	    exit(EXIT_FAILURE);
	}
	return;
    }

    /* 
     * handle i/o events. work off exceptions first. then read (try to fill
     * buffer) and write (try to empty buffer)
     */
    for (i = 0; i <= npollfd; i++) {
	if ((clients[i].except)
	    && (pollfds[i].revents & (POLLERR | POLLHUP | POLLNVAL))) {
	    (*clients[i].except) (i);
	    continue;
	}
	if ((clients[i].read) && (pollfds[i].revents & POLLIN)) {
	    (*clients[i].read) (i);
	}
	if ((clients[i].write) && (pollfds[i].revents & POLLOUT)) {
	    (*clients[i].write) (i);
	}
    }
}

/**
 * accept connection and set up new session for "echo" service
 *
 * @param i index into pollfds/clients array for server descriptor
 */
static void
acceptecho(int i)
{
    int             fd;
    socklen_t       t;
    struct sockaddr_in sain;

    t = sizeof(sain);
    if ((fd = accept(clients[i].fd, (void *) &sain, &t)) == -1) {
	if (errno == EWOULDBLOCK) {
	    return;
	}
	perror("accept(ECHO)");
	exit(EXIT_FAILURE);
    }
    message(MSG_ACCEPT);
    addclient(fd, &readecho, &writeecho, &closeclient);
}

/**
 * read data from echo client
 *
 * @param i index into pollfds/clients array for echo session
 *
 * If the buffer is not full, tries to do one read from the
 * filedescriptor associated with this echo client.
 */
static void
readecho(int i)
{
    ssize_t         nread;

    if (clients[i].echo.n == BUFSIZE) {
	message(MSG_FULL);
	flowecho(i);
	return;
    }
    if (clients[i].echo.r >= clients[i].echo.w) {
	nread =
	    read(clients[i].fd, clients[i].echo.buf + clients[i].echo.r,
		 BUFSIZE - clients[i].echo.r);
    } else {
	nread =
	    read(clients[i].fd, clients[i].echo.buf + clients[i].echo.r,
		 clients[i].echo.w - clients[i].echo.r);
    }

    switch (nread) {
    case -1:
	if ((errno != EINTR) && (errno != EWOULDBLOCK)) {
	    perror("read()");
	    exit(EXIT_FAILURE);
	}
	break;
    case 0:
	closeclient(i);
	return;
    default:
	message(MSG_READ);
	clients[i].echo.n += nread;
	clients[i].echo.r += nread;
	clients[i].echo.r %= BUFSIZE;
	flowecho(i);
    }
}

/**
 * write data to an echo client
 *
 * @param i index into pollfds/clients array for echo session
 *
 * If the buffer is not empty, tries to do one write to the
 * filedescriptor associated with the echo client.
 */
static void
writeecho(int i)
{
    ssize_t         nwrite;

    if (clients[i].echo.n == 0) {
	message(MSG_EMPTY);
	flowecho(i);
	return;
    }
    if (clients[i].echo.r > clients[i].echo.w) {
	nwrite =
	    write(clients[i].fd, clients[i].echo.buf + clients[i].echo.w,
		  clients[i].echo.r - clients[i].echo.w);
    } else {
	nwrite =
	    write(clients[i].fd, clients[i].echo.buf + clients[i].echo.w,
		  BUFSIZE - clients[i].echo.w);
    }

    switch (nwrite) {
    case -1:
	if (errno == EPIPE) {
	    closeclient(i);
	} else if ((errno != EINTR) && (errno != EWOULDBLOCK)) {
	    perror("write()");
	    exit(EXIT_FAILURE);
	}
	break;
    case 0:
	break;
    default:
	message(MSG_WRITE);
	clients[i].echo.n -= nwrite;
	clients[i].echo.w += nwrite;
	clients[i].echo.w %= BUFSIZE;
	flowecho(i);
    }
}

/**
 * do "flow" control for echo
 *
 * @param i index into pollfds/clients array for echo session
 *
 * turn off checking for read if buffer is full resp. write if buffer
 * is empty.
 */
static void
flowecho(int i)
{
    switch (clients[i].echo.n) {
    case 0:
	pollfds[i].events = POLLIN;
	break;
    case BUFSIZE:
	pollfds[i].events = POLLOUT;
	break;
    default:
	pollfds[i].events = POLLIN | POLLOUT;
    }
}

/**
 * close and delete client session
 *
 * @param i index into pollfds/clients array
 */
static void
closeclient(int i)
{
    message(MSG_CLOSE);
    close(clients[i].fd);
    delclient(i);
    clients[i].fd = -1;
}

/**
 * accept connection and set up new session for "chargen" service
 *
 * @param i index into pollfds/clients array for server descriptor
 */
static void
acceptchargen(int i)
{
    int             fd;
    socklen_t       t;
    struct sockaddr_in sain;

    t = sizeof(sain);
    if ((fd = accept(clients[i].fd, (void *) &sain, &t)) == -1) {
	if (errno == EWOULDBLOCK) {
	    return;
	}
	perror("accept(CHARGEN)");
	exit(EXIT_FAILURE);
    }
    addclient(fd, NULL, &writechargen, &closeclient);
}

/**
 * write data to an chargen client
 *
 * @param i index into pollfds/clients array for chargen session
 *
 * If the buffer is not empty, tries to do one write to the
 * filedescriptor associated with the chargen client.
 */
static void
writechargen(int i)
{
    ssize_t         nwrite;

    nwrite =
	write(clients[i].fd, chargen_buf + clients[i].chargen.i,
	      sizeof(chargen_buf) - clients[i].chargen.i);

    switch (nwrite) {
    case -1:
	if (errno == EPIPE) {
	    closeclient(i);
	    return;
	} else if ((errno != EINTR) && (errno != EWOULDBLOCK)) {
	    perror("write()");
	    exit(EXIT_FAILURE);
	}
	break;
    case 0:
	break;
    default:
	message(MSG_WRITE);
	clients[i].chargen.i += nwrite;
	clients[i].chargen.i %= sizeof(chargen_buf);
    }
}

int
main()
{
    printf("example: better main loop\n");

    blocksigpipe();

    initclients();
    addclient(listensocket(PORT_ECHO), &acceptecho, NULL, NULL);
    addclient(listensocket(PORT_CHARGEN), &acceptchargen, NULL, NULL);
    initalarms();
    addalarm(heartbeat, HEARTBEAT_INTERVAL);
    addalarm(slowheartbeat, SLOWHEARTBEAT_INTERVAL);
    printf("heartbeat every %d seconds\n", HEARTBEAT_INTERVAL);
    printf("slow heartbeat every %d seconds\n", SLOWHEARTBEAT_INTERVAL);

    /* main loop */
    while (1) {
	mainloop();
    }
    /* notreached */
}
