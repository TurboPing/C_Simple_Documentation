/* runnertest.c */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "runner.h"

int main(void) {

  int id;
  Runner* runner;
  Runner** runnerPtr;

  setRunner(0, "Guido", "Roelli", 'A');  
  setRunner(1, "Fritz", "Meier", 'B');  
  setRunner(2, "Michael", "Brand", 'C');  
  setRunner(3, "Peter", "Menzi", 'D');  
  setRunner(4, "Felix", "Egli", 'E');  
  setRunner(5, "Andreas", "Tobler", 'A');  
  setRunner(6, "Igor", "Jerkic", 'B');  
  setRunner(7, "Julie", "Engel", 'C');  
  setRunner(8, "Dennis", "Cavegn", 'D');  
  setRunner(9, "Hans", "Muster", 'E');  

  setTime(8, 2, 123);
  setTime(1, 1, 321);
  setTime(3, 0, 555);

  printf("%s\n\n", getRunnerTitleLine());

  runnerPtr = getAllRunner();
  while (*runnerPtr != NULL) {

    printf("%d\t%s\t\t%s\t\t%c\t%d\t%d\t%d\n", 
           getIdPtr(*runnerPtr),
           getFirstnamePtr(*runnerPtr),
           getLastnamePtr(*runnerPtr),
           getCategoryPtr(*runnerPtr),
           getTime(*runnerPtr, 0),
           getTime(*runnerPtr, 1),
           getTime(*runnerPtr, 2));

    *runnerPtr++;
  }

  return 0;
  
}
