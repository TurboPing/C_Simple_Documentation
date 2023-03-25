/* remove.c */

#include <stdio.h>
#include <dirent.h>                /* Directory */
#include <sys/stat.h>
#include <sys/errno.h>

extern int errno;

int main()
{

  int res;
  char resFile[100];
  char dirFile[100];
  DIR *dirp;
  struct dirent *dirEntry;
  
  sprintf(dirFile, "/home/roelli/gugus");  
  dirp = opendir(dirFile);
  if (dirp != NULL) {
    printf("open dir :%s\n", dirFile);
  }  
  else {
    printf("can't open dir : %s\n", dirFile);
  }

  while ((dirEntry = readdir(dirp)) != NULL) {
  
    printf( "%s\n", dirEntry->d_name);
  
    if (strstr(dirEntry->d_name, ".res") != NULL) {
    
      sprintf(resFile, "%s/%s", dirFile, dirEntry->d_name);
      
      if (res=remove(resFile)!=0) {
	  printf("not able to remove files:%s|result:%d:errno:%d\n\n", 
	  	resFile, res, errno);
      }
      else {
	  printf("remove files:%s|result:%d\n\n", resFile, res);  
      }
    }
  
  }
  
  closedir(dirp);
  
}

