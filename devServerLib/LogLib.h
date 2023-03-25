/* ----------------------------------------------------------------------------
 * LogLib.h -- Basic library module for error handling.
 * ----------------------------------------------------------------------------
 *
 * (c) 1995 Alcatel STR, Zuerich, Switzerland.
 *
 * ----------------------------------------------------------------------------
 *
 * $Id: LogLib.h,v 2.4 1996/06/07 08:45:43 roelli Exp $
 *
 * Status : Release 1.0
 *
 * ----------------------------------------------------------------------------
 * Description
 *
 * This module implements logging functions. It is based on the syslog daemon
 * syslogd. This module used 2 facilities and 3 level of the syslog daemon.
 * The 2 facilities are : user, local0.
 * The 3 level are : err, info, debug.
 *
 * For configuration of the LogLib see also the manual page for syslogd.
 * Each function use one priority specification in syslog.conf.
 *
 * Example for a sequence in file /etc/syslog.conf:
 *
 * #
 * #define for debugging and user application for A8620
 * #
 * local0.err                     /var/adm/local0Err
 * local0.info                    /var/adm/local0Info
 * local0.debug                   /var/adm/local0Debug
 * user.err                       /var/adm/userErr
 * user.info                      /var/adm/userInfo
 * user.debug                     /var/adm/userDebug
 *
 * The following operations are provided:
 *
 * LogOpen        -- Open the connection to syslog with identification.
 * LogSetHeader   -- Set the header for the debug output. (no longer active)
 * LogDebugMessage-- Write a debug message and return. (local0.debug)
 * LogMessage     -- Write a normal message and return. (local0.info)
 * LogReturn      -- Write an error message and return. (local0.err)
 * LogSystem      -- Write a system error message and return. (local0.err)
 * LogQuit        -- Write an error message and exit. (local0.err)
 * LogDump        -- Write an error message and dump core and exit. (local0.err)
 * LogUserDebug   -- Write a debug message and return. (user.debug)
 * LogUserMessage -- Write a normal message and return. (user.info)
 * LogUserReturn  -- Write an error message and return. (user.err)
 * LogUserSystem  -- Write a system error message and return. (user.err)
 *
 * ----------------------------------------------------------------------------
 */

#ifndef __LogLib_h__

#define __LogLib_h__

/* ----------------------------------------------------------------------------
 * INCLUDES
 */
/*
#include <stdarg.h>
*/

#include <a8620types.h>

/* ----------------------------------------------------------------------------
 * DEFINES
 */

/* ----------------------------------------------------------------------------
 * TYPES
 */

/* ----------------------------------------------------------------------------
 * GLOBAL VARIABLES
 */
External int	LogLibErrNo;	/* Error number */
External char	*LogLibErrMsg;	/* Error message */

/* ----------------------------------------------------------------------------
 * PROCEDURES
 */

/* ----------------------------------------------------------------------------
 * LogOpen -- Open the connection to syslog with identification.
 *
 * Arguments:      identification -- identification string.
 *
 * Returns:        always true.
 *
 * Error Messages:   None.
 *
 */
External
Boolean
LogOpen(char *ident);

/* ----------------------------------------------------------------------------
 * LogSetHeader -- Set the header for the error printout. (no longer active)
 *
 * Arguments:      fmt -- The format of the header message.
 *                 ... -- The parameters
 *
 * Returns:        None.
 *
 * Error Messages:   None.
 *
 */
External
void
LogSetHeader(const char *fmt, ...);

/* ----------------------------------------------------------------------------
 * LogDebugMessage -- Write a debug message and return.
 *
 * Arguments:      fmt -- The format of the debug message.
 *                 ... -- The parameters
 *
 * Returns:        None.
 *
 * Error Messages:   None.
 *
 */
External
void
LogDebugMessage(const char *fmt, ...);

/* ----------------------------------------------------------------------------
 * LogMessage -- Write a normal message and return.
 *
 * Arguments:      fmt -- The format of the normal message.
 *                 ... -- The parameters
 *
 * Returns:        None.
 *
 * Error Messages:   None.
 *
 */
External
void
LogMessage(const char *fmt, ...);

/* ----------------------------------------------------------------------------
 * LogReturn -- Write an error message and return.
 *
 * Arguments:      fmt -- The format of the error message.
 *                 ... -- The parameters
 *
 * Returns:        None.
 *
 * Error Messages:   None.
 *
 */
External
void
LogReturn(const char *fmt, ...);

/* ----------------------------------------------------------------------------
 * LogSystem -- Write a system error message and return.
 *
 * Arguments:      fmt -- The format of the error message.
 *                 ... -- The parameters
 *
 * Returns:        None.
 *
 * Error Messages:   None.
 *
 */
External
void
LogSystem(const char *fmt, ...);

/* ----------------------------------------------------------------------------
 * LogQuit -- Write an error message and exit.
 *
 * Arguments:      fmt -- The format of the error message.
 *                 ... -- The parameters
 *
 * Returns:        None.
 *
 * Error Messages:   None.
 *
 */
External
void
LogQuit(const char *fmt, ...);

/* ----------------------------------------------------------------------------
 * LogDump -- Write an error message and dump core and exit.
 *
 * Arguments:      fmt -- The format of the error message.
 *                 ... -- The parameters
 *
 * Returns:        None.
 *
 * Error Messages:   None.
 *
 */
External
void
LogDump(const char *fmt, ...);

/* ----------------------------------------------------------------------------
 * LogUserDebug -- Write a debug message and return.
 *
 * Arguments:      fmt -- The format of the debug message.
 *                 ... -- The parameters
 *
 * Returns:        None.
 *
 * Error Messages:   None.
 *
 */
External
void
LogUserDebug(const char *fmt, ...);

/* ----------------------------------------------------------------------------
 * LogUserMessage -- Write a normal message and return.
 *
 * Arguments:      fmt -- The format of a normal message.
 *                 ... -- The parameters
 *
 * Returns:        None.
 *
 * Error Messages:   None.
 *
 */
External
void
LogUserMessage(const char *fmt, ...);

/* ----------------------------------------------------------------------------
 * LogUserReturn -- Write an error message and return.
 *
 * Arguments:      fmt -- The format of the error message.
 *                 ... -- The parameters
 *
 * Returns:        None.
 *
 * Error Messages:   None.
 *
 */
External
void
LogUserReturn(const char *fmt, ...);

/* ----------------------------------------------------------------------------
 * LogUserSystem -- Write a system error message and return.
 *
 * Arguments:      fmt -- The format of the error message.
 *                 ... -- The parameters
 *
 * Returns:        None.
 *
 * Error Messages:   None.
 *
 */
External
void
LogUserSystem(const char *fmt, ...);

#endif __LogLib_h__
/* End of LogLib.h */
