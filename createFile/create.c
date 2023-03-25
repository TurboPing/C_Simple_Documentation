/* ----------------------------------------------------------------------------
 * convert.c -- read and convert a file.
 * ----------------------------------------------------------------------------
*/

/* ----------------------------------------------------------------------------
 * INCLUDES
 */
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <sys/stat.h>

/* ----------------------------------------------------------------------------
 * main -- Main Program
 *
 */
int
main(int argc, char **argv)
{
	
	int	nf;
	int nf2;
	char filename[100];
	int buflen;
	char	buf[1000];
	int     n;
		
	/* Open the file */
	
	sprintf(filename, "%s", "/home/user/c_code/createFile/gugus");
	
	if ((nf = creat(filename, 0666)) == -1) {
		/* No new file */
		printf("file open error : %d\n\n", errno);
		return(-1);
		
	} 

	printf("nf:%d|time:%d\n\n", nf, time(NULL));
		
	sprintf(buf, "%d", time(NULL));
	buflen = strlen(buf);
		
	write(nf, buf, buflen);
	   	
   	close(nf);
	return(0);
					
}
/* End of main */
