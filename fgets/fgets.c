#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  
  char line[1000];
  char *returnCode;
  FILE *fd;

  char delim[100] = ": #\t$";
  int i;
  char *ptr[10];

  char *hostname;
  char *application;

  if ((fd = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "Konnte file nicht öffnen!\n");
    exit(1);
  }

  while (fgets(line, 1000, fd) != NULL) {

    printf("=========================================================\n");

    printf("len:%3d|%s", strlen(line), line);

    i = 0;
    hostname = '\0';
    application = "";

    ptr[i] = (char *)strtok(&line[0], delim);
    while (ptr[i] != NULL) {

      printf("ptr[%d]:%s\n", i, ptr[i]);

      switch(i) {
        case 0:    hostname = ptr[i];
          break;

        case 1:    application = ptr[i];
          break;

        default:
          break;


      }

      i++; 

      ptr[i] = (char *)strtok(NULL, delim);
 
    }

    if ((strcmp(hostname, "") != 0) &&
        (strcmp(application, "") != 0)) {

      printf("hostname:%s\n", hostname);
      printf("application:%s\n", application);

      

      /* call ConfigGetValue */

    }

  }

  return 0;

}
