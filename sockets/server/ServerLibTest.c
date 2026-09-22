/* ----------------------------------------------------------------------------
 * ServerTest.c - Test program for IP server communication library.
 * ----------------------------------------------------------------------------
 *
 * (c) 1998 STS AG, Zuerich, Switzerland.
 *
 * ----------------------------------------------------------------------------
 * Description
 *
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef WIN32
#else
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>
#include <syslog.h>
#endif
#include <errno.h>
#include <sys/types.h>

#include "ClientLib.h"
#include "ServerLib.h"

#define	LINELEN		128

char    buf[1000];
int	s, n;			/* socket descriptor, read count*/
int	outchars, inchars;	/* characters sent and received	*/

/*------------------------------------------------------------------------
 * TCPwrite
 *------------------------------------------------------------------------
 */
static
int
TCPwrite(int sock, char *buf, int outchars)
{
	printf("send:%s\n", buf);
	(void) send(sock, buf, outchars, 0);
	return 0;
}

/*------------------------------------------------------------------------
 * TCPread
 *------------------------------------------------------------------------
 */
static
int
TCPread(int ssock)
{
  char	buf[500];		/* buffer for one line of text	*/
  int     n;
 
  if ((n = recv(ssock, buf, 1000, 0)) < 0) {
    if ((n == -1) && (errno == EINTR)) {
      /* interrupted system call --> continue */
      printf("Interrupted system call...\n");
    }
    else if (n == -1) {
      printf("Read error\n");
    }
  }

  if (n == 0) {
    printf("Nothing pending (recv).\n");
  }

  else {			
    buf[n] = '\0';	/* insure line null-terminated	*/
    printf("read:%s\n", buf);
  }

  return 0;  
}

/*------------------------------------------------------------------------
 * TCPMainServer
 *------------------------------------------------------------------------
 */
int
TCPMainServer(char *hostname, char *service, char *message) {

	int ret;
	int	sock;			/* master server socket		*/
        int clientFd;
	char	sendBuf[LINELEN+1];		/* buffer for one line of text	*/
	int     ssock;

	syslog(LOG_INFO, "Offer service on host : %s / service : %s", hostname, service);
	printf("Offer service on host : %s / service : %s\n", hostname, service);
	sock = ServerOfferTCP(service);
	if (sock == -1) {
		syslog(LOG_INFO, "error create socket : %d", ServerLibErrNo);
		return -1;
	}
	syslog(LOG_INFO, "Offered Socket is %d", sock);
	printf("Offered Socket is %d\n", sock);

        syslog(LOG_INFO, "Connect to service : %s", service);
        printf("Connect to service : %s\n", service);
       	clientFd = ClientConnectTCP(hostname, service);
	if (clientFd == -1) {
	
          printf("Unable to open connection\n");
	
	}
        else {
          syslog(LOG_INFO, "Connected Socket is : %d", clientFd);
          printf("Connected Socket is : %d\n", clientFd);

  	  sprintf(sendBuf, "%s", message);
       	  outchars = strlen(sendBuf);

  	  syslog(LOG_INFO, "send message : %s", sendBuf);
	
  	  TCPwrite(clientFd, sendBuf, outchars);

	  ssock = ServerSelect(sock);

	  TCPread(ssock);

	  TCPwrite(ssock, sendBuf, outchars);

	  TCPread(clientFd);
	
	  ret = close(ssock);
	  syslog(LOG_INFO, "close ssock returns : %d", ret);
	  ret = close(sock);
	  syslog(LOG_INFO, "close sock returns : %d", ret);
	  ret = close(clientFd);
	  syslog(LOG_INFO, "close clientFd returns : %d", ret);
        }
	return 0;
}

/*------------------------------------------------------------------------
 * TCPMainServerTimeout
 *------------------------------------------------------------------------
 */
int
TCPMainServerTimeout(char *hostname, char *service, char *message) {

	int ret;
	int	sock;			/* master server socket		*/
        int clientFd;
	char	sendBuf[LINELEN+1];		/* buffer for one line of text	*/
	int     ssock;

	syslog(LOG_INFO, "Offer service on host : %s / service : %s", hostname, service);
	printf("Offer service on host : %s / service : %s\n", hostname, service);
	sock = ServerOfferTCP(service);
	if (sock == -1) {
		syslog(LOG_INFO, "error create socket : %d", ServerLibErrNo);
		return -1;
	}
	syslog(LOG_INFO, "Offered Socket is %d", sock);
	printf("Offered Socket is %d\n", sock);

        syslog(LOG_INFO, "Connect to service : %s", service);
        printf("Connect to service : %s\n", service);
       	clientFd = ClientConnectTCP(hostname, service);
	if (clientFd == -1) {
	
          printf("Unable to open connection\n");
	
	}
        else {
          syslog(LOG_INFO, "Connected Socket is : %d", clientFd);
          printf("Connected Socket is : %d\n", clientFd);

  	  sprintf(sendBuf, "%s", message);
       	  outchars = strlen(sendBuf);

  	  syslog(LOG_INFO, "send message : %s", sendBuf);
	
  	  TCPwrite(clientFd, sendBuf, outchars);

	  ssock = ServerSelectTimeout(sock, 1, 0);

	  TCPread(ssock);

	  TCPwrite(ssock, sendBuf, outchars);

	  TCPread(clientFd);
	
	  ret = close(ssock);
	  syslog(LOG_INFO, "close ssock returns : %d", ret);
	  ret = close(sock);
	  syslog(LOG_INFO, "close sock returns : %d", ret);
	  ret = close(clientFd);
	  syslog(LOG_INFO, "close clientFd returns : %d", ret);
        }
	return 0;
}

/*------------------------------------------------------------------------
 * TCPMainServerQlen
 *------------------------------------------------------------------------
 */
int
TCPMainServerQlen(char *hostname, char *service, char *message, int qlen) {

	int ret;
	int	sock;			/* master server socket		*/
        int clientFd;
	char	sendBuf[LINELEN+1];		/* buffer for one line of text	*/
	int     ssock;

	syslog(LOG_INFO, "Offer service on host : %s / service : %s / qlen : %d", 
			hostname, service, qlen);
	printf("Offer service on host : %s / service : %s / qlen : %d\n", 
			hostname, service, qlen);
	sock = ServerOfferTCPQlen(service, qlen);
	if (sock == -1) {
		syslog(LOG_INFO, "error create socket : %d", ServerLibErrNo);
		return -1;
	}
	syslog(LOG_INFO, "Offered Socket is %d", sock);
	printf("Offered Socket is %d\n", sock);

        syslog(LOG_INFO, "Connect to service : %s", service);
        printf("Connect to service : %s\n", service);
       	clientFd = ClientConnectTCP(hostname, service);
	if (clientFd == -1) {
	
          printf("Unable to open connection\n");
	
	}
        else {
          syslog(LOG_INFO, "Connected Socket is : %d", clientFd);
          printf("Connected Socket is : %d\n", clientFd);

  	  sprintf(sendBuf, "%s", message);
       	  outchars = strlen(sendBuf);

  	  syslog(LOG_INFO, "send message : %s", sendBuf);
	
  	  TCPwrite(clientFd, sendBuf, outchars);

	  ssock = ServerSelect(sock);

	  TCPread(ssock);

	  TCPwrite(ssock, sendBuf, outchars);

	  TCPread(clientFd);
	
	  ret = close(sock);
	  syslog(LOG_INFO, "close sock returns : %d", ret);
	  ret = close(ssock);
	  syslog(LOG_INFO, "close ssock returns : %d", ret);
	  ret = close(clientFd);
	  syslog(LOG_INFO, "close clientFd returns : %d", ret);
        }
	return 0;
}

/*------------------------------------------------------------------------
 * TCPMainServerSystem
 *------------------------------------------------------------------------
 */
int
TCPMainServerSystem(char *hostname, char *service __attribute__ ((unused)),
		    char *message)
{

	int ret;
	int	sock;			/* master server socket		*/
	int port;
	char portString[30];
        int clientFd;
	char	sendBuf[LINELEN+1];		/* buffer for one line of text	*/
	int     ssock;

	syslog(LOG_INFO, "Offer service on host : %s", hostname);
	printf("Offer service on host : %s\n", hostname);
	sock = ServerOfferSystemTCP();
	if (sock == -1) {
		syslog(LOG_INFO, "error create socket : %d", ServerLibErrNo);
		return -1;
	}
	port = GetServerPort(sock);
	sprintf(portString, "%d", port);
	 
	syslog(LOG_INFO, "Offered Socket is %d|port:%d", sock, port);
	printf("Offered Socket is %d|port:%d\n", sock, port);

        syslog(LOG_INFO, "Connect to service : %s", portString);
        printf("Connect to service : %s\n", portString);
       	clientFd = ClientConnectTCP(hostname, portString);
	if (clientFd == -1) {
	
          printf("Unable to open connection\n");
	
	}
        else {
          syslog(LOG_INFO, "Connected Socket is : %d", clientFd);
          printf("Connected Socket is : %d\n", clientFd);

   	  sprintf(sendBuf, "%s", message);
       	  outchars = strlen(sendBuf);

  	  syslog(LOG_INFO, "send message : %s", sendBuf);

	  TCPwrite(clientFd, sendBuf, outchars);

	  ssock = ServerSelect(sock);

	  TCPread(ssock);
  
  	  TCPwrite(ssock, sendBuf, outchars);

	  TCPread(clientFd);

	  ret = close(sock);
	  syslog(LOG_INFO, "close sock returns : %d", ret);
	  ret = close(ssock);
  	  syslog(LOG_INFO, "close ssock returns : %d", ret);
	  ret = close(clientFd);
	  syslog(LOG_INFO, "close clientFd returns : %d", ret);
	}
  	 
  	return 0;
	
}

/*------------------------------------------------------------------------
 * main
 *------------------------------------------------------------------------
 */
int
main(int argc __attribute__ ((unused)), char **argv __attribute__ ((unused)))
{
	char	hostname[100];	/* host to use if none supplied	*/
	char	service[100];	/* default service name		*/
	char	message[100];   /* message to send              */
	int     qlen;		/* queue length                 */

	openlog("ServerTest", LOG_PID | LOG_NDELAY | LOG_NOWAIT, LOG_USER);
	syslog(LOG_INFO, "Test Programs Start");
	printf("Test Programs Start\n");

	/* create socket with given port 4444 */
	sprintf(hostname, "%s", "localhost");
	sprintf(service, "%s", "4444");
	sprintf(message, "%s", "this is a test message");
	syslog(LOG_INFO, "ServerTest service:%s/message:%s", service, message);
	TCPMainServer(hostname, service, message);

	printf("\n");

	/* create socket with given port 4444 */
	sprintf(hostname, "%s", "localhost");
	sprintf(service, "%s", "4444");
	sprintf(message, "%s", "this is a test message");
	syslog(LOG_INFO, "ServerTest service:%s/message:%s", service, message);
	TCPMainServerTimeout(hostname, service, message);

	printf("\n");

	/* create socket with given port 4444 */
	sprintf(hostname, "%s", "localhost");
	sprintf(service, "%s", "4444");
	sprintf(message, "%s", "this is a test message");
	qlen = 10;
	syslog(LOG_INFO, "ServerTest service:%s/message:%s/qlen:%d", 
			service, message, qlen);
	TCPMainServerQlen(hostname, service, message, qlen);

	printf("\n");
	
	/* create socket on system port */
	sprintf(hostname, "%s", "localhost");
	sprintf(service, "%s", "0");
	sprintf(message, "%s", "this is a test message");
	syslog(LOG_INFO, "ServerTest service:%s/message:%s", service, message);
	TCPMainServerSystem(hostname, service, message);

	syslog(LOG_INFO, "Test Programs End");
	printf("Test Programs End\n");

	return 0;
}

