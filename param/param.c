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
  int c;
  int i;
  
  int argc2;
  char *argv2[100];

  printf("optind : %d\n", optind);
  while (  (c = getopt ( argc, argv, "a:b:c:" )) != EOF  ) {
	switch(c) {
	case 'a': printf("option -a %s\n", optarg);
		break;

	case 'b': printf("option -b %s\n", optarg);
	        break;

	case 'c': printf("option -c %s\n", optarg);
                break;

        default : printf("nothing\n");
	        break;
	}
  }

  optind = 1;
  printf("optind : %d\n", optind);

  argc2=7;
  argv2[0]="command";
  argv2[1]="-a";
  argv2[2]="arga";
  argv2[3]="-b";
  argv2[4]="argb";  
  argv2[5]="-c";
  argv2[6]="argc";  
  argv2[7]=NULL;

  while (  (c = getopt ( argc2, argv2, "a:b:c:" )) != EOF  ) {
	switch(c) {
	case 'a': printf("option -a %s\n", optarg);
		break;

	case 'b': printf("option -b %s\n", optarg);
	        break;

	case 'c': printf("option -c %s\n", optarg);
                break;

        default : printf("nothing\n");
	        break;
	}
  }

  for (i = 0; i < argc2; i++) {
    printf("argc : %d | argv %s\n", i, argv2[i]);
    if (strncmp(argv2[i], "-a", 2) == 0) {
      printf("arc : %d | argv %s -> found -a : %s\n", i, argv2[i], argv2[i+1]);    
      i++;
    }
    if (strncmp(argv2[i], "-b", 2) == 0) {
      printf("arc : %d | argv %s -> found -b : %s\n", i, argv2[i], argv2[i+1]);    
      i++;
    }
    if (strncmp(argv2[i], "-c", 2) == 0) {
      printf("arc : %d | argv %s -> found -c : %s\n", i, argv2[i], argv2[i+1]);    
      i++;
    }    
    printf("\n");
  }

  exit(0);
  
	
} /* End of main */

/* End of TecoFillData.c */
