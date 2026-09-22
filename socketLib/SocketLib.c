/* ----------------------------------------------------------------------------
 * SocketLib.c -- Basic library module for IP communication.
 * ----------------------------------------------------------------------------
 *
 * (c) 2009 Nexus Telecom AG, Zuerich, Switzerland.
 *
 * Author : Guido Roelli
 *
 * ----------------------------------------------------------------------------
 * Description
 *
 * ClientConnectTCP -- Connect to a TCP service.
 * ClientConnectTimeoutTCP -- Connect to a TCP service with timeout.
 * ClientConnectUDP -- Connect to a UDP service.
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

/*-----------------------------------------------------------------------------
 * Compiler directives
 *-----------------------------------------------------------------------------
 */

/* ----------------------------------------------------------------------------
 * INCLUDES
 */

#include <stdio.h>
#include <string.h>
#include <stddef.h>

#ifdef WIN32
#include <stdlib.h>
#include <stdarg.h>
#include <windows.h>
#else
#include <unistd.h>
#include <syslog.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#endif
#include <errno.h>
#include <sys/types.h>

#include "SocketLib.h"

/* ----------------------------------------------------------------------------
 * EXTERNALS
 */

/* ----------------------------------------------------------------------------
 * DEFINES
 */

#ifndef	INADDR_NONE
#define	INADDR_NONE	0xffffffff
#endif	/* INADDR_NONE */

#define TEMP_LENGTH	2048
#define SYS_ERROR	1
#define NO_SYS_ERROR	0

#define false           0
#define true            1

/* ----------------------------------------------------------------------------
 * TYPES
 */

/* ----------------------------------------------------------------------------
 * GLOBAL VARIABLES
 */
extern int	ClientLibErrNo;	/* Error number */

/* ----------------------------------------------------------------------------
 * LOCAL VARIABLES
 */

/* u_long	inet_addr(); */

int InitWinSockDLL = false;
u_short	portbase = 0;	/* port base, for non-root servers	*/

/* ----------------------------------------------------------------------------
 * PROCEDURES
 */

/*------------------------------------------------------------------------
 * doClose - implements close 2 with handling of EINTR
 * fd: descriptor to close
 * Return: see close 2
 *------------------------------------------------------------------------
 */
static
int
doClose(int fd)
{
  int status;

#ifdef WIN32
  status = closesocket(fd);
#else
  while (((status = close(fd)) == -1) && (errno == EINTR));
#endif
  return status;

}

/*------------------------------------------------------------------------
 * connectsock - allocate & connect a socket using TCP or UDP
 *------------------------------------------------------------------------
 */
static
int
connectsock(host, service, protocol, timeout)
char	*host;		/* name of host to which connection is desired	*/
char	*service;	/* service associated with the desired port	*/
char	*protocol;	/* name of protocol to use ("tcp" or "udp")	*/
int	timeout;	/* timeout in seconds, timeout = 0 means forever */
{
  struct hostent	*phe;	/* pointer to host information entry	*/
  struct servent	*pse;	/* pointer to service information entry	*/
  struct protoent *ppe;	/* pointer to protocol information entry*/
  struct sockaddr_in sin;	/* an Internet endpoint address		*/
  int	s, type;	/* socket descriptor and socket type	*/
  int one=1;
  int flags;
  int n;
  struct timeval timeVal;
  fd_set	rfds;			/* read file descriptor set	*/
  fd_set	wfds;			/* write file descriptor set	*/
#if !defined(Linux)
  int ret;
  int opterror;
  int optlen; 
  int nread;
  char buf[100];
#endif
  struct servent serventResult;
  struct protoent protoentResult;
  struct hostent hostentResult;
  int h_errnop;
  char buffer[1024];

  memset((char *)&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;

  /* Map service name to port number */
#if defined(WIN32)
  if ( pse = getservbyname(service, protocol) )
#elif defined(Linux)
  getservbyname_r(service, protocol, &serventResult, buffer, 1024, &pse);
  if (pse != NULL)
#else
  if (( pse = getservbyname_r(service, protocol, &serventResult, buffer, 1024) ) != NULL)
#endif
    sin.sin_port = pse->s_port;
  else {
    if ((sin.sin_port = htons((short)atoi(service))) == 0 ) {
      ClientLibErrNo = errno;
      syslog(LOG_ERR, "can't get \"%s\" service entry", service);
      return -1;
    }
  }

  /* Map host name to IP address, allowing for dotted decimal */
#if defined(WIN32)
  if ( phe = gethostbyname(host) )
#elif defined(Linux)
  gethostbyname_r(host, &hostentResult, buffer, 1024, &phe, &h_errnop);
  if (phe != NULL)
#else
  if (( phe = gethostbyname_r(host, &hostentResult, buffer, 1024, &h_errnop) ) != NULL)
#endif
    memcpy((char *)&sin.sin_addr, phe->h_addr, phe->h_length);
  else {
    if ( (sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE ) {
      ClientLibErrNo = errno;
      syslog(LOG_ERR, "can't get \"%s\" host entry", host);
      return -1;
    }
  }
        
  /* Map protocol name to protocol number */
#if defined(WIN32)
  if ( (ppe = getprotobyname(protocol)) == 0) {
#elif defined(Linux)
  getprotobyname_r(protocol, &protoentResult, buffer, 1024, &ppe);
  if (ppe == NULL) {
#else
  if ( (ppe = getprotobyname_r(protocol, &protoentResult, buffer, 1024)) == 0) {
#endif
    ClientLibErrNo = errno;
    syslog(LOG_ERR, "can't get \"%s\" protocol entry", protocol);
    return -1;
  }
        
  /* Use protocol to choose a socket type */
  if (strcmp(protocol, "udp") == 0) {
    type = SOCK_DGRAM;
  }
  else {
    type = SOCK_STREAM;
  }
        
  /* Allocate a socket */
  s = socket(PF_INET, type, ppe->p_proto);
  if (s < 0) {
    ClientLibErrNo = errno;
    syslog(LOG_ERR, "can't create socket");
        	return -1;
  }

  /* Enable immediate restart of daemon */
  setsockopt( s, SOL_SOCKET, SO_REUSEADDR, (char *) &one, sizeof(int));
      
  if (timeout == 0) {
    while (1) {
      /* Connect the socket without timeout */
      if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        if (errno == EINTR) {
          continue;
        }
        else {
          ClientLibErrNo = errno;
          doClose(s);
          syslog(LOG_ERR, "can't connect to %s.%s", host, service);
          return -1;
        }
      }
      else {
        return s;
      }
    }
  }
  else {
    /* Connect the socket with timeout */

#ifdef WIN32
    /* function call with timeout not supported for win nt */
    ClientLibErrNo = EINVAL;
    return -1;
#else
    /* set non blocking fd */
    flags = fcntl(s, F_GETFL, 0);
    fcntl(s, F_SETFL, flags | O_NONBLOCK);

    /* Connect the socket */
    while (((n = connect(s, (struct sockaddr *)&sin, sizeof(sin))) == -1)
	   && (errno == EINTR));
    if (n < 0) {
      if ((errno == EINPROGRESS)||(errno == EALREADY)) {
        /* normal condition, go on with select */
      }
      else {
        ClientLibErrNo = errno;
        doClose(s);
        syslog(LOG_ERR, "can't connect to %s.%s", host, service);
        return -1;
      }
    }
     
    if (n == 0) {
      /* connect completed immediately */    
      fcntl(s, F_SETFL, flags);
      return s;
    }

    while (1) {

      timeVal.tv_sec = timeout;
      timeVal.tv_usec = 0;

      FD_ZERO(&rfds);
      FD_SET(s, &rfds);

      wfds = rfds;

      n = select((s + 1), &rfds, &wfds, NULL,
			(struct timeval *)&timeVal);

      if (n == 0) {
        /* just nothing pending no answer from connect to host */
        ClientLibErrNo = errno;
        doClose(s);
        return -1;
      }

      if (n < 0) {
        if (errno == EINTR) { /* Probably caught a SIGCLD  */
          continue;
        }
        else {
          ClientLibErrNo = errno;
          doClose(s);
          syslog(LOG_ERR, "select error : %d", ClientLibErrNo);
          return -1;        
        }
      }

      if (FD_ISSET(s, &rfds) || FD_ISSET(s, &wfds)) { 

#if !defined(Linux)
        /* check with revc call, len=0 if the connection is really ready */
        while (((nread = recv(s, buf, 0, 0)) == -1) && (errno == EINTR));
        if (nread < 0) {
          optlen = sizeof(opterror);
          ret = getsockopt(s, SOL_SOCKET, SO_ERROR, &opterror, &optlen);
          syslog(LOG_ERR, "getsockopt returns %d", ret);
          ClientLibErrNo = errno;
          doClose(s);
          return -1;
        }
        else {
          ClientLibErrNo = errno;
          fcntl(s, F_SETFL, flags);
          return s;
        }
#else
        fcntl(s, F_SETFL, flags);
        return s;
#endif
      }
      else {
        ClientLibErrNo = errno;
        doClose(s);
        syslog(LOG_ERR, "FD_ISSET --> false !!!");
        return -1;
      }
		
    }
#endif
  }
}

/* ----------------------------------------------------------------------------
 * InitialiseWinSockDLL -- Initialise Windows Socket Library.
 *
 * Arguments:   no
 *
 * Returns:	true = OK, false = NOK      
 *
 * Error Messages:   None.
 *
 */
extern
int
InitialiseWinSockDLL()
{

#ifdef WIN32

  int err;

  /* check the WinSock DLL */
  WORD wVersionRequested;
  
  WSADATA wsaData; /* structure of IPC Networkprogramming */
  
  /* The MAKEWORD macro creates an unsigned 16-bit integer
     by concatenating two given unsinged character values.
     wVersionRequested : The highest version of Wondows Socket
     API support that caller can use. The high order byte 
     specifies the minor version (revision) number; the low
     order byte specifies the major version number */

  if (InitWinSockDLL == false) {

    wVersionRequested = MAKEWORD(2, 0);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
      syslog(LOG_DEBUG, "WSAStartup failed");
      return false;
    }

    /* Confirm that the WinSock DLL supports 2.0.
       Note that if the DLL supports versions greater
       than 2.0 in addition to 2.0, it will return
       2.0 in wVersion since that is the version we
       requested */
     
    if ((LOBYTE(wsaData.wVersion) != 2) ||
        (HIBYTE(wsaData.wVersion) != 0)) {
      syslog(LOG_DEBUG, "wVersion mismatch");
      WSACleanup();
      return false;
    }

    /* The WinSock DLL is acceptable. Proceed */
  
    syslog(LOG_DEBUG, "InitWinSockDLL successful");
    InitWinSockDLL = true;
  
  }

#endif

  return true;

} 

/* ----------------------------------------------------------------------------
 * ClientConnectTCP -- Connect to a TCP service.
 *
 * Arguments:      host -- name of host to which connection is desired.
 *                 service -- service associated with the desired port
 *
 * Returns:        socket, -1 = error in ClientLibErrNo
 *
 * Error Messages:   None.
 *
 */
extern
int
ClientConnectTCP(const char *host, const char *service)
{

#ifdef WIN32
  InitialiseWinSockDLL();
#endif

  return connectsock(host, service, "tcp", 0);

} /* End of ClientConnectTCP */

/* ----------------------------------------------------------------------------
 * ClientConnectTimeoutTCP -- Connect to a TCP service with timeout.
 *
 * Arguments:      host -- name of host to which connection is desired.
 *                 service -- service associated with the desired port
 *                 timeout -- timeout in seconds
 *
 * Returns:        socket, -1 = error in ClientLibErrNo
 *
 * Error Messages:   None.
 *
 */
extern
int
ClientConnectTimeoutTCP(const char *host, const char *service, int timeout)
{

#ifdef WIN32
  InitialiseWinSockDLL();
#endif

  return connectsock(host, service, "tcp", timeout);

} /* End of ClientConnectTCP */

/* ----------------------------------------------------------------------------
 * ClientConnectUDP -- Connect to a UDP service.
 *
 * Arguments:      host -- name of host to which connection is desired.
 *                 service -- service associated with the desired port
 *
 * Returns:        socket, -1 = error in ClientLibErrNo
 *
 * Error Messages:   None.
 *
 */
extern
int
ClientConnectUDP(const char *host, const char *service)
{

#ifdef WIN32
  InitialiseWinSockDLL();
#endif
  
  return connectsock(host, service, "udp", 0);

} /* End of ClientConnectUDP */

/*------------------------------------------------------------------------
 * passivesock - allocate & bind a server socket using TCP or UDP
 *------------------------------------------------------------------------
 */
static
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
static
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
static
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
extern
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
extern
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
extern
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
extern
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
extern
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
extern
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
extern
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

/* End of SocketLib.c */
