#include <stdio.h>
#include <stdlib.h>

int main() {

  FILE *file;
  char buffer[1000];
  int ret;
  char *ret_char;

  file = fopen("input.txt", "r");

  printf("fopen returns %d\n", file);
  
  while (((ret = feof(file)) == 0) && 
         ((ret_char = fgets(buffer, 1000, file)) != NULL)) {
    printf("ret:%d|ret_char:%s|line:%s", ret, ret_char, buffer);
  }

  printf("ret:%d|ret_char:%s\n", ret, ret_char);

  ret_char = fgets(buffer, 1000, file);
  ret = feof(file);

  printf("ret:%d|ret_char:%s\n", ret, ret_char);

  return 0;

}
