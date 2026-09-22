/* mkdir.c */

#include <stdio.h>
#include <dirent.h>                /* Directory */
#include <sys/stat.h>

#define MODE_RESULTS_DIRECTORY     (S_IREAD|S_IWRITE|S_IEXEC|\
                                    S_IRGRP|S_IXGRP|\
                                    S_IROTH|S_IXOTH)

int main()
{

  char moveFile[100];
  char movedFile[100];
  
  sprintf(moveFile, "/home/roelli/c_code/guidoToMove");  
  sprintf(movedFile, "/home/roelli/c_code/guidoMoved");  
  if (rename(moveFile, movedFile)!=0) {
	  printf("not able to move file : %s\n\n", moveFile);
  }
  else {
	  printf("move file to : %s\n\n", movedFile);  
  }
  
}
