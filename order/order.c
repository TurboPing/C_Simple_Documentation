/* order.c */

#include <stdio.h>

int main()
{

  int a;
  int b;
  int result;

  a = 10;
  b = 30;

  result = a++ + ++b;

  printf("a:%d|b:%d|result:%d\n\n", a, b, result);
  
}
