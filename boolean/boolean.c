/* boolean.c */

#include <stdio.h>

int main()
{

typedef enum {
	true = 1, false = 0
} Boolean;

typedef int BOOLEAN;

Boolean bool1;
BOOLEAN bool2;

bool1 = true;
bool2 = true;
printf("bool1 true = %d | bool2 true = %d\n", bool1, bool2);
  
bool1 = false;
bool2 = false;
printf("bool1 false = %d | bool2 false = %d\n", bool1, bool2);
  
}
