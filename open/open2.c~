/* open.c */

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
#define LINE_LEN 1000

int 
readLine(int fd, char *buf, int maxlinelen)
{
  int res;
  int readIndex;
  char buffer;

  readIndex = 0;
  while ((res = read(fd, &buffer, 1)) != 0) {
    buf[readIndex] = buffer;
    readIndex++;
    if ((buffer == '\n') || (readIndex >= maxlinelen)) {
      buf[readIndex] = '\0';
      break;
    }     
  }
  return readIndex;

}

int 
main(int argc, char **argv) {
  int i;
  int fd;
  int ret;
  char line[1000];

  if(argc < 2) {
      fprintf(stderr, "Usage: %s DATEI\n", *argv);
      exit(EXIT_FAILURE);
  }

  if ((fd = open("gugus.txt", O_RDONLY)) == NULL) {
    fprintf(stderr, "Konnte %s nicht öffnen!\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  printf("fopen fd[%d]:%d\n", i, (int)fd);

  while ((ret = readLine(fd, &line[0], LINE_LEN)) != 0) {
    printf("len:%d|line:%s", strlen(line), line);
  }

}

