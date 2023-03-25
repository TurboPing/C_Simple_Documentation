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
  static struct tm	tm1;
  struct tm	*tm0;

  timeVal = time(NULL);

  for (i=0;i<200;i++) {
  
     sprintf(asciiTime, "%s", ctime(&timeVal));

	   printf("day:%d|unixtime:%d|asciitime:%s\n", 
	   	i, timeVal, asciiTime);
  
     timeVal = timeVal + 86400;   
  
  }

  for (i=0;i<30;i++) {

    timeVal = time(NULL);
    tm0 = (struct tm *) localtime(&timeVal);
    tm1.tm_sec = 0;
    tm1.tm_min = 0;
    tm1.tm_hour = 0;
    tm1.tm_mday = i;
    tm1.tm_mon = 10;
    tm1.tm_year = 2002 - 1900;
    tm1.tm_wday = (i + 1) % 7;
    tm1.tm_isdst = tm0->tm_isdst;
   
    printf("date:%02d:%02d:%02d %02d.%02d.%02d\n",
	   tm1.tm_mday, tm1.tm_mon, tm1.tm_year,
	   tm1.tm_hour, tm1.tm_min, tm1.tm_sec); 
  
  }
  
  return 0;
}
