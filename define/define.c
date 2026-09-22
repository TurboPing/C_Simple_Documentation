/* define.c */

#include <stdio.h>

#define getUserIdent()	57
#define schlafen(value)	sleep(value)
#define microschlafen(value)	usleep(value/10)

int main()
{

  int i;

  i = getUserIdent();
  printf("returnOne returns %d\n", i);

  printf("vor dem schlafen %d\n", time(NULL));
  schlafen(3);
  printf("nach dem schlafen %d\n", time(NULL));

  printf("vor dem microschlafen %d\n", time(NULL));
  microschlafen(38345273);
  printf("nach dem microschlafen %d\n", time(NULL));

}
