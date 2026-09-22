/* sscanf.c */

#include <stdio.h>

int main()
{

  char buf[100];
  int varCount;
  char strings[10][200];

  sprintf(buf, "%s", "mtu01|6543|MTU3-tone|1.3.g|MTU|LAN-WAN");

  if (varCount = sscanf(buf, "%s|%s|%s|%s|%s|%s|%[^\n]", 
      	&strings[0][0], &strings[1][0], &strings[2][0], 
      	&strings[3][0], &strings[4][0], &strings[5][0]) > 0) {

    printf("%s %s %s %s %s %s", strings[0][0], strings[1][0], strings[2][0], 
      	strings[3][0], strings[4][0], strings[5][0]);

  }

  
}
