/* pid.c */

#include <stdio.h>

int main()
{

  int pid;
  int ppid;
  char name[100];
  
  pid = getpid();
  ppid = getppid();
  gethostname(&name, 100);
  
  printf("pid:%d|ppid:%d|name:%s\n", pid, ppid, name);
  
  
}
