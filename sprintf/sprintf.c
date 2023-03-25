/* sprintf.c */

#include <stdio.h>
#include <string.h>

int main()
{


#define ASCII "ascii"

  int bufsize;
  char buf[100]; 

  sprintf(buf, "first:%s"ASCII"second:%s", "part1", "part2");
  bufsize = (int)strlen(&buf[0]);

  printf("buf:%s\n", buf);

}
