/* ----------------------------------------------------------------------------
 * pointer.c
 * ----------------------------------------------------------------------------
 */

/* ----------------------------------------------------------------------------
 * INCLUDES
 */
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>

#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/file.h>

#include <netinet/in.h>
#ifdef SYSV
#	include <limits.h>
#	include <stropts.h>
#else
#	include <sys/param.h>
#endif


static
int 
buffer_alloc(char **buffer) {

  char *ptr;

  ptr = (void *)malloc(1000);

  sprintf(ptr, "%s", "hallo");

  *buffer = ptr;

  return 0;

}


int
main(int argc, char **argv)
{

  char *buffer_ptr;

  buffer_alloc(&buffer_ptr);

  printf("buffer_ptr : %s\n", buffer_ptr);
	
  exit(0);
	
}
