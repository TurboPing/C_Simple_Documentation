#include <stdio.h>

/*
 *
 */
int main(argc, argv) {

  printf("main\n");

  ConfigStartThreads();

  while (1) {

    sleep(10);

  }

  return 0;

}
