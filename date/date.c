/* fork.c */

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

	time_t  Time;
	
                time(&Time);
                printf("%s\n", asctime(&Time));

}
