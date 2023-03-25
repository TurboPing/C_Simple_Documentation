/* arg.c */

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
 * main -- main program
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


  for (i=0;i<argc;i++) {
     printf("%s\n", argv[i]);
  }
  return 0;
  
	
} /* End of main */

