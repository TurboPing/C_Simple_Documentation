/* getenv.c */

#include <stdio.h>
#include <stdlib.h>

int main()
{

  int i;
  char *envVarList[] = {
  	"HOME", "PATH",
        "USER", "PWD",
        "A8610GLOBAL_PATH",
        "LD_LIBRARY_PATH",
        "",
  };
  char *envPtr;

  for(i = 0; (strcmp((char *)envVarList[i], "") != 0); i++) {
    
    envPtr = getenv(envVarList[i]);
    if (envPtr != NULL) {
      printf("%s=%s\n", envVarList[i], getenv(envVarList[i]));
    }
          
  }

}
