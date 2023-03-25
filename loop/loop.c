/* loop.c */

#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>

#include <time.h>

/* ----------------------------------------------------------------------------
 */
int
main(argc, argv)
int     argc;
char    *argv[];
{

	int i;
	int limit;

	limit = 1;
	for (i=0; i<limit; i++) {
		printf("loop i:%d|limit:%d\n", i, limit);
	}

}
