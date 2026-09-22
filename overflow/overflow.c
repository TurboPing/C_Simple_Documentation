/* atoi.c */

#include <stdio.h>

int main()
{

  unsigned int ctr;

  unsigned int ctr1;
  unsigned int ctr2;


  ctr = 100;
  ctr2 = 200;

  ctr += ctr2;
  if (ctr2 > ctr) {
	printf("overflow\n");
  }					

  printf("ctr2:%d|ctr:%d\n\n", ctr2, ctr); 

  ctr = 4000000000;
  ctr2 = 2000000000;

  ctr += ctr2;
  if (ctr2 > ctr) {
	printf("overflow\n");
  }  					

  printf("ctr2:%d|ctr:%d\n\n", ctr2, ctr);
  
}
