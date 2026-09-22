/* fstat.c */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>


int main()
{

  int state;
  struct stat buf;
  
  state = stat("/home/roelli/c_code/fstat/guido", &buf);

  printf("state of /home/roelli/c_code/fstat/guido : %d\n", state);
  
  state = stat("guido", &buf);

  printf("state of guido : %d\n", state);
  
  state = stat("/unknown/dir/gugusFile", &buf);

  printf("state of /unknown/dir/gugusFile : %d\n", state);  
  
}
