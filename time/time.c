/* time.c */

#include <stdio.h>
#include <sys/time.h>

#define TIME_LEN        100
#define TIME_FORMAT     "%d-%m-%Y %H:%M:%S"
#define TIME_FORMAT2     "%H:%M:%S"
#define FORMAT_LEN      50   

int main(int argc, char ** argv) {

  time_t actualTime;
  struct   tm  * tm;
  char         timeBuffer[TIME_LEN];
  static char  timeFormat[FORMAT_LEN] = TIME_FORMAT;
  static char  timeFormat2[FORMAT_LEN] = TIME_FORMAT2;

  //printf("argv:%s", *argv[0]);
	
  actualTime = time(NULL);


  if( (tm =gmtime(&actualTime)) == NULL) {
    exit(1);
  }

  if( strftime(timeBuffer, TIME_LEN, timeFormat, tm) == 0) {
    exit(1);
  }

  printf("Time is %s\n", timeBuffer);

  actualTime = 4567;

  if( (tm = gmtime(&actualTime)) == NULL) {
    exit(1);
  }

  if( strftime(timeBuffer, TIME_LEN, "%H:%M:%S", tm) == 0) {
    exit(1);
  }
  
  printf("Time is %s\n", timeBuffer);

}
