/* ----------------------------------------------------------------------------
 * TecoCleanUp.c -- Basic library module for error handling.
 * ----------------------------------------------------------------------------
 *
 * (c) 1995 Alcatel STR, Zuerich, Switzerland.
 *
 * ----------------------------------------------------------------------------
 *
 * $Id: TecoCleanUp.c,v 3.0 1996/03/04 12:16:15 roelli Exp $
 *
 * Status : Release 1.0
 *
 * ----------------------------------------------------------------------------
 * Description
 *
 * ----------------------------------------------------------------------------
 */

static char *TecoCleanUp_c_rcsid = "$Id: TecoCleanUp.c,v 3.0 1996/03/04 12:16:15 roelli Exp $";

/* ----------------------------------------------------------------------------
 * INCLUDES
 */
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>

#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/file.h>

#include <netinet/in.h>
#ifdef SYSV
#	include <limits.h>
#	include <stropts.h>
#else
#	include <sys/param.h>
#endif


/* ----------------------------------------------------------------------------
 * DEFINES
 */
#define MAX_LINE_LEN	2048
#define MAXARGC		50		/* max number of arguments in buf */
#define WHITE		" \t\n" 	/* white space to split lines */

/* ----------------------------------------------------------------------------
 * TYPES
 */

/* ----------------------------------------------------------------------------
 * GLOBAL VARIABLES
 */

/* ----------------------------------------------------------------------------
 * LOCAL VARIABLES
 */

/* ----------------------------------------------------------------------------
 * PROCEDURES
 */

/* ----------------------------------------------------------------------------
 * main -- Main program for TecoCleanUp.
 *
 * Arguments:      None.
 *
 * Returns:        None.
 *
 * Error Messages: None.
 *
 */
int
main(int argc, char **argv)
{
	
	int i;
	char 	ch;
	char buf[10];
	
 	for (i=0; i<=255; i++) {
 		ch = (char)i;
 		sprintf(buf, "%s", i);
      		printf("%d %s\n", i, buf);
	}

	exit(0);
	
} /* End of main */

/* End of TecoCleanUp.c */
