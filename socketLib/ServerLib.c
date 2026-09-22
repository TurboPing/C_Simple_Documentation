/* ----------------------------------------------------------------------------
 * ServerLib.c -- Basic library module for IP server communication.
 * ----------------------------------------------------------------------------
 *
 * (c) 1998 STS AG, Zuerich, Switzerland.
 *
 * ----------------------------------------------------------------------------
 * Description
 *
 * ServerOfferTCP -- Offer a TCP service.
 * ServerOfferTCPQlen -- Offer a TCP service with a queue length.
 * GetServerPort - Get a service port.
 * ServerOfferSystemTCP - Offer a TCP service choose by the system.
 * ServerOfferUDP -- Offer a UDP service.
 * ServerSelect -- Check on server socket descriptor.
 * ServerSelectTimeout -- Check on server socket descriptor with timeout.
 *
 * ----------------------------------------------------------------------------
 */

#define COMPILE_SERVERLIB

/* ----------------------------------------------------------------------------
 * INCLUDES
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#ifdef WIN32
#else
#include <syslog.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/file.h> 
#endif
#include <errno.h>
#include <sys/types.h>

#include <machine.h>
#include <a8620types.h>
#include <a8620assert.h>

#include <ClientLib.h>
#include <ServerLib.h>

/* ----------------------------------------------------------------------------
 * DEFINES
 */

#define TEMP_LENGTH	2048
#define SYS_ERROR	1
#define NO_SYS_ERROR	0

/* ----------------------------------------------------------------------------
 * TYPES
 */

/* ----------------------------------------------------------------------------
 * EXTERNALS
 */

/* ----------------------------------------------------------------------------
 * GLOBAL VARIABLES
 */
Public int	ServerLibErrNo;	/* Error number */

/* ----------------------------------------------------------------------------
 * LOCAL VARIABLES
 */

u_short	portbase = 0;	/* port base, for non-root servers	*/

/* ----------------------------------------------------------------------------
 * PROCEDURES
 */

/*------------------------------------------------------------------------
 * passivesock - allocate & bind a server socket using TCP or UDP
 *------------------------------------------------------------------------
 */
Private
int
passivesock( service, protocol, qlen )
char	*service;	/* service associated with the desired port	*/
char	*protocol;	/* name of protocol to use ("tcp" or "udp")	*/
int	qlen;		/* maximum length of the server request queue	*/
{
	struct servent	*pse;	/* pointer to service information entry	*/
	struct protoent *ppe;	/* pointer to protocol information entry*/
	struct sockaddr_in sin;	/* an Internet endpoint address		*/

	int	s, type;	/* socket descriptor and socket type	*/
	int one=1;

	memset((char *)&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;

    /* Map service name to port number */
        if ( (pse = getservbyname(service, protocol)) )
		sin.sin_port = ((u_short)pse->s_port
			+ portbase);
        else {
            if ((sin.sin_port = htons((short)atoi(service))) == 0 ) {
		ServerLibErrNo = errno;
		syslog(LOG_ERR, "can't get \"%s\" service entry\n", service);
		return -1;
            }
        }
        
    /* Map protocol name to protocol number */
	if ( (ppe = getprotobyname(protocol)) == 0) {
		ServerLibErrNo = errno;
		syslog(LOG_ERR, "can't get \"%s\" protocol entry\n", protocol);
		return -1;
        }
        
    /* Use protocol to choose a socket type */
	if (strcmp(protocol, "udp") == 0)
		type = SOCK_DGRAM;
	else
		type = SOCK_STREAM;

    /* Allocate a socket */
	s = socket(PF_INET, type, ppe->p_proto);
	if (s < 0) {
		ServerLibErrNo = errno;
		syslog(LOG_ERR, "can't create socket");
		return -1;
        }
        
    /* Enable immediate restart of daemon */
    setsockopt( s, SOL_SOCKET, SO_REUSEADDR, (char *) &one, sizeof(int));

    /* Bind the socket */
	if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
		ServerLibErrNo = errno;
		syslog(LOG_ERR, "can't bind to %s port", service);
		return -1;
	}		
	if (type == SOCK_STREAM && listen(s, qlen) < 0) {
		ServerLibErrNo = errno;
		syslog(LOG_ERR, "can't listen on %s port", service);
		return -1;
	}
	return s;
}

/*------------------------------------------------------------------------
 * passiveTCP - create a passive socket for use in a TCP server
 *------------------------------------------------------------------------
 */
int
passiveTCP( service, qlen )
char	*service;	/* service associated with the desired port	*/
int	qlen;		/* maximum server request queue length		*/
{
	return passivesock(service, "tcp", qlen);
}

/*------------------------------------------------------------------------
 * passiveUDP - create a passive socket for use in a UDP server
 *------------------------------------------------------------------------
 */
int
passiveUDP( service )
char	*service;	/* service associated with the desired port	*/
{
	return passivesock(service, "udp", 0);
}

/* ----------------------------------------------------------------------------
 * ServerOfferTCP -- Offer a TCP service.
 *
 * Arguments:      service -- service associated with the desired port
 *
 * Returns:        0 OK, -1 returns errno
 *
 * Error Messages:   None.
 *
 */
Public
int
ServerOfferTCP(const char *service)
{

#ifdef WIN32
  InitialiseWinSockDLL();
#endif

  return passiveTCP(service, 5);

} /* End of ServerOfferTCP */

/* ----------------------------------------------------------------------------
 * ServerOfferTCPQlen -- Offer a TCP service with a queue length.
 *
 * Arguments:      service -- service associated with the desired port
 *                 qlen    -- queue length
 *
 * Returns:        0 OK, -1 returns errno
 *
 * Error Messages:   None.
 *
 */
Public
int
ServerOfferTCPQlen(const char *service, int qlen)
{

#ifdef WIN32
  InitialiseWinSockDLL();
#endif

  return passiveTCP(service, qlen);

} /* End of ServerOfferTCPQlen */

/* ----------------------------------------------------------------------------
 * GetServerPort -- Get a service port.
 *
 * Arguments:      socket -- socket
 *
 * Returns:        0 OK, -1 returns errno
 *
 * Error Messages:   None.
 *
 */
Public
int
GetServerPort(int socket)
{
	struct sockaddr_in sock_addr;
	unsigned int sock_addr_len;

        sock_addr_len = sizeof(sock_addr);
        getsockname(socket, (struct sockaddr *)&sock_addr, &sock_addr_len);

	return ntohs(sock_addr.sin_port);

}

/* ----------------------------------------------------------------------------
 * ServerOfferSystemTCP -- Offer a TCP service choose by the system.
 *
 * Arguments:      
 *
 * Returns:        0 OK, -1 returns errno
 *
 * Error Messages:   None.
 *
 */
Public
int
ServerOfferSystemTCP(void)
{

	char *protocol;
	int qlen;

	struct protoent *ppe;	/* pointer to protocol information entry*/
	struct sockaddr_in sin;	/* an Internet endpoint address		*/

	int	s, type;	/* socket descriptor and socket type	*/
	int one=1;

#ifdef WIN32
  InitialiseWinSockDLL();
#endif

        protocol = "tcp";
	qlen = 5;

	memset((char *)&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	/* port choosen by the system */
	sin.sin_port = htons(0);  
	   
    /* Map protocol name to protocol number */
	if ( (ppe = getprotobyname(protocol)) == 0) {
		ServerLibErrNo = errno;
		syslog(LOG_ERR, "can't get \"%s\" protocol entry\n", protocol);
		return -1;
        }
        
    /* Use protocol to choose a socket type */
	if (strcmp(protocol, "udp") == 0)
		type = SOCK_DGRAM;
	else
		type = SOCK_STREAM;

    /* Allocate a socket */
	s = socket(PF_INET, type, ppe->p_proto);
	if (s < 0) {
		ServerLibErrNo = errno;
		syslog(LOG_ERR, "can't create socket");
		return -1;
        }
        
    /* Enable immediate restart of daemon */
    setsockopt( s, SOL_SOCKET, SO_REUSEADDR, (char *) &one, sizeof(int));

    /* Bind the socket */
	if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
		ServerLibErrNo = errno;
		syslog(LOG_ERR, "can't bind to port");
		return -1;
	}		

	if (type == SOCK_STREAM && listen(s, qlen) < 0) {
		ServerLibErrNo = errno;
		syslog(LOG_ERR, "can't listen on port");
		return -1;
	}

	return s;

} /* End of ServerOfferSystemTCP */

/* ----------------------------------------------------------------------------
 * ServerOfferUDP -- Offer a UDP service.
 *
 * Arguments:      service -- service associated with the desired port
 *
 * Returns:        0 OK, -1 returns errno
 *
 * Error Messages:   None.
 *
 */
Public
int
ServerOfferUDP(const char *service)
{

#ifdef WIN32
  InitialiseWinSockDLL();
#endif

  return passiveUDP(service);

} /* End of ServerConnectUDP */

/* ----------------------------------------------------------------------------
 * ServerSelect -- Check on server socket descriptor.
 *
 * Arguments:      sock -- server socket to wait for
 *
 * Returns:        accepted ssock or -1 if fails
 *
 * Error Messages:   None.
 *
 */
Public
int
ServerSelect(int sock)
{

  int ret;
  struct sockaddr_in fsin;	/* the from address of a client	*/
  fd_set	rfds;			/* read file descriptor set	*/
  fd_set	afds;			/* active file descriptor set	*/
  unsigned int	alen;			/* from-address length		*/
  int	nfds;
  int     ssock;

  while (1) {
    nfds = sock + 1;
    FD_ZERO(&afds);
    FD_SET(sock, &afds);

    memcpy((char *)&rfds, (char *)&afds, sizeof(rfds));

    if ((ret = select(nfds, &rfds, (fd_set *)0, (fd_set *)0,
			(struct timeval *)0) < 0)) {

      if ((ret == -1) && (errno == EINTR)) {
        /* interrupted system call --> continue */
	syslog(LOG_DEBUG, "Interrupted system call...");
	continue;
      }
      else if (ret == -1) {
	syslog(LOG_ERR, "Select error|errno:%d", errno);
        return -1;
      }
    }
		
    if (ret == 0) {
      syslog(LOG_DEBUG, "Nothing pending (select).");
    }

    if (FD_ISSET(sock, &rfds)) {

      alen = sizeof(fsin);
      ssock = accept(sock, (struct sockaddr *)&fsin, &alen);
      if (ssock < 0) {
        if (errno == EINTR) { /* Probably caught a SIGCLD  */
	  syslog(LOG_DEBUG, "errno = EINTR");
	  continue;
        }
      }
      else {
        return ssock;
      }
    }
    else {
      syslog(LOG_ERR, "FD_ISSET --> false !!!");
      return -1;
    }

}

} /* End of ServerSelect */

/* ----------------------------------------------------------------------------
 * ServerSelectTimeout -- Check on server socket descriptor with timeout.
 *
 * Arguments:      sock                -- server socket to wait for
 *                 timeoutSeconds      -- timeout in seconds
 *                 timeoutMicroSeconds -- timeout in micro seconds
 *
 *                 timeoutSeconds, timeoutMicroSeconds = 0 -> wait forever
 *
 * Returns:        accepted ssock or -1 if fails, 0 means timeout
 *
 * Error Messages:   None.
 *
 */
Public
int
ServerSelectTimeout(int sock, int timeoutSeconds, int timeoutMicroSeconds)
{

  int ret;
  struct sockaddr_in fsin;	/* the from address of a client	*/
  fd_set	rfds;			/* read file descriptor set	*/
  fd_set	afds;			/* active file descriptor set	*/
  unsigned int	alen;			/* from-address length		*/
  int	nfds;
  int     ssock;
  struct timeval timeout;

  while (1) {

    timeout.tv_sec = timeoutSeconds;
    timeout.tv_usec = timeoutMicroSeconds;

    nfds = sock + 1;
    FD_ZERO(&afds);
    FD_SET(sock, &afds);

    memcpy((char *)&rfds, (char *)&afds, sizeof(rfds));

    ret = select(nfds, &rfds, (fd_set *)0, (fd_set *)0,
			(struct timeval *)&timeout);

    if (ret < 0) {

      if ((ret == -1) && (errno == EINTR)) {
        /* interrupted system call --> continue */
	syslog(LOG_DEBUG, "Interrupted system call...");
	continue;
      }
      else if (ret == -1) {
	syslog(LOG_ERR, "Select error|errno:%d", errno);
        return -1;
      }
    }
		
    if (ret == 0) {
      /* just nothing pending */
      return 0;
    }

    if (FD_ISSET(sock, &rfds)) {

      alen = sizeof(fsin);
      while (1) {
        ssock = accept(sock, (struct sockaddr *)&fsin, &alen);
        if (ssock < 0) {
          if (errno == EINTR) { /* Probably caught a SIGCLD  */
	    syslog(LOG_ERR, "errno = EINTR");
	    continue;
          }
        }
        else {
          return ssock;
        }
      }
    
    }
    else {
      syslog(LOG_ERR, "FD_ISSET --> false !!!");
      return -1;
    }

}

} /* End of ServerSelectTimeout */

/* End of ServerLib.c */
