/* acess.c */

#include <stdio.h>
#include <unistd.h>

int main()
{

  
  int accessState;

  char commandPath[1000];

  sprintf(commandPath, "%s/%s", "/package/ergon/tms/0/tms-server/bin", "server");

  accessState = access(commandPath, X_OK);
  if (accessState == -1) {
  
    printf("access %s NOK\n", commandPath);
  
  }
  else {
   
    printf("access %s OK\n", commandPath);
  }
  
  return 0;
  
}
