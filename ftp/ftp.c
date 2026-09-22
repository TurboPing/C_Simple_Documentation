/* ftp.c */

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <sys/param.h>
#include <errno.h>
#include <unistd.h>

#define TMPPATH		"/usr/tmp"
#define LOGFILE		"worker.ftp.log"
#define ERRORFILE	"worker.error.log"
#define FTPFILE		"ftpfile"
#define FTPPUTFILE      "ftpputfile"
#define FTPSYNCFILE	"ftpsyncfile"
#define FTPGETFILE	"ftpgetfile"

static int MtuCreateFtpFile(char *srcfile, int mode) {

    FILE *fftp;

	printf("MtuCreateFtpFile\n");

    if ((fftp = fopen( srcfile, "w" )) == NULL) {
        printf("open of ftp source file failed (%m)\n",
		  errno);
        return -1;
    }

    switch (mode) {
      case 0 :
      
    fprintf(fftp, "put tests/_def.ascii _def.ascii\n");
    fprintf(fftp, "put tests/mtu01.ascii mtu01.ascii\n");

      	break;
      
      case 1 :

    fprintf(fftp, "put tests/time.syn time.syn\n");

      	break;
      
      case 2 :

    fprintf(fftp, "bin\n");
    fprintf(fftp, "get mtu01.res results/mtu01.res\n");

      	break;
      
      default:

    fprintf(fftp, "");

      	break;
      
    }

    fclose(fftp);

}

static int MtuCreateRcFile(char *rcfile, char *srcfile) 
{
    FILE    *fftp;
    FILE    *fsrc;
    char     line[1000];
    char src[100];
    char dst[100];
    char    *dot;
    char    *dotdot;
    char     first;

    printf("MtuCreateRcFile %s %s\n", rcfile, srcfile);
    
    if ((fftp = fopen( rcfile, "w" )) == NULL) {
        printf("open of ftprc file failed (%m)\n",
		  errno);
        return -1;
    }

    /* Get files to be copied */

    if ((fsrc = fopen( srcfile, "r" )) == NULL) {
        printf("open of %s failed (%m)\n",
		  srcfile,
		  errno );
        fclose( fftp );
        return -1;
    }

    /* Generate ftp commands */

    fputs( "type ascii\n", fftp );
    fputs( "verbose\n", fftp );
    fputs( "prompt\n", fftp );

    /* read src and write rc file */
    
    while (fgets(line, 1000, fsrc)) {
      printf("source file : %s", line);
      fprintf( fftp, "%s", line);
    }

    /* Finish */

    fputs( "bye\n", fftp );

    /* Done, close and exit */

    if ((fclose( fsrc ) == EOF) || (fclose( fftp ) == EOF)) {
        printf("fclose failed\n" );
        return -1;
    }

    else {
      return 0;
    }
}

static int MtuDoFtp(char *rcfile)
{
    int ftppid;
  int res;
  int st;
      
  char logfile[100];
  char errorfile[100];

    struct stat  sbuf;
    char         ftpcmds[1000];
    void         (*oldhandler)( int );

    /* Check rc file presence */

    if (access( rcfile, R_OK ) < 0) {
        printf("ftprc file not found/readable\n" );
        return -1;
    }

    /* Construct file names and ftp command string */

    sprintf(logfile, "%s/%s%d%s", 
    	TMPPATH, "testname", (int)getpid(), LOGFILE);
    sprintf(errorfile, "%s/%s%d%s", 
    	TMPPATH, "testname", (int)getpid(), ERRORFILE);
/*  sprintf( logfile.name, "%s/%s%d%s",
	     lepath->name,
	     lefile,
	     (int)getpid(),
	     LOGFILE );
    sprintf( errorfile.name, "%s/%s%d%s",
	     lepath->name,
	     lefile,
	     (int)getpid(),
	     ERRORFILE );
*/
    sprintf( ftpcmds, "exec ftp %s < %s 1> %s 2> %s ",
	     "mtu01", rcfile, logfile, errorfile);

    printf("ftp command is : %s\n", ftpcmds);

    /* Fork shell/ftp process */

    ftppid = fork();

    if (ftppid < 0) {
        printf("cannot fork (%m)\n",
		  errno);
        return -1;
    }

    /* Child execs ftp */

    if (ftppid == 0) {
        execl( "/bin/sh", "sh", "-c", ftpcmds, NULL );
        printf("cannot exec /bin/sh (%m)\n", errno );
        exit(-1);
    }

    /* Parent waits for termination */

    if (ftppid > 0) {

      printf("pid %d forks ftp process (pid %d)\n",
	       getpid(),ftppid);


  while (1) {
    res = waitpid(ftppid, &st, 0 );

    if (res == ftppid) { 
      /* ok; this is the pid we waited for */
      printf("pid %d reaps exit status of child process (pid %d)\n",
      	getpid(),ftppid);
      break;
    } else if ((res == -1) && (errno != EINTR)) {
      /* error condition (maybe the exit status was reaped by a signal 
	 handler) --> break */
      break;
    }
  }

  printf("status of ftp is : %d\n", st);
    
    /* Check for errors */

    if ((stat( errorfile, &sbuf ) == 0) && (sbuf.st_size == 0)) {
      /* 
      unlink(errorfile);
      unlink(logfile);
      */
      return 0;
    } 
    else {
      printf("ftp not successful: see %s and %s",
	errorfile, logfile);
      return -1;
    }

  }
  
}

int main () {

	MtuCreateFtpFile(FTPPUTFILE, 0);
	MtuCreateRcFile(FTPFILE, FTPPUTFILE);
	MtuDoFtp(FTPFILE);
	unlink(FTPPUTFILE);
	unlink(FTPFILE);

	MtuCreateFtpFile(FTPSYNCFILE, 1);
	MtuCreateRcFile(FTPFILE, FTPSYNCFILE);
	MtuDoFtp(FTPFILE);
	unlink(FTPSYNCFILE);
	unlink(FTPFILE);

	MtuCreateFtpFile(FTPGETFILE, 2);
	MtuCreateRcFile(FTPFILE, FTPGETFILE);
	MtuDoFtp(FTPFILE);
	unlink(FTPGETFILE);
	unlink(FTPFILE);

}
