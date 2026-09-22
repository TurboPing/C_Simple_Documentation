/* putenv.c */

#include <stdio.h>
#include <stdlib.h>

int main()
{

  int pid;
  int ppid;
  char name[100];
  char putenvString[100];
  
  sprintf(putenvString, "%s", "envVar1=value1");
  putenv(putenvString);  

  printf("envVar1=%s\n", getenv("envVar1"));

  putenv("envVar1=valueChanged");  

  printf("envVar1=%s\n", getenv("envVar1"));
    
  putenv("envVar1=valueChanged2");  

  printf("envVar1=%s\n", getenv("envVar1"));

  putenv("envVar1");  

  printf("envVar1=%s\n", getenv("envVar1"));

  putenv("envVar1=");  

  printf("envVar1=%s\n", getenv("envVar1"));

  putenv("envVar1=new=");  

  printf("envVar1=%s\n", getenv("envVar1"));

}
