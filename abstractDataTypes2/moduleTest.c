#include <stdio.h>
#include <stdlib.h>

#include "module.h"

int main() {

  Handle handle;
  int time;

  handle = Open();

  Print(handle);

  Change(handle);

  ChangeName(handle, "newName");

  Print(handle);

  Create(handle, 100, 123);
  Create(handle, 200, 234);
  Create(handle, 300, 345);

  Print(handle);

  time = Get(handle, 200);
  printf("Get 200 returns time:%d\n", time);

  Close(handle);

  return 0;

}
