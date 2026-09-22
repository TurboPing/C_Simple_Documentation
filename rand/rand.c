/* rand.c */

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
  int randomNumber;

  /* Initialize random number generator */
  srand((int)time(NULL));

  for (i=0; i<1000; i++) {

    randomNumber = rand() % 1000;
    printf("randomNumber:%d|rand():%d\n", randomNumber, rand());

    sleep(1);
    
  }

}
