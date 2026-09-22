/* setuid.c */

#include <stdio.h>
#include <stdarg.h>
#include <signal.h>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>

#include <sys/wait.h>
#include <sys/ioctl.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <rpc/types.h>
#include <rpc/rpc.h>

int main()
{
  int answer;

 
  struct passwd  *pswd;

  if ((pswd = getpwuid(getuid())) == NULL) {
    return -1;
  }
		
 
  printf("answer from getuid:%d|group:%d|name:%s\n\n", 
             getuid(), getgid(), (char *)pswd->pw_name);


  printf("getpid():%d\n", getpid());
  printf("getpgrp():%d\n", getpgrp());
  printf("getppid():%d\n", getppid());
  printf("getpgid():%d\n", getpgid(getpid()));

  sleep(30);


  return 0;

}
