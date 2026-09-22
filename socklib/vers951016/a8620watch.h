/* ----------------------------------------------------------------------------
 * a8620watch.h -- Header file for a8620 watch daemon library.
 * ----------------------------------------------------------------------------
 *
 * (c) 1995 Alcatel STR, Zuerich, Switzerland.
 *
 * ----------------------------------------------------------------------------
 *
 * $Id: a8620watch.h,v 1.1 1995/07/26 09:31:30 herren Exp herren $
 *
 * Status : Release 1.0 beta
 *
 * ----------------------------------------------------------------------------
 * Description
 *
 * This header file defines the interface for the a8620watchd facility.
 * The a8620watchd provides some services used in the a8620 environment.
 * These services include:
 *
 * - A watchdog function for processes with questionable stability. These
 *   processes can attach to the a8620watchd and set a command that will be
 *   executed whenever the client process dies prematurely. So it is possible
 *   to restart the client process through the a8620watchd.
 *
 * - A remote execution facility. A client process can tell the a8620watchd
 *   to execute a command (in back- or foreground) on a remote machine.
 *
 * - A facility to guess the bandwidth available between the client and the
 *   a8620watchd.
 *
 * Interface Short Cut
 *
 * AwAttach        -I- Attach to the a8620watchd.
 * AwSetCmd        -I- Set the command for a connection loss.
 * AwExecCmd       -I- Execute a command on a a8620watchd.
 * AwDetach        -I- Detach correctly from the a8620watchd.
 * AwAttachCmd     -I- Attach to the a8620watchd with a command line.
 * AwPing          -I- Send a ICMP ECHO request.
 * AwGetBandWidth  -I- Get the bandwidth for the connection to a8620watchd.
 * AwSetDebugLevel -I- Set the debug level for the a8620watch interface.
 *
 * Legend: -I- Implemented.
 *         -N- Not yet implemented.
 *
 * ----------------------------------------------------------------------------
 */

#include <a8620types.h>

#ifndef __a8620watch_h__

#define __a8620watch_h__

static char *a8620watch_h_rcsid = "$Id: a8620watch.h,v 1.1 1995/07/26 09:31:30 herren Exp herren $";

/* ----------------------------------------------------------------------------
 * INCLUDES
 */

/* ----------------------------------------------------------------------------
 * DEFINES
 */
#define AW_DEBUG_ATTACH		0x0001
#define AW_DEBUG_DETACH		0x0002
#define AW_DEBUG_SET_CMD	0x0004
#define AW_DEBUG_ATTACH_CMD	0x0008
#define AW_DEBUG_EXEC_CMD	0x0010
#define AW_DEBUG_PROTO		0x0020

/* ----------------------------------------------------------------------------
 * TYPES
 */
typedef Pointer	AwConnection;	/* Handle for the connection to a8620watchd */

/* ----------------------------------------------------------------------------
 * GLOBAL VARIABLES
 */
External int	AwErrNo;		/* Error number */
External char	*AwErrMsg;		/* Error messages */
External int	AwServErrNo;	/* Error number from server*/
External char	*AwServErrMsg;	/* Error messages from server */

/* ----------------------------------------------------------------------------
 * LOCAL VARIABLES
 */

/* ----------------------------------------------------------------------------
 * PROCEDURES
 */

/* ----------------------------------------------------------------------------
 * AwAttach -- Attach to the a8620watchd.
 *
 * Arguments:   server -- Hostname of the server host.
 *              progname -- name of the own program.
 *
 * Returns:     != NULL -- The client successfully attached to the server.
 *              == NULL -- Some error occured, see AwErrNo and AwErrMsg above.
 *
 * Status:      Implemented.
 *
 */
External
AwConnection
#ifdef UsePrototypes
AwAttach(char *server, char *progname);
#else
AwAttach();
#endif

/* ----------------------------------------------------------------------------
 * AwSetCmd -- Set the command for a connection loss.
 *
 * Arguments:   conn   -- The connection to be configured.
 *              cmd    -- Command line to be executed on the server host by
 *                        the a8620watchd whenever the client dies without
 *                        calling AwDetach.
 *
 * Returns:     0      -- The command was succesfully set on the server.
 *              < 0    -- Some error occured, see AwErrNo and AwErrMsg above.
 *
 * Status:      Implemented.
 *
 */
External
Boolean
#ifdef UsePrototypes
AwSetCmd(AwConnection conn, char * command);
#else
AwSetCmd();
#endif

/* ----------------------------------------------------------------------------
 * AwExecCmd -- Execute a command on a a8620watchd.
 *
 * Arguments:   conn   -- The connection to be used.
 *              user   -- The name of the user for whom the command should
 *                        be run.
 *              cmd    -- The command to be executed on the a8620watchd.
 *              runBg  -- true == The command will be run in background.
 *                        This type of call always returns with a exit
 *                        status of 0.
 *                        false == The command will be run in foreground.
 *                        During the exection of the command the call does
 *                        not return.
 *
 * Returns:     >= 0   -- The exit status of the command.
 *              < 0    -- Some error occured, see AwErrNo and AwErrMsg above.
 *
 * Status:      Implemented.
 *
 */
External
int
#ifdef UsePrototypes
AwExecCmd(AwConnection conn, char *user, char *command, Boolean runBg);
#else
AwExecCmd();
#endif

/* ----------------------------------------------------------------------------
 * AwDetach -- Detach correctly from the a8620watchd.
 *
 * Arguments:   conn   -- The connection to be closed.
 *
 * Returns:     true  == The client successfully detached from the server.
 *              false == Some error occured, see AwErrNo and AwErrMsg above.
 *
 * Status:      Implemented.
 *
 */
External
Boolean
#ifdef UsePrototypes
AwDetach(AwConnection conn);
#else
AwDetach();
#endif

/* ----------------------------------------------------------------------------
 * AwAttachCmd -- Attach to the a8620watchd with a command line.
 *
 * Arguments:   server -- Hostname of the server host.
 *              progname -- name of the own program.
 *              cmd    -- Command line to be executed on the server host by
 *                        the a8620watchd whenever the client dies without
 *                        calling AwDetach.
 *
 * Returns:     != NULL -- The client successfully attached to the server.
 *              == NULL -- Some error occured, see AwErrNo and AwErrMsg above.
 *
 * Status:      Implemented.
 *
 */
External
AwConnection
#ifdef UsePrototypes
AwAttachCmd(char *server, char *progname, char *cmd);
#else
AwAttachCmd();
#endif

/* ----------------------------------------------------------------------------
 * AwPing -- Send a ICMP ECHO request.
 *
 * Arguments:   hname  -- Hostname to send to.
 *              plen   -- Size of the packet to be transmitted (0 == use
 *                        default value of 56).
 *              maxretries  --
 *                        Number of retries permitted.
 *              timeout --
 *                        Maximum time to wait for a reply after a send.
 *              ttime  -- Travel time. Time it took the packet to be trans-
 *                        mitted to the server and back in micro-seconds.
 *              ttime  -- Travel time. Time it took the packet to be trans-
 *                        mitted to the server and back in micro-seconds.
 *
 * Returns:     0      -- The client successfully detached from the server.
 *              < 0    -- Some error occured, see AwErrNo and AwErrMsg above.
 *
 * Status:      Implemented.
 *
 * Known limitations:
 *              The AwPing procedure can only be called by processes running
 *              as root.
 *              The AwPing call uses the SIGALRM signal to set its timeout.
 *              It sets the signal-mask to fetch SIGALRM. After return from
 *              AwPing the signal-mask is set back to the old one.
 *
 */
External
int
#ifdef UsePrototypes
AwPing(char *hname, int plen, int maxretries, int timeout, long *ttime);
#else
AwPing();
#endif

/* ----------------------------------------------------------------------------
 * AwGetBandWidth -- Get the bandwidth for the connection to a8620watchd.
 *
 * Arguments:   server -- Hostname of the server host.
 *              psize  -- Size of the packet to be transmitted (0 == use
 *                        default value of 56).
 *              ttime  -- Travel time. Time it took the packet to be trans-
 *                        mitted to the server and back in micro-seconds.
 *              bwidth -- Bandwith calculated from the ttime in bits per
 *                        second.
 *
 * Returns:     0      -- The client successfully detached from the server.
 *              < 0    -- Some error occured, see AwErrNo and AwErrMsg above.
 *
 * Status:      Implemented.
 *
 * Known limitations:
 *              The AwGetBandWidth procedure can only be called by
 *              processes running as root.
 *              The AwGetBandWidth call uses the SIGALRM signal to set
 *              its timeout.
 *              It sets the signal-mask to fetch SIGALRM. After return from
 *              AwGetBandWidth the signal-mask is set back to the old one.
 *
 */
External
int
#ifdef UsePrototypes
AwGetBandWidth(char *server, long *psize, long *ttime, long *bandwidth);
#else
AwGetBandWidth();
#endif

/* ----------------------------------------------------------------------------
 * AwSetDebugLevel -- Set the debug level for the a8620watch interface.
 *
 * Arguments:   level -- ORed AW_DEBUG_.. defines.
 *                       -1 --> returns the current level and does not change
 *                              this level.
 *
 * Returns:     >= 0   -- The level which was set was set.
 *              < 0    -- Some error occured, see AwErrNo and AwErrMsg above.
 *
 *
 * Status:      Implemented.
 *
 */
External
int
#ifdef UsePrototypes
AwSetDebugLevel(int level);
#else
AwSetDebugLevel();
#endif

#endif __a8620watch_h__
