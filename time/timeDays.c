/* time.c */

#include <stdio.h>
#include <time.h>

#define TIME_LEN        100
#define TIME_FORMAT     "%d-%m-%Y %H:%M:%S"
#define FORMAT_LEN      50   

int main(int argc, char ** argv) {

  int i;
  time_t actualTime;
  struct   tm  * tm;
  char         timeBuffer[TIME_LEN];
  static char  timeFormat[FORMAT_LEN] = TIME_FORMAT;

  for (i = 0; i < 200; i++) {
	
    actualTime = time(NULL) + (i * 86400);

    if( (tm = localtime(&actualTime)) == NULL) {
      return 1;
    }

    if( strftime(timeBuffer, TIME_LEN, timeFormat, tm) == 0) {
      return 1;
    }
  
    printf("Time is %s\n", timeBuffer);

  }

  return 0;

}
