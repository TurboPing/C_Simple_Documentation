/* time.c */

#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#define TIME_LEN        100
#define TIME_FORMAT     "%d-%m-%Y %H:%M:%S"
#define FORMAT_LEN      50   

#define LOOPS    500000000

int printtime() {

  time_t actualTime;
  struct   tm  * tm;
  char         timeBuffer[TIME_LEN];
  static char  timeFormat[FORMAT_LEN] = TIME_FORMAT;

  actualTime = time(NULL);

  if( (tm = localtime(&actualTime)) == NULL) {
    return -1;
  }

  if( strftime(timeBuffer, TIME_LEN, timeFormat, tm) == 0) {
    return -1;
  }
  
  printf("Time is %s\n", timeBuffer);

  return 0;

}

int main(int argc, char ** argv)
{

  int i;
  time_t actualTime;
  struct timeval now;		/* time (sec/usec since epoch) */

  printf("\ncall time : %d times\n\n", LOOPS);
  printtime();

  for (i=0; i < LOOPS; i++) {
    actualTime = time(NULL);
  }
  printtime();

  printf("--------------------------------------\n");

  printf("\ncall timeofday : %d times\n\n", LOOPS);
  printtime();

  for (i=0; i < LOOPS; i++) {
    gettimeofday(&now, NULL);
  }
  printtime();
  
}
