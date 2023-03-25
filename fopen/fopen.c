/* fopen.c */

#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>

#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/file.h>

#include <sys/times.h>
#include <limits.h>

#define FILE_MAX 1000
#define LINE_LEN 10000

int main(int argc, char **argv) {
  int i;
  FILE *fd;
  int fdup;
  int fdup2;
  int fdup3;
  int flags, modus;

  int numberOfBytes = 0;
  int nbytes;
  char line[10000];

  if(argc < 2) {
      fprintf(stderr, "Usage: %s DATEI\n", *argv);
      exit(EXIT_FAILURE);
  }

  if ((fd = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "Konnte %s nicht öffnen!\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  while (!feof(fd)) {
    nbytes = fread(line, 1, LINE_LEN, (FILE *)fd);
    numberOfBytes += nbytes;
    printf("nbytes:%d|%d\n", nbytes, numberOfBytes);
  }

}

