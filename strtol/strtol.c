/* strtol.c */

#include <stdio.h>

int main()
{

  long val;
  char string[100];
  
  sprintf(string, "%s", "0");
  val = strtol(string, (char **)NULL, 16);
  printf("move value from : %s -> to : %x\n\n", string, val);  
  
  sprintf(string, "%s", "0ff");
  val = strtol(string, (char **)NULL, 16);
  printf("move value from : %s -> to : %x\n\n", string, val);  

  sprintf(string, "%s", "0X245");
  val = strtol(string, (char **)NULL, 16);
  printf("move value from : %s -> to : %x\n\n", string, val);  

}
