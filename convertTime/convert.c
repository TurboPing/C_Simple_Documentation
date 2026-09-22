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

  timeVal = time(NULL);

  for (i=0;i<100;i++) {
  
     sprintf(asciiTime, "%s", ctime(&timeVal));

	   printf("day:%d|unixtime:%d|asciitime:%s\n", 
	   	i, timeVal, asciiTime);
  
     timeVal = timeVal + 86400;   
  
  }
  exit(0);
}
