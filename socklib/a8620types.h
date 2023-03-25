/* ----------------------------------------------------------------------------
 * a8620types.h -- Types and defines for a8620.
 * ----------------------------------------------------------------------------
 *
 * (c) 1995 Alcatel STR, Zuerich, Switzerland.
 *
 * ----------------------------------------------------------------------------
 *
 * $Id: a8620types.h,v 1.1 1995/07/26 09:31:30 herren Exp herren $
 *
 * ----------------------------------------------------------------------------
 */

#ifndef __a8620types_h__

#define __a8620types_h__

/* ----------------------------------------------------------------------------
 * INCLUDES
 */

/* ----------------------------------------------------------------------------
 * DEFINES
 */
#define Private	static
#define Public extern	
#define External	extern

#ifdef __STDC__
#define UsePrototypes
#endif

/* ----------------------------------------------------------------------------
 * TYPES
 */
typedef void *Pointer;

typedef char *String;

typedef enum {
	true = 1, false = 0
} Boolean;

#endif __a8620types_h__
