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

#define R2A_COMMAND	"%s/bin/r2a"	
#define RES_FILE   "%s/results/%s/%s.res1"
#define ASCII_FILE   "%s/results/%s/%s.ascii1"

#define DIFF_COMMAND	"%s/bin/TecoDiffResult"	

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
 * main -- Main program for TecoFillData.
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

  #define VALUE1 0x0001
  #define VALUE2 0x0002
  #define VALUE3 0x0004
  #define VALUE4 0x0008
  #define VALUE5 0x0010

  int value;
  int i;
  
  value = 15;
  
  printf("out is %d\n", (value & VALUE1));

  printf("out is %d\n", (value & VALUE2));

  printf("out is %d\n", (value & VALUE3));

  printf("out is %d\n", (value & VALUE4));

  printf("out is %d\n", (value & VALUE5));

  exit(0);
  	
} /* End of main */

/* End of TecoFillData.c */
