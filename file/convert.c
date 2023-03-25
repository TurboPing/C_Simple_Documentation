/* ----------------------------------------------------------------------------
 * convert.c -- read and convert a file.
 * ----------------------------------------------------------------------------
*/

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

#include <sys/times.h>
#include <limits.h>

#include <netinet/in.h>
#ifdef SYSV
#	include <limits.h>
#	include <stropts.h>
#else
#	include <sys/param.h>
#endif

int ConvertFile(char *filename)
{
	FILE	*nf;
	char	buf[1000];
        int     i;
	int     n;
        char    c;
		
	/* Open the file */
	if ((nf = fopen(filename, "r")) == NULL) {
		/* No new file */
		printf("no such file found : %s\n\n", filename);
		return(-1);
		
	} else {

                printf("filename:%s|nf:%d\n", filename, (int)nf);

                while (fgets(buf, 1000, nf) != NULL) {
                
		  /* for(i=0; i<2; i++) {
		     printf("%c", c[i]);
		   }
		   printf(" ");
		   
		   printf("%s", &c[2]);
		   */
		}
	   	
	   	fclose(nf);
		return(0);

	}
	

}

/* ----------------------------------------------------------------------------
 * main -- Main Program
 *
 */
int
main(int argc, char **argv)
{
  char c;	
  char filename[100];

  while ((c = getopt(argc, argv, "f:vrmt")) != EOF) {
    switch(c) {
    case 'f':
      sprintf(filename, "%s", optarg);
      break;
    default :
      break;
    }
  }

  ConvertFile(filename);
  return(0);
					
}
/* End of main */
