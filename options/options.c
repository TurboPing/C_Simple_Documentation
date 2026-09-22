#include <stdio.h>
#include <stdarg.h>
#include <signal.h>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>

/* ----------------------------------------------------------------------------
 * EXTERNALS
 */
extern int	errno;
extern char     *sys_errlist[];
extern int   optind;
extern char  *optarg;

/* ----------------------------------------------------------------------------
 * DEFINES
 */
#define MAXARGC		50		/* max number of arguments in buf */
#define MAX_SELECT_FD  256
#define WHITE		" \t\n" 	/* white space to split lines */
#define BRACKET		"\""	 	/* white space to split lines */
#define VERTICALLINE	"|\t\n" 	/* vertical to split lines */
#define DOUBLEPOINT	":\t\n" 	/* double point to split lines */

#define MAX_COLUMNS	10
#define MAX_ROWS	1000


int argc;
char *argv[50];
va_list ap;
char optionBuf[500];

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
static
int
SplitLine(char *buf, char *separator, int *argc, char **argv)
{
	int bracket;
	char *ptr;
	char *sep;
	char sepBuf[50];
	
	printf("Proc : SplitLine\n");

	sprintf(sepBuf, "%s%s", separator, BRACKET);
	sep = sepBuf;
	bracket = 0;

	*argc = 0;
	argv[*argc] = buf;
	ptr = buf;

	while ((ptr = (char *)strpbrk(ptr, sep)) != (char *)NULL ) {
		
		if (*ptr == '"') {
			if (bracket == 0) {
			  sep = BRACKET;
			  bracket = 1;
			  *ptr = '\0';
			  ptr++;
			  argv[*argc] = ptr;
			}
			else {
			  sep = sepBuf;
			  bracket = 0;
			  *ptr = '\0';
			  ptr++;
			}
		}
		else {
		  *ptr = '\0';
		  ptr++;			  
		  argv[++*argc] = ptr;
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

/* ----------------------------------------------------------------------------
 * Convert2Options
 *
 * Arguments:      None.
 *
 * Returns:        Nothing.
 *
 * Error Messages: None.
 *
 */
static
void
Convert2Options(char *procname, int *argc, char **argv, 
		const char *fmt, va_list ap, char *optionBuf) { 

	int i;
	char buf1[1000];

	printf("Proc : Convert2Options\n");

	vsprintf(buf1, fmt, ap);
	printf("Input : %s %s\n", procname, buf1);	

	/* set option buffer */
	sprintf(optionBuf, "%s", buf1);
	
	/* init argc/argv */
	for (i=0; i<*argc; i++) {
		argv[i] = NULL;
	}
	*argc = 0;
			
	if (SplitLine(buf1, WHITE, argc, argv) != -1) {
		/* Split line ok */		
	}

	/* init getopt values */
	optind = 0;
		
} /* End of Convert2Options */

static
int
options(const char *fmt, ...) 
{

  int c;
  int i;
  
  printf("Proc : DbConnect\n");

  /* test and set of the options */
  va_start(ap, fmt);
  Convert2Options("DbConnect", &argc, argv, fmt, ap, optionBuf);
  va_end(ap);

  printf("optionBuf:%s\n", optionBuf);
  for (i=0; i<argc; i++) {
    printf("argc:%d|argv:%s\n", i, argv[i]);
  }

  /* set default */

  /* set options */
  printf("optind : %d\n", optind);
  while (  (c = getopt ( argc, argv, "S:U:P:X:Z:" )) != EOF  ) {
    switch(c) {
	case 'S': printf("option -S %s\n", optarg);
		break;

	case 'U': printf("option -U %s\n", optarg);
		break;

	case 'P': printf("option -P %s\n", optarg);
		break;

	case 'X': printf("option -X %s\n", optarg);
		break;

	case 'Z': printf("option -Z %s\n", optarg);
		break;

	default : printf("nothing\n");
		break;

    }
  }



}

main () {


  options("-S ssssss -U uuuuuuuuuuuuu -X xxxxxxxxxxxxxxx -Z zzzzzzzzzzzzzzz -P pppppppppppppppp");

}