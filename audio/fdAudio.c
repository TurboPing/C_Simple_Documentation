/* ----------------------------------------------------------------------------
 * fdTest.c -- file descriptor test with read/write
 * ----------------------------------------------------------------------------
 *
 * (c) 1997 STS AG, Zuerich, Switzerland.
 *
 * ----------------------------------------------------------------------------
 *
 * Status : Release 1.0
 *
 * ----------------------------------------------------------------------------
 * Description
 *
 *
 *
 */

/*-----------------------------------------------------------------------------
 * INCLUDES
 *-----------------------------------------------------------------------------
 */

/* System includes */
/*******************/

#include <stdio.h>
#include <sys/file.h>
#include <fcntl.h>

/* Local Includes */
/******************/

/* ----------------------------------------------------------------------------
 * LOCAL DEFINES
 * ----------------------------------------------------------------------------
 */

#define	LINELEN		1000

/* ----------------------------------------------------------------------------
 * LOCAL TYPES
 * ----------------------------------------------------------------------------
 */

/* ----------------------------------------------------------------------------
 * GLOBAL VARIABLES
 * ----------------------------------------------------------------------------
 */

extern int	errno;
extern char	*sys_errlist[];

/* ----------------------------------------------------------------------------
 * LOCAL VARIABLES
 * ----------------------------------------------------------------------------
 */  

/* ----------------------------------------------------------------------------
 * LOCAL FUNCTIONS (DECLARATIONS)
 * ----------------------------------------------------------------------------
 */

/* ----------------------------------------------------------------------------
 * LOCAL FUNCTIONS (BODIES)
 * ----------------------------------------------------------------------------
 */


/*------------------------------------------------------------------------
 * main - TCP client for ECHO service
 *------------------------------------------------------------------------
 */
int
main(argc, argv)
int	argc;
char	*argv[];

{
	int c;
	int i;
	int loop;
		
	int audioFd;
	int fileFd;

	char inputDevice[100];
	char outputDevice[100];

	char buf[1000];
	int n;

	/* set default value */
	
	loop = 1;
	sprintf(inputDevice, "test.au");
	sprintf(outputDevice, "/dev/audio");
	
	while ((c = getopt(argc, argv, "i:o:l:")) != EOF) {
		switch(c) {
		case 'i':	
			sprintf(inputDevice, "%s", optarg);
			break;
		case 'o':	
			sprintf(outputDevice, "%s", optarg);
			break;
		case 'l':	
			loop = atoi(optarg);
			break;
		case '?':
			exit(1);
			break;
		}
	}
	
	printf("input:%s / output:%s\n\n", inputDevice, outputDevice);

	audioFd = creat(outputDevice, 0777);

	for (i = 0; i<loop; i++) {
	
		fileFd = open(inputDevice, O_RDONLY, 0);
	
		printf("inputDeviceFd:%d / outputDeviceFd:%d\n\n", 
			fileFd, audioFd);
			
		while ((n = read(fileFd, buf, 100)) > 0) {
	
			write(audioFd, buf, n);

		}

		close(fileFd);
	}
}

