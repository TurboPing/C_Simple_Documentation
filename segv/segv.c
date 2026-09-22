/* segv.c */

#include <stdio.h>

int proc1 (char *string) {

  printf("i'm in proc 1\n");

  printf("string is %s\n", string);

  printf("am i still alive\n");

}


int main()
{
  int i;
  char *string = NULL;

  i = 10;

  proc1(string);

  
}
