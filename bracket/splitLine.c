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

/* ----------------------------------------------------------------------------
 * SplitLine -- Split a line
 *
 * Arguments:      None.
 *
 * Returns:        .
 *
 * Error Messages: None.
 *
 */
int
SplitLine(char *buf, char *separator, int *argc, char **argv)
{
	int bracket;
	char *ptr;
	char *sep;
	char sepBuf[50];
	char *bracketPtr;

	sprintf(sepBuf, "%s%s", separator, BRACKET);
	sep = sepBuf;
	bracket = 0;

	/* skip leading blank or tabs */
	while ((buf[0] == '\t') || (buf[0] == ' ')) {
		buf++;
	}
	
	if (buf[0] == '\n') {
		/* empty line */
		return(-1);
	}

	*argc = 0;
	argv[*argc] = buf;
	ptr = buf;

	bracketPtr = (char *)strpbrk(separator, BRACKET);

	while ((ptr = (char *)strpbrk(ptr, sep)) != (char *)NULL ) {
		
		if (*ptr == '"') {
			if (bracket == 0) {
			  sep = BRACKET;
			  bracket = 1;
			  if (bracketPtr != NULL) {
			    *ptr = '\0';
 			    ptr++;
			    argv[*argc] = ptr;
			  }
			  else {
			    ptr++;
			  }
			}
			else {
			  sep = sepBuf;
			  bracket = 0;
			  if (bracketPtr != NULL) {
			    *ptr = '\0';
			  }
			  ptr++;
			}
		}
		else {
		  *ptr = '\0';
		  ptr++;			  
 		  if (strchr(WHITE, *ptr) == NULL) {
		    argv[++*argc] = ptr;
		  }
		}		

		if( *argc >= MAXARGC - 1 ) { 
			/* -1 for room for NULL at end */
			printf("Too many arguments\n" );
			return(-1);
		}

	}

	argv[++*argc] = NULL;

	return(0);

} /* End of SplitLine */

int
splitLine(line) {

  int i;
  char	*args[MAXARGC];
  int   narg;

    /* split up in different strings */    
    if (SplitLine(line, WHITE_BRACKET, &narg, args) == -1) {
      /* Ignore empty line */
      printf("Empty line\n");
    }

    else {
    
      for (i=0; i<narg; i++) {
        printf("args[%d] = \"%s\"\n", i, args[i]);
      }
    
    }

}

int
main(int argc, char **argv)
{

  char line[2048];
	char *ptr;
	char *sep;
	char sepBuf[50];
	char *bracketPtr;

    sprintf(line, "%s", "   arg1		arg2 \"arg3   arg4\" arg5 	arg6     \"arg7 	arg77 arg777\"   arg8 	");
    splitLine(line);

    sprintf(line, "%s", "	  	");
    splitLine(line);

    sprintf(line, "%s", "\"arg1	arg2  arg3	   arg4		arg5 \" 	");
    splitLine(line);

    return 0;
    	
} /* End of main */
