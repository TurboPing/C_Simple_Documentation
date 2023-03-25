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
#include <string.h>

#include <netinet/in.h>
#ifdef SYSV
#	include <limits.h>
#	include <stropts.h>
#else
#	include <sys/param.h>
#endif

#define LINE_LEN 1000

char * incr(char * ptr) {

  ptr = ptr + 2;
  ptr = ptr + ((*ptr & 0xff) * 256) + (*(ptr + 1) & 0xff);
  return ptr;

}

int ConvertFile(char *filename)
{
	FILE	*fd;
	char	buf[1000000];
        int     i;
	int     n;
        char    c;
        int     nbytes;
        int     numberOfBytes;
        char    textBuf[1000];
        char   *ptr;
		
	/* Open the file */
	if ((fd = fopen(filename, "r")) == NULL) {
		/* No new file */
		printf("no such file found : %s\n\n", filename);
		return(-1);
		
	} else {

                printf("filename:%s|nf:%d\n", filename, (int)fd);

                numberOfBytes = 0;
                while (!feof(fd)) {
                  nbytes = fread(&buf[numberOfBytes], 1, LINE_LEN, (FILE *)fd);
                  numberOfBytes += nbytes;
                  printf("nbytes:%d|%d\n", nbytes, numberOfBytes);
                }
	   	
	   	fclose(fd);

                ptr = &buf[0];

                while (((*ptr & 0xff) != 0xff) || ((*(ptr + 1) & 0xff) != 0xd9)) {

                  if ((*ptr & 0xff) == 0xff) {

                    printf("ptr:%x %x\n", *ptr & 0xff, *(ptr + 1) & 0xff);

                    switch (*(ptr + 1) & 0xff) {

                    case 0xd8: printf("start of image\n");
                               ptr = ptr + 2;
                               break;

                    case 0xe0: printf("jfif tag\n");
                               ptr = incr(ptr);
                               break;                     

                    case 0xed: printf("appn\n");
                               ptr = incr(ptr);
                               break;                     

                    case 0xfe: printf("comment\n");
                               ptr = incr(ptr);
                               break;                     

                    case 0xee: printf("copyright\n");
                               printf("*(ptr + 3):%x\n", *(ptr + 3) & 0xff); 
                               strncpy(textBuf, (ptr + 4), *(ptr + 3) & 0xff);
                               textBuf[*(ptr + 3) & 0xff] = 0;
                               printf("text:%s\n", textBuf);
                               ptr = incr(ptr);
                               break;                     

                    case 0xdb: printf("dqt\n");
                               ptr = incr(ptr);
                               break;                     

                    case 0xc0: printf("sof0\n");
                               ptr = incr(ptr);
                               break;                     

                    case 0xdd: printf("dd?\n");
                               ptr = incr(ptr);
                               break;                     

                    case 0xc4: printf("dht\n");
                               ptr = incr(ptr);
                               break;                     

                    case 0xda: printf("sos\n");
                               ptr = ptr + 2;
                               break;                     

                    default: printf("unknown\n");
                             ptr = ptr + 2;
                             break;

                    }
  
                  } else {
                    printf("%x ", *ptr & 0xff);
                    ptr++;
                  }


                }

                printf("\nend : %x %x\n", *(ptr) & 0xff, *(ptr +1) & 0xff);

                

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
