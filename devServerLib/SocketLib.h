/* ----------------------------------------------------------------------------
 * SocketLib.h -- Basic library module for socket handling on server side
 * ----------------------------------------------------------------------------
 *
 * (c) 1995 Alcatel STR, Zuerich, Switzerland.
 *
 * ----------------------------------------------------------------------------
 *
 * $Id: SocketLib.h,v 1.4 1995/09/25 06:28:21 --- Exp $
 *
 * Status : Release 1.0
 *
 * ----------------------------------------------------------------------------
 * Description
 *
 * This module implements socket as abstract data-types. The following
 * operations on socket are provided:
 *
 * SvConnCreateList      -- create a empty service list
 * SvConnDestroyList     -- destroy a service list
 * SvConnAppendService   -- append server service to list
 * SvConnSelect          -- select a connection request
 * SvConnRead            -- read 
 * SvConnWrite           -- write
 * SvConnClose           -- close
 *
 * ----------------------------------------------------------------------------
 */

#ifndef __SocketLib_h__

#define __SocketLib_h__

/* ----------------------------------------------------------------------------
 * INCLUDES
 */

#include <sys/time.h>
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
External int	SvConnErrNo;	/* Error number - not used */
External char	*SvConnErrMsg;	/* Error message - not used */

/* ----------------------------------------------------------------------------
 * LOCAL VARIABLES
 */

/* ----------------------------------------------------------------------------
 * PROCEDURES
 */

/* ----------------------------------------------------------------------------
 * SvConnCreateList      -- create a new list for server services
 *
 * Arguments:      
 *
 * Returns:        
 *
 * Error Messages:    
 *
 */
External
AhList
SvConnCreateList();

/* ----------------------------------------------------------------------------
 * SvConnAppendService   -- append a server service
 *
 * Arguments:      
 *
 * Returns:        
 *
 * Error Messages:    
 *
 */
External
Boolean
SvConnAppendService(AhList list, char *service, int prot, int (*callBack)());

/* ----------------------------------------------------------------------------
 * SvConnDestroyList      -- destroy a service list
 *
 * Arguments:      
 *
 * Returns:        
 *
 * Error Messages:    
 *
 */
External
Boolean
SvConnDestroyList(AhList list);

/* ----------------------------------------------------------------------------
 * SvConnSelect       -- accept one or more connection 
 *
 * Arguments:      
 *
 * Returns:        
 *
 * Error Messages:    
 *
 */
External
Boolean
SvConnSelect(AhList list, struct timeval *timeout, int (*timeoutCallback)());
 
/* ----------------------------------------------------------------------------
 * SvConnRead         -- read on a connection     not implemented
 *
 * Arguments:      
 *
 * Returns:        
 *
 * Error Messages:    
 *
 */
External
int
SvConnRead(int fd,char *buf, int buflen);

/* ----------------------------------------------------------------------------
 * SvConnWrite        -- write on a connection    not implemented
 *
 * Arguments:      
 *
 * Returns:        
 *
 * Error Messages:    
 *
 */
External
int
SvConnWrite(int fd, char *buf, int buflen);

/* ----------------------------------------------------------------------------
 * SvConnClose        -- close a connection       not implemented
 *
 * Arguments:      
 *
 * Returns:        
 *
 * Error Messages:    
 *
 */
External
Boolean
SvConnClose();

#endif __SocketLib_h__
