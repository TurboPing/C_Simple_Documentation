/* ----------------------------------------------------------------------------
 * ClientTest.c -- Test program for IP client communication.
 * ----------------------------------------------------------------------------
 *
 * (c) 1997 STS AG, Zuerich, Switzerland.
 *
 * ----------------------------------------------------------------------------
 * Description
 *
 * Test the ClientLib.
 *
 * ----------------------------------------------------------------------------
 */

/*-----------------------------------------------------------------------------
 * Compiler directives
 *-----------------------------------------------------------------------------
 */

#ifdef WIN32
#pragma warning (disable : 4101) /* unreferenced local variable */
#endif

#include <errno.h>
#include <sys/types.h>

#ifdef WIN32
#else
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>
#endif

#include <stdio.h>
#include <signal.h>

#include <machine.h>
#include <ClientLib.h>

#define	LINELEN		128

char    buf[1000];
int	s, n;			/* socket descriptor, read count*/
int	outchars, inchars;	/* characters sent and received	*/

char	host[100];	/* host to use if none supplied	*/
char	service[100];	/* default service name		*/
char	message[100];
int	timeout;

/*------------------------------------------------------------------------
 * TCPwrite - send info to a TCP socket
 *------------------------------------------------------------------------
 */
int
TCPwrite(int sock, char *buf, int outchars)
{
	printf("write:%s\n", buf);
	send(sock, buf, outchars, 0);
        return 0;
}

/*------------------------------------------------------------------------
 * TCPMainClient - Connect to a TCP service
 *------------------------------------------------------------------------
 */
int
TCPMainClient(char *host, char *service, char *message, int timeout) {

	int ret;
	int	sock;			/* master server socket		*/
	fd_set	rfds;			/* read file descriptor set	*/
	fd_set	afds;			/* active file descriptor set	*/
	int	nfds;
	char	buf[LINELEN+1];		/* buffer for one line of text	*/
	int     n;

	printf("\nConnect to %s/%s/%d\n", host, service, timeout);

	if (timeout == 0) {
	  sock = ClientConnectTCP(host, service);
	  if (sock == -1) {
	    printf("Connect failed : error : %d\n", ClientLibErrNo);
	    return -1;
	  } 
        }
	else {
	  sock = ClientConnectTimeoutTCP(host, service, timeout);
	  if (sock == -1) {
	    printf("Connect failed : error : %d\n", ClientLibErrNo);
	    return -1;
	  } 	
	}

	sprintf(buf, "%s", message);
        outchars = strlen(buf);

	nfds = sock + 1;

	FD_ZERO(&afds);
	FD_SET(sock, &afds);

	TCPwrite(sock, buf, outchars);

	memcpy((char *)&rfds, (char *)&afds, sizeof(rfds));

	if ((ret = select(nfds, &rfds, (fd_set *)0, (fd_set *)0,
			  (struct timeval *)0)) < 0) {

		if ((ret == -1) && (errno == EINTR)) {
			/* interrupted system call --> continue */
			printf("Interrupted system call...\n");
		}
		else if (ret == -1) {
			printf("select(): %s\n", strerror(errno));
		}
	}
		
	if (ret == 0) {
		printf("Nothing pending (select).\n");
	}

	if (FD_ISSET(sock, &rfds)) {

		printf("Reading...\n");

		if ((n = recv(sock, buf, 1000, 0)) < 0) {
			if ((ret == -1) && (errno == EINTR)) {
				/* interrupted system call --> continue */
				printf("Interrupted system call...\n");
			}
			else if (ret == -1) {
				printf("Read error: %s\n", strerror(errno));
			}
		}
			
		if (n == 0) {
			printf("Nothing pending (read).\n");
		}

		else {			
			buf[n] = '\0';	/* insure line null-terminated	*/
			printf("read:%s\n", buf);
		}
	}
	else {
		printf("FD_ISSET --> false !!!\n");
	}

	close(sock);

	printf("Connect done\n");
	return 0;
}

/* ----------------------------------------------------------------------------
 * Usage -- print call synopsis to stdout.
 */
static
void
Usage()
{

  printf(
    "Usage:\tClientTest [options]\n"
    "  -h\tPrints this message.\n"
    "  -n\t<hostname>\t\thostname or ip address. Default=localhost\n"
    "  -s\t<service>\t\tservice. Default=echo\n"
    "  -m\t<message>\t\tmessage. Default=message\n"
    "  -t\t<timeout>\t\ttimeout value. Default=0\n"
    "\n"
  );

  printf(
    "Description:\n"
    "This program tests the ClientLib for tcp/udp client connections.\n"
    "\n"    
  );
  printf(
    "Example:\n"
    "ClientTest -n localhost -s echo -m message -t 5\n"
    "\n"
  ); 

} /* End of usage */

/* ----------------------------------------------------------------------------
 * ParseArgs --  Parse args and setup defaults.
 * 
 * Description
 * 
 * interactions for query-arguments are held over stdout and stderr.
 *
 * The features:
 *  - parse the cmdline-args,
 *  - set defaults for unspecified cmdline-options,
 *
 * Call-synopsis
 * Input:	argc, argv : arguments of the command line.
 * 
 * Output:	...
 * 
 * Return:	...
 * 
 * 
 * Algorithm: ...
 * 
 * Error Conditions: ...
 * 
 * Known Problems: ...
 * 
 */
static
void
ParseArgs(int argc, char **argv)
{
  int c;

  /* set default */
  sprintf(host, "%s", "localhost");
  sprintf(service, "%s", "echo");
  sprintf(message, "%s", "message");
  timeout = 0;
    
  /* Get cmdline-options & params */

  while ((c = getopt(argc, argv, "hvn:s:m:t:")) != -1) {
    switch (c) {
	case 'h':
	    Usage();
	    exit(0);
	case 'n':
	    sprintf(host, "%s", optarg);
	    break;
	case 's':
	    sprintf(service, "%s", optarg);
	    break;
	case 'm':
	    sprintf(message, "%s", optarg);
	    break;
	case 't':
	    timeout = atoi(optarg);
	    break;
	default : printf("nothing");
	    break;
    }
  }

} /* End of ParseArgs */

/* ----------------------------------------------------------------------------
 * SigHandler -- Catch all Signals.
 *
 * Arguments:      None.
 *
 * Returns:        Nothing.
 *
 * Side Effects:   None.
 *
 * Algorithm:      Nothing crazy.
 *
 */
#ifndef WIN32
static
void
SigHandler(int signo)
{
    signo = signo;
} /* End of SigHandler */

/* ----------------------------------------------------------------------------
 * SignalFunction -- Reliable version of signal(), using POSIX sigaction().
 *
 * Arguments:       signo    signal to catch.
 *                  func     Handler to install.
 *
 * Returns:         Previous signal disposition.
 *
 * Algorithm:       POSIX.1 compliant replacement for signal-function.
 *                  This is to be used when we want to use SIGALRM to timeout
 *                  I/O operations.
 *                  This should work for 4.3+BSD, SVR4 and  SunOs 4.1.3
 *                  (Stevens,1992, section 10.14
 *                   Advanced Programming in the UNIX Environment)
 */

static
void *
SignalFunction(int signo, void *func)
{
	struct sigaction	act, oact;

	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (signo == SIGALRM) {
#ifdef	SA_INTERRUPT
		act.sa_flags |= SA_INTERRUPT;	/* SunOS */
#endif
	} else {
#ifdef	SA_RESTART
		act.sa_flags |= SA_RESTART;		/* SVR4, 44BSD */
#endif
	}
	if (sigaction(signo, &act, &oact) < 0)
		return(SIG_ERR);
	return(oact.sa_handler);

} /* End of SignalFunction */
#endif

/*------------------------------------------------------------------------
 * main - TCP client test program
 *------------------------------------------------------------------------
 */
int
main(argc, argv)
int	argc;
char	*argv[];
{
	int i;

	printf("Test Programs Start\n");

	ParseArgs(argc, argv);

#ifndef WIN32
	/* Handle signals */
  	for(i = 1; i < NSIG; i++) {
    	  SignalFunction(i, SigHandler);
  	}
#endif
	/* test case echo service */
	printf("ClientTest host:%s/service:%s/message:%s\n",
			host, service, message);
	TCPMainClient(host, service, message, timeout);

	/* test case echo service */
	sprintf(host, "%s", "localhost");
	sprintf(service, "%s", "7");
	printf("ClientTest host:%s/service:%s/message:%s\n",
			host, service, message);
	TCPMainClient(host, service, message, timeout);

	/* test case unknown service */
	sprintf(host, "%s", "localhost");
	sprintf(service, "%s", "unknown");
	printf("ClientTest host:%s/service:%s/message:%s\n",
			host, service, message);
	TCPMainClient(host, service, message, timeout);

	/* test case 9999 service */
	sprintf(host, "%s", "localhost");
	sprintf(service, "%s", "9999");
	printf("ClientTest host:%s/service:%s/message:%s\n",
			host, service, message);
	TCPMainClient(host, service, message, timeout);

	printf("Test Programs Done\n");

	return 0;
}

