/* gethostid.c */

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
  unsigned int id;

  id = gethostid() + (getuid() << 16);
  
  printf("key is %u | gethostid is %u | getuid is %u | getuid << 16 is %u\n\n", 
  	(unsigned int)id, gethostid(), getuid(), (getuid() << 16));
}
