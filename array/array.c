/* printf.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int i;

SplitHostname(char *hostname, char *name1[0]) {
  
    for (i = 0; i < 10; i++) {

      printf("name1[%d] len : %2d | string : %s\n", i, strlen(name1[i]), name1[i]);

    }

}

int main()
{
    char hostname[100];
    char name1[10][100];

    sprintf(hostname, "%s", "host1|host2|host3");

    for (i = 0; i < 10; i++) {
      name1[i][0] = 0;
    }

    SplitHostname(hostname, name1[0]);

    sprintf(name1[0], "%s", "host1");
    sprintf(name1[1], "%s", "host2");
    sprintf(name1[2], "%s", "host3 a longer host name");

    for (i = 0; i < 10; i++) {

      printf("name1[%d] len : %2d | string : %s\n", i, strlen(name1[i]), name1[i]);

    }

}
