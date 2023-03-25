/* fork.c */

#include <stdio.h>

int main()
{
  int pid;
  int status;
		
  if ((pid = fork()) < 0) {
    printf("Could not fork"); 
    return -1;
  }
  else {
    if (pid == 0) {
      printf("We are in the child pid:%d|getpid:%d|getppid:%d\n", pid, getpid(), getppid());

      sleep(10);

      printf("child exit\n");
    } else {

      printf("we are in the parent pid:%d|getpid:%d|getppid:%d\n", pid, getpid(), getppid());

      sleep(20);

      printf("parent exit\n");

    }

  }

  return 0;

}
