#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>

/* ----------------------------------------------------------------------------
 */
int
main(argc, argv)
int     argc;
char    *argv[];
{
  int i;

  char asciiTime[50];
  time_t timeVal;

  timeVal = 1035716100;

  sprintf(asciiTime, "%s", ctime(&timeVal));

  printf("unixtime:%d|asciitime:%s\n", 
	   	timeVal, asciiTime);
  
  exit(0);
}
