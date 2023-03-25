/* intValue.c */

#include <stdio.h>
#include <limits.h>

int main()
{

  unsigned int uintValue;
  int intValue;

  uintValue = 3860000000;
  intValue = uintValue;

  printf("INT_MAX:%d\n", INT_MAX);

  printf("uintValue:%u|intValue:%d\n", uintValue, intValue);

  if (intValue < 0) {
    printf("uintValue:%u|intValue:%d\n", uintValue, intValue);
  }

  uintValue = 3860;
  intValue = uintValue;

  if (intValue > INT_MAX) {
    printf("uintValue:%u|intValue:%d\n", uintValue, intValue);
  }

}
