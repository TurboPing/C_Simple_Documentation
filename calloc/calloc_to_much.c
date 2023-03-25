/* calloc.c */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int i;
char name[1000000];
char *namePtr;

proc1(char name[1000000000]) {

  char name1[1000000000];

}

int main()
{

  proc1(&name[1000000]);

  sleep(10);

  for (i=0; i<100000; i++) {

    namePtr = calloc (1, sizeof(name));
    printf("i:%d|errno:%d\n", i, errno);

    // sleep(2);

    // free(namePtr);
  
  }

}
