/* newline.c */

#include <stdio.h>
#include <string.h>

int main()
{

  char *ptr;
  char string[1000];

  sprintf(string, "%s", "das i st ein s tring mit new lin es");

  string[5] = '\n';
  string[14] = '\n';
  string[28] = '\n';
  string[32] = '\n';
  printf("string with newlines : \n%s\n\n", string);

  ptr = string;
  while (ptr != NULL) {
    ptr = strstr(ptr, "\n");
    if (ptr != NULL) {
      *ptr = ' ';
    }
  }

  printf("string fixed\n%s\n", string);

  return 0;
    
}
