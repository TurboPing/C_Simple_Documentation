/* ----------------------------------------------------------------------------
 * SvConn.h -- Basic library module for socket handling on server side
 * ----------------------------------------------------------------------------
 *
 * (c) 1995 Alcatel STR, Zuerich, Switzerland.
 *
 * ----------------------------------------------------------------------------
 *
 * $Id: SvConn.h,v 1.4 1995/09/25 06:28:21 --- Exp $
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
 * SvConnAppendService   -- append server service to list
 * SvConnDestroyList     -- destroy a service list
 * SvConnSelect          -- select a connection request
 *
 * The following operations are not implemented
 *
 * SvConnRead            -- read 
 * SvConnWrite           -- write
 * SvConnClose           -- close a connection
 * SvConnConfig          -- set config data
 * SvConnGetStats        -- read statistics on a connection
 *
 * ----------------------------------------------------------------------------
 */

#ifndef __SvConn_h__

#define __SvConn_h__

/* ----------------------------------------------------------------------------
 * INCLUDES
 */
#include "a8620types.h"

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
SvConnAppendService(AhList list, char *service, char prot, int (*callBack)());

/* ----------------------------------------------------------------------------
 * SvConnCreateList      -- destroy a service list
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
SvConnSelect(AhList list);
 
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
Boolean
SvConnRead();

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
Boolean
SvConnWrite();

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

/* ----------------------------------------------------------------------------
 * SvConnConfig         -- config a connection    not implemented
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
SvConnConfig();

/* ----------------------------------------------------------------------------
 * SvConnGetStats       -- get statistics         not implemented
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
SvConnGetStats();

#endif __SvConn_h__
