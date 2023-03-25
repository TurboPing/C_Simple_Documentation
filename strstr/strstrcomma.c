/* strstrcomma.c */

#include <stdio.h>

#define MAXARGC		50	


int
SplitLine(char *buf, char *sep, int *argc, char **argv) {

  char *ptr;

  printf("buf is %s\n", buf);

  *argc = 0; 
  argv[*argc] = buf;

  ptr = buf; 
  while (ptr != NULL) { 
 
    ptr=(char *) strstr(ptr, sep);

    if (ptr == NULL) {
      continue;
    }
       
    *ptr='\0';
    ptr++;
    (*argc)++;
    argv[*argc] = ptr;

  }

  (*argc)++;
  argv[*argc] = NULL;
    
}


int main() {

  int i;
  char buf[100];

  char	*argv[MAXARGC];
  int   argc;

  for (i=0; i<MAXARGC; i++) {
    argv[i] = NULL;
  }
  argc = 0;

  sprintf(buf, "this,is,a,comma,separated,string,,,,end");

  printf("decoded : %s\n", buf);

  if (SplitLine(buf, ",", &argc, argv) == -1) {
    printf("decoding error : %s", buf);
    return -1;
  }

  for (i=0; i<argc; i++) {
    printf("%d|%s\n", i, argv[i]);
  }

  return 0;

}
