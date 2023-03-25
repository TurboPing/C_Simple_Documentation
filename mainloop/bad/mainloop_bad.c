/**
 * @file
 * Example for "bad" main loop
 *
 * @author Rico Pajarola
 *
 * This example tries to do everything as bad as possible without doing
 * it just plain wrong (that's not as easy as it sounds). This is done
 * by putting all the logic into one huge main loop, and explicitly spelling
 * out all special cases in place.
 *
 * Apart from the time spent trying to find worse ways to do things,
 * this example was completed really quick. That is, at least until I
 * tried to add a second client and a second timer...
 *
 * To emphasize that this is the bad example, select() is used.
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
#define MAXCLIENTS 4

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

#define MIN(a, b)	((a)<(b)?(a):(b))
#define MAX(a, b)	((a)>(b)?(a):(b))

/** characters to @return in chargen service */
static char     chargen_buf[] = "0123456789abcdefghijklmnopqrstuv";

/**
 * echo client state
 */
typedef struct {
    int             fd;	/**< socket */
    int             r;	/**< read position */
    int             w;	/**< write position */
    int             n;	/**< number of bytes */
    char            buf[BUFSIZE];	/**< buffer */
} echoclient_t;

typedef struct {
    int             fd;	/**< socket */
    int             i;		/**< index into chargen_buf */
} chargenclient_t;

static void     message(int msg);
static int      listensocket(int port);
static void     setnonblock(int fd);

static void     sighandler(int signo);

static void     readecho(echoclient_t * client);
static void     writeecho(echoclient_t * client);
static void     writechargen(chargenclient_t * client);

/**
 * print message describing current activity
 *
 * @param msg	id of message to print (MSG_XYZ)
 */
static void
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
 * signal handler
 *
 * @param signo	signal number
 */
static void
sighandler(int signo)
{
    struct timeval  now;
    static time_t   time_heartbeat, time_slowheartbeat;
    int             nextalarm;

    switch (signo) {
    case SIGALRM:
	/* 
	 * (re)install signal handler and set new alarm
	 * there is a possible race condition, but for alarm()/SIGALRM this is not a real concern
	 */
	if (signal(SIGALRM, &sighandler) == SIG_ERR) {
	    perror("signal(SIGALRM)");
	    exit(EXIT_FAILURE);
	}
	gettimeofday(&now, NULL);
	if (time_heartbeat == 0) {
	    time_heartbeat = now.tv_sec + HEARTBEAT_INTERVAL;
	    time_slowheartbeat = now.tv_sec + SLOWHEARTBEAT_INTERVAL;
	}
	if (time_heartbeat <= now.tv_sec) {
	    time_heartbeat += HEARTBEAT_INTERVAL;
	    message(MSG_HEARTBEAT);
	}
	if (time_slowheartbeat <= now.tv_sec) {
	    time_slowheartbeat += SLOWHEARTBEAT_INTERVAL;
	    message(MSG_SLOWHEARTBEAT);
	}

	nextalarm =
	    MAX(1, MIN(time_heartbeat, time_slowheartbeat) - now.tv_sec);

	if (alarm(nextalarm) == -1) {
	    perror("alarm()");
	    exit(EXIT_FAILURE);
	}
	break;
    default:
	/* ignore everything */
	break;
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
 * read data from an echo client
 *
 * @param client	echo client state
 *
 * If the buffer is not full, tries to do one read from the
 * filedescriptor associated with the echo client.
 */
static void
readecho(echoclient_t * client)
{
    int             nread;

    if (client->n == BUFSIZE) {
	message(MSG_FULL);
	return;
    }

    if (client->r >= client->w) {
	nread =
	    read(client->fd, client->buf + client->r, BUFSIZE - client->r);
    } else {
	nread =
	    read(client->fd, client->buf + client->r,
		 client->w - client->r);
    }

    switch (nread) {
    case 0:
	message(MSG_CLOSE);
	close(client->fd);
	client->fd = -1;
	break;
    case -1:
	if ((errno != EINTR) && (errno != EWOULDBLOCK)) {
	    perror("read()");
	    exit(EXIT_FAILURE);
	}
	break;
    default:
	message(MSG_READ);
	client->n += nread;
	client->r += nread;
	client->r %= BUFSIZE;
    }
}

/**
 * write data to an echo client
 *
 * @param client	echo client state
 *
 * If the buffer is not empty, tries to do one write to the
 * filedescriptor associated with the echo client.
 */
static void
writeecho(echoclient_t * client)
{
    int             nwrite;

    if (client->n == 0) {
	message(MSG_EMPTY);
	return;
    }

    if (client->r > client->w) {
	nwrite =
	    write(client->fd, client->buf + client->w,
		  client->r - client->w);
    } else {
	nwrite =
	    write(client->fd, client->buf + client->w,
		  BUFSIZE - client->w);
    }

    switch (nwrite) {
    case -1:
	if (errno == EPIPE) {
	    message(MSG_CLOSE);
	    close(client->fd);
	    client->fd = -1;
	} else if ((errno != EINTR) && (errno != EWOULDBLOCK)) {
	    perror("write()");
	    exit(EXIT_FAILURE);
	}
	break;
    case 0:
	break;
    default:
	message(MSG_WRITE);
	client->n -= nwrite;
	client->w += nwrite;
	client->w %= BUFSIZE;
    }
}

/**
 * write data to a chargen client
 *
 * @param client	chargen client state
 */
static void
writechargen(chargenclient_t * client)
{
    int             nwrite;

    nwrite =
	write(client->fd, chargen_buf + client->i,
	      sizeof(chargen_buf) - client->i);

    switch (nwrite) {
    case -1:
	if (errno == EPIPE) {
	    message(MSG_CLOSE);
	    close(client->fd);
	    client->fd = -1;
	} else if ((errno != EINTR) && (errno != EWOULDBLOCK)) {
	    perror("write()");
	    exit(EXIT_FAILURE);
	}
	break;
    case 0:
	break;
    default:
	message(MSG_WRITE);
	client->i += nwrite;
	client->i %= sizeof(chargen_buf);
    }
}

int
main()
{
    int             echoserverfd;
    int             chargenserverfd;
    echoclient_t    echoclients[MAXCLIENTS];
    chargenclient_t chargenclients[MAXCLIENTS];
    fd_set          rfdset, wfdset;
    int             fdsetmax;
    int             n, i;
    size_t          s;
    struct sockaddr_in sain;

    printf("example: bad main loop\n");

    /* 
     * ignore SIGPIPE (this occurs whenever a chargen client closes
     * the connection).
     */
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
	perror("signal(SIGPIPE, SIG_IGN)");
	exit(EXIT_FAILURE);
    }

    /* 
     * install heartbeat. this is done by calling the signal
     * handler for SIGALRM which then installs itself as a signal
     * handler and starts the alarm clock.
     */
    printf("heartbeat every %d seconds\n", HEARTBEAT_INTERVAL);
    printf("slow heartbeat every %d seconds\n", SLOWHEARTBEAT_INTERVAL);
    sighandler(SIGALRM);

    /* 
     * create server sockets for echo and chargen services
     */
    echoserverfd = listensocket(PORT_ECHO);
    chargenserverfd = listensocket(PORT_CHARGEN);

    /* 
     * reset all client state slots (mark as unused)
     */
    for (i = 0; i < MAXCLIENTS; i++) {
	echoclients[i].fd = -1;
	chargenclients[i].fd = -1;
    }

    /* 
     * initialize fdsets for select()
     */
    FD_ZERO(&rfdset);
    FD_ZERO(&wfdset);
    FD_SET(STDIN_FILENO, &rfdset);
    FD_SET(echoserverfd, &rfdset);	/* fd 3 */
    FD_SET(chargenserverfd, &rfdset);	/* fd 4 */
    fdsetmax = chargenserverfd;	/* fd 4 */

    /* 
     * THE main loop
     * Remeber, this is the bad example. On Solaris, select() is a
     * (rather clumsy) wrapper around poll(). There is no way to efficiently
     * emulate select() using poll().
     */
    while (((n = select(fdsetmax + 1, &rfdset, &wfdset, NULL, NULL)) != -1)
	   || (errno == EINTR)) {
	message(MSG_MAINLOOP);

	if (n == -1) {
	    /* got -1 and errno==EINTR */
	    continue;
	}

	/* 
	 * check for new echo connections
	 */
	if (FD_ISSET(echoserverfd, &rfdset)) {
	    /* find free slot */
	    for (i = 0; (i < MAXCLIENTS) && (echoclients[i].fd != -1);
		 i++);
	    if (echoclients[i].fd != -1) {
		/* no free slots */
		s = sizeof(sain);
		i = accept(echoserverfd, (struct sockaddr *) &sain, &s);
		close(i);
		message(MSG_TOOMANY);
	    } else {
		s = sizeof(sain);
		echoclients[i].fd =
		    accept(echoserverfd, (struct sockaddr *) &sain, &s);
		message(MSG_ACCEPT);
		setnonblock(echoclients[i].fd);
		echoclients[i].r = 0;	/* start reading from buffer at pos 0 */
		echoclients[i].w = 0;	/* start writing to buffer at pos 0 */
		echoclients[i].n = 0;	/* 0 bytes in buffer */
	    }
	}

	/* 
	 * check for new chargen connections
	 */
	if (FD_ISSET(chargenserverfd, &rfdset)) {
	    /* find free slot */
	    for (i = 0; (i < MAXCLIENTS) && (chargenclients[i].fd != -1);
		 i++);
	    if (chargenclients[i].fd != -1) {
		/* no free slots */
		s = sizeof(sain);
		i = accept(chargenserverfd, (struct sockaddr *) &sain, &s);
		close(i);
		message(MSG_TOOMANY);
	    } else {
		s = sizeof(sain);
		chargenclients[i].fd =
		    accept(chargenserverfd, (struct sockaddr *) &sain, &s);
		message(MSG_ACCEPT);
		setnonblock(chargenclients[i].fd);
		chargenclients[i].i = 0;	/* start sending from buffer
						 * at pos 0 */
	    }
	}

	/* 
	 * try to read/write data for echo clients
	 */
	for (i = 0; i < MAXCLIENTS; i++) {
	    if ((echoclients[i].fd != -1)
		&& (FD_ISSET(echoclients[i].fd, &rfdset))) {
		readecho(&echoclients[i]);
	    }
	    if ((echoclients[i].fd != -1)
		&& (FD_ISSET(echoclients[i].fd, &wfdset))) {
		writeecho(&echoclients[i]);
	    }
	}

	/* 
	 * try to write data for chargen clients
	 */
	for (i = 0; i < MAXCLIENTS; i++) {
	    if ((chargenclients[i].fd != -1)
		&& (FD_ISSET(chargenclients[i].fd, &wfdset))) {
		writechargen(&chargenclients[i]);
	    }
	}

	/* 
	 * reinitialize fdset
	 */
	FD_ZERO(&rfdset);
	FD_ZERO(&wfdset);
	fdsetmax = chargenserverfd;
	FD_SET(echoserverfd, &rfdset);
	FD_SET(chargenserverfd, &rfdset);
	for (i = 0; i < MAXCLIENTS; i++) {
	    if (echoclients[i].fd >= 0) {
		if (echoclients[i].n < BUFSIZE) {
		    FD_SET(echoclients[i].fd, &rfdset);
		    if (echoclients[i].fd > fdsetmax) {
			fdsetmax = echoclients[i].fd;
		    }
		}
		if (echoclients[i].n > 0) {
		    FD_SET(echoclients[i].fd, &wfdset);
		    if (echoclients[i].fd > fdsetmax) {
			fdsetmax = echoclients[i].fd;
		    }
		}
	    }
	}
	for (i = 0; i < MAXCLIENTS; i++) {
	    if (chargenclients[i].fd >= 0) {
		FD_SET(chargenclients[i].fd, &wfdset);
		if (chargenclients[i].fd > fdsetmax) {
		    fdsetmax = chargenclients[i].fd;
		}
	    }
	}
    }
    /* notreached */
    exit(EXIT_FAILURE);
}
