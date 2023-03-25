/* ----------------------------------------------------------------------------
 * ServerLib.h -- Basic library module for IP server communication.
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

#ifndef __ServerLib_h__

#define __ServerLib_h__

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
int	ServerLibErrNo;	/* Error number */

/* ----------------------------------------------------------------------------
 * PROCEDURES
 */

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

#endif /* __ServerLib_h__  */
