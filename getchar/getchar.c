/* getchar.c */

#include <stdio.h>

int main()
{

  char a[10];

  fgets(a, 2, stdin);
  printf("char a is %s\n", a);  
  
}
