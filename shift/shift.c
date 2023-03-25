/* shift.c */

#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>

#include <time.h>

#define  FLAG1   (1 << 0)
#define  FLAG2   (1 << 1)
#define  FLAG3   (1 << 2)
#define  FLAG4   (1 << 3)
#define  FLAG5    (1 << 4)

/* ----------------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
  printf("FLAG1:%d|FLAG2:%d|FLAG3:%d|FLAG4:%d|FLAG5:%d\n\n", 
	FLAG1,
	FLAG2,
	FLAG3,
	FLAG4,
	FLAG5);
  return 0;
}
