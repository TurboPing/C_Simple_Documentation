/* ----------------------------------------------------------------------------
 * ClientLib.h -- Basic library module for IP client communication.
 * ----------------------------------------------------------------------------
 *
 * (c) 1997 STS AG, Zuerich, Switzerland.
 *
 * ----------------------------------------------------------------------------
 * Description
 *
 * ClientConnectTCP -- Connect to a TCP service.
 * ClientConnectTimeoutTCP -- Connect to a TCP service with timeout.
 * ClientConnectUDP -- Connect to a UDP service.
 *
 * ----------------------------------------------------------------------------
 */

#ifndef __ClientLib_h__

#define __ClientLib_h__

/* ----------------------------------------------------------------------------
 * INCLUDES
 */

#include "machine.h"

/* ----------------------------------------------------------------------------
 * DEFINES
 */

#ifdef COMPILE_CLIENTLIB
#  define CLIENTLIBEXPORT SHLIBEXPORT
#else
#  define CLIENTLIBEXPORT SHLIBIMPORT
#endif

/* ----------------------------------------------------------------------------
 * TYPES
 */

/* ----------------------------------------------------------------------------
 * GLOBAL VARIABLES
 */
CLIENTLIBEXPORT int	ClientLibErrNo;	/* Error number */

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
CLIENTLIBEXPORT
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
CLIENTLIBEXPORT
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
CLIENTLIBEXPORT
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
CLIENTLIBEXPORT
int
ClientConnectUDP(const char *host, const char *service);

#endif /* __ClientLib_h__  */
