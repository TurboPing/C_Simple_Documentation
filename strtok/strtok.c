/* strtok.c */

#include <stdio.h>

int main()
{

  char delim[100] = ": #\t";
  char *ptr;
  char string[100];

  sprintf(string, "    \t  string1:string2#\t#  :string3:####  string4");
  printf("string:%s\n", string);

  ptr = (char *)strtok(&string[0], delim);
  while (ptr != NULL) {

    printf("ptr:%s\n", ptr);

    ptr = (char *)strtok(NULL, delim);

  }

}
