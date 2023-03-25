/* swap.c */

#include <stdio.h>
#include <netinet/in.h>
#include <inttypes.h>

typedef struct Frame {
  short type;
  short len;
  int data[10];
} Frame;

swap16(char *buf) {

  char temp;

  if ( ntohl(1) == 1 ) {
    temp = buf[0];
    buf[0] = buf[1];
    buf[1] = temp;
  }

}

swap32(char *buf) {

  char temp;

  if ( ntohl(1) == 1 ) {
    temp = buf[0];
    buf[0] = buf[3];
    buf[3] = temp;
    temp = buf[1];
    buf[1] = buf[2];
    buf[2] = temp;
  }
  
}

int main()
{

  Frame *frame;

  int i;
  char buf[100];

  for (i=0; i<20; i++) {
    buf[i] = 'A'+ i;  
  }
  buf[20] = '\0';
  printf("%s\n", buf);

  frame = (Frame *)buf;

  swap16(&buf[2]);

  swap16((char *)&frame->type);

  swap32((char *)&frame->data);

  swap32((char *)&frame->data[2]);

  printf("%s\n", buf);

}
