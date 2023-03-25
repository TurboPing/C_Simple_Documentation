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

	fprintf(stderr, "Usage: tab source_file destination_file Nr._pos Seite_pos\n");
	
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
CleanUp(char *sourceFile, char *destinationFile, char *Nr, char *Seite)
{
  int   posNumber;
  int   posSeite;
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
	char	line[MAX_LINE_LEN];
	char  outline[MAX_LINE_LEN];
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
		
  posNumber = atoi(Nr);
  posSeite = atoi(Seite);
  printf("pos number : %d / pos seite : %d\n", posNumber, posSeite);
  printf("source : %s / destination : %s\n", sourceFile, destinationFile);

	/* Open the filled result file */
	if ((srcFile = fopen(sourceFile, "r")) == NULL) {
		printf("could not open : %s\n", sourceFile);
		return;
	}
	if ((destFile = fopen(destinationFile, "w")) == NULL) {
		printf("could not open : %s\n", destinationFile);
		return;
	}
	nLines = 0;
	/* count the line of file : srcFile */
 	while (fgets(line, MAX_LINE_LEN, srcFile) != NULL) { 
				i = 0;
        while (line[i] != '\n') {
					if ((line[i] == 'N') && (line[i+1] == 'r') && (line[i+2] == '.')) {
             if (i <= posNumber) {
               for (j=0; j<(posNumber - i); j++) {
                 putc(' ', destFile);        
               }             
             }
          }
					if ((line[i] == 'S') && (line[i+1] == 'e') && (line[i+2] == 'i') &&
					    (line[i+3] == 't') && (line[i+4] == 'e') && (line[i+5] == ' ')) {
             if (i <= posSeite) {
               printf("i:%d/seite:%d",i,posSeite);
               for (j=0; j<(posSeite - i); j++) {
                 putc(' ', destFile);        
               }             
             }
          }
          ch = line[i];
          putc(ch, destFile);
        	i++;
        }
        ch = '\n';
        putc(ch, destFile);
 		  	nLines ++;
  }
  printf("\nnumber of lines %d\n", nLines);	
	 
  /* Close the files */
  fclose(srcFile); 
  fclose(destFile);
		
} /* End of CleanUp */

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
	
	CleanUp(argv[1], argv[2], argv[3], argv[4]);
	
	exit(0);
	
} /* End of main */

/* End of TecoCleanUp.c */
