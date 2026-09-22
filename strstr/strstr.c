/* strstr.c */

#include <stdio.h>

int main()
{

  char *ptr;
  char string[100];

  sprintf(string, "c:\\dir1\\dir2/dir3\\usr/vxsim\\dir6/filename.ascii");
  printf("string is %s\n", string);

  if (strstr(string, "vxsim") != NULL) {
    printf("vxsim found\n");
  }
  else {
    printf("vxsim not found\n");
  }
  
  if (strstr(string, "roelli") != NULL) {
    printf("roelli found\n");
  }
  else {
    printf("roelli not found\n");
  }

}
