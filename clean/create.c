/* ----------------------------------------------------------------------------
 * TecoCleanUp.c -- Basic library module for error handling.
 * ----------------------------------------------------------------------------
 *
 * (c) 1995 Alcatel STR, Zuerich, Switzerland.
 *
 * ----------------------------------------------------------------------------
 *
 * $Id: TecoCleanUp.c,v 3.0 1996/03/04 12:16:15 roelli Exp $
 *
 * Status : Release 1.0
 *
 * ----------------------------------------------------------------------------
 * Description
 *
 * ----------------------------------------------------------------------------
 */

static char *TecoCleanUp_c_rcsid = "$Id: TecoCleanUp.c,v 3.0 1996/03/04 12:16:15 roelli Exp $";

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
 * Usage -- Print usage.
 *
 * Arguments:      None.
 *
 * Returns:        None.
 *
 * Error Messages: None.
 *
 */
void
Usage()
{

	fprintf(stderr, "Usage: clean source_file destination_file\n");
	
	return;
	
} /* End of Usage */

/* ----------------------------------------------------------------------------
 * MyProc -- .
 *
 * Arguments:      None.
 *
 * Returns:        None.
 *
 * Error Messages: None.
 *
 */
void
MyProc()
{
} /* End of MyProc */


/* ----------------------------------------------------------------------------
 * CleanUp -- .
 *
 * Arguments:      test -- The test name.
 *                 tru  -- The TRU name.
 *
 * Returns:        None.
 *
 * Error Messages: None.
 *
 */
void
CreateFile(char *destinationFile)
{
	int	nLines;
	int 	nBuf;
	char 	ch;
	char	buf[150];
	FILE	*srcFile;
	FILE	*destFile;
	FILE	*diffFile;
	char	filledResultFile[MAXPATHLEN];
	char	newResultFile[MAXPATHLEN];
	char	CleanUpFile[MAXPATHLEN];
	int		saveLine40;

	char	*args[MAXARGC], *iargs[MAXARGC];
	int   narg, niarg;
	char	cmd[4096];
	char	line[MAX_LINE_LEN], iline[MAX_LINE_LEN];
	char	timeStamp[32];
	char	*info, *faultCh, *ptr;
	long measCode;
	double measVal, newSlot, oldSlot = -1.0;
	long aMod;
	long aLine;
	long bMod;
	long bLine;
	long testLink;
	long fault, newCycle, oldCycle = -1.0;
	char	callSide[4], *callSideArray[256][256][4];
	long	callId, callIdArray[256][256];
	int	i, j, xi, yi;
	int n40 = 0, nRest = 0;
	int	fullTime, call40Time = 0, callRestTime = 0, callTime;
	int	oldLvl;
	long	retval;
		
  printf("destination : %s\n", destinationFile);

	if ((destFile = fopen(destinationFile, "w")) == NULL) {
		printf("could not open : %s\n", destinationFile);
		return;
	}

 	for (i=32; i<=256; i++) {
      ch = i;
      printf("%d %x\n", i, ch);
      sprintf(buf, "%d ", i);
      putc(buf[0], destFile);
      putc(buf[1], destFile);
      putc(buf[2], destFile);
      putc(' ', destFile);
      putc(ch, destFile);
      putc('\n', destFile);
  }
	 
  /* Close the files */
  fclose(destFile);
		
} /* End of CreateFile */

/* ----------------------------------------------------------------------------
 * main -- Main program for TecoCleanUp.
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
	
	CreateFile(argv[1]);
	
	exit(0);
	
} /* End of main */

/* End of TecoCleanUp.c */
