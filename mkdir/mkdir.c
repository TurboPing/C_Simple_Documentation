/* mkdir.c */

#include <stdio.h>
#include <dirent.h>                /* Directory */
#include <sys/stat.h>
#include <sys/errno.h>

#define MODE_RESULTS_DIRECTORY     (S_IREAD|S_IWRITE|S_IEXEC|\
                                    S_IRGRP|S_IXGRP|\
                                    S_IROTH|S_IXOTH)
extern int errno;

int main()
{

  int res;
  char resFile[100];
  
  sprintf(resFile, "/home/roelli/gugus/gugus/gugus");  
  if (res=mkdir(resFile,MODE_RESULTS_DIRECTORY)!=0) {
	  printf("not able to create dir: %s|result:%d:errno:%d\n\n", 
	  	resFile, res, errno);
  }
  else {
	  printf("create dir : %s|result:%d\n\n", resFile, res);  
  }
  
}
