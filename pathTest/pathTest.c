/* ----------------------------------------------------------------------------
 * pathTest.c 
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

#include <sys/times.h>
#include <limits.h>

#include <netinet/in.h>
#ifdef SYSV
#	include <limits.h>
#	include <stropts.h>
#else
#	include <sys/param.h>
#endif

typedef struct {
  char       name[1000];
} FileName;

int CreatePath(FileName *rootPath)
{

  sprintf(rootPath->name, "%s %s", "test", "path");
	return 0;

}

/* ----------------------------------------------------------------------------
 * main -- Main Program
 *
 */
int
main(int argc, char **argv)
{

	FileName rootPath;
	
	CreatePath(&rootPath);
	printf("%s\n", rootPath.name);

	return(0);
					
}
/* End of main */
