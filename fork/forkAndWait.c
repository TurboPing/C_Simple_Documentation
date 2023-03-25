/* fork.c */

#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

int main()
{
  int pid;
  int status;
  int ret;
  int child_pid;
		
  if ((pid = fork()) < 0) {
    printf("Could not fork"); 
    return -1;
  }
  else {
    if (pid == 0) {
      printf("We are in the child pid:%d|getpid:%d|getppid:%d\n", pid, getpid(), getppid());

      sleep(10);

      printf("child exit\n");

      return(99);

    } else {

      printf("we are in the parent pid:%d|getpid:%d|getppid:%d\n", pid, getpid(), getppid());

      sleep(15);

      child_pid = wait(&status);

      if (status == 0) {
        printf("ok status : %d\n", status);
      } else {
        if (WIFEXITED(status)) {
          printf("exit status : %d\n", WEXITSTATUS(status));
        }

        if (WIFSIGNALED(status)) {
          printf("signal status : %d\n", WTERMSIG(status));
        }
      }

      sleep(10);

      printf("parent exit\n");

    }

  }

}
