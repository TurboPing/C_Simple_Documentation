/* calloc.c */

#include <stdio.h>

int main()
{

  int i;
  char name[100000];
  char *namePtr;

  for (i=0; i<100000; i++) {

    namePtr = calloc (1, sizeof(name));

    sleep(2);

    free(namePtr);
  
  }

}
