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

#define MAXARGC		50		/* max number of arguments in buf */
#define WHITE		" \t\n" 	/* white space to split lines */
#define BRACKET		"\""	 	/* bracket to split lines */
#define WHITE_BRACKET	" \t\n\"" 	/* white space and bracket to split lines */

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

int
main(int argc, char **argv)
{
  int i;
	char *ptr;
	char *sep;
	char sepBuf[50];
	char *bracketPtr;
	
	sprintf(sepBuf, "%s", WHITE_BRACKET);
	sep = sepBuf;

	/* if in the separator string is a bracket set this to null */
	bracketPtr = (char *)strpbrk(sepBuf, BRACKET);
	if (bracketPtr == NULL) {
	 printf("bracket not in separator buf\n");
	}
	else {
	 printf("bracket in separator buf\n");	
	}

	sprintf(sepBuf, "%s", WHITE);
	sep = sepBuf;

	/* if in the separator string is a bracket set this to null */
	bracketPtr = (char *)strpbrk(sepBuf, BRACKET);
	if (bracketPtr == NULL) {
	 printf("bracket not in separator buf\n");
	}
	else {
	 printf("bracket in separator buf\n");	
	}
	
} /* End of main */
