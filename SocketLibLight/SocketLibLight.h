/* ----------------------------------------------------------------------------
 * SocketLib.h -- Basic library module for IP communication.
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

#ifndef __SocketLib_h__

#define __SocketLib_h__

/* ----------------------------------------------------------------------------
 * INCLUDES
 */

/* ----------------------------------------------------------------------------
 * DEFINES
 */

/* ----------------------------------------------------------------------------
 * TYPES
 */

/* ----------------------------------------------------------------------------
 * GLOBAL VARIABLES
 */
int	ClientLibErrNo;	/* Error number */
int	ServerLibErrNo;	/* Error number */

/* ----------------------------------------------------------------------------
 * PROCEDURES
 */

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
InitialiseWinSockDLL(void);

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
ClientConnectTCP(const char *host, const char *service);

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
ClientConnectTimeoutTCP(const char *host, const char *service, int timeout);

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
ClientConnectUDP(const char *host, const char *service);

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
ServerOfferTCP(const char *service);

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
ServerOfferTCPQlen(const char *service, int qlen);

/* ----------------------------------------------------------------------------
 * GetServerPort -- Get a service port.
 *
 * Arguments:      sock -- socket
 *
 * Returns:        0 OK, -1 returns errno
 *
 * Error Messages:   None.
 *
 */
extern
int
GetServerPort(int sock);

/* ----------------------------------------------------------------------------
 * ServerOfferSystemTCP -- Offer a TCP service choose by the system.
 *
 * Arguments:      service -- service port will be set by the system
 *
 * Returns:        0 OK, -1 returns errno
 *
 * Error Messages:   None.
 *
 */
extern
int
ServerOfferSystemTCP(void);

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
ServerOfferUDP(const char *service);

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
ServerSelect(int sock);

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
ServerSelectTimeout(int sock, int timeoutSeconds, int timeoutMicroSeconds);

#endif /* __SocketLib_h__  */
