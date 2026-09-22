/* ----------------------------------------------------------------------------
 * MainSock.c -- Module for testing the Socket Library.
 * ----------------------------------------------------------------------------
 *
 * (c) 1995 Alcatel STR, Zuerich, Switzerland.
 *
 * ----------------------------------------------------------------------------
 */
 
#include <stdio.h>
#include <signal.h>
#include "a8620types.h"
#include "AhList.h"
#include "SvConn.h"

  int j;

/*------------------------------------------------------------------------
 * TCPechod1 - echo data until end of file  service callBack Routine
 *------------------------------------------------------------------------
 */
int
TCPechod1(fd)
int	fd;
{
	char	buf[BUFSIZ];
	int	cc;

       	while (cc = read(fd, buf, sizeof buf)) {
		if (cc < 0)
			errexit("echo read\n");

                buf[cc]=0;
                printf("TCPechod1 ->  %s",buf);

                sleep(1);

		if (write(fd, buf, cc) < 0)
			errexit("echo write\n");
	}
	return 0;
}

/*------------------------------------------------------------------------
 * TCPechod - echo data until end of file service callBack Routine
 *------------------------------------------------------------------------
 */
int
TCPechod2(fd)
int	fd;
{
	char	buf[BUFSIZ];
	int	cc;

       	while (cc = read(fd, buf, sizeof buf)) {
		if (cc < 0)
			errexit("echo read\n");

                buf[cc]=0;
                printf("TCPechod2  -> %s",buf);

                sleep(1);

		if (write(fd, buf, cc) < 0)
			errexit("echo write\n");
	}
	return 0;
}


/*------------------------------------------------------------------------
 * Signal Handler 
 *------------------------------------------------------------------------
 */

void SigHandler (int signo)

{

  switch(signo) {
  case SIGALRM:
       printf("sig alarm\n");
       j=1;
       break;
  default:
       break;
  }

}


/*------------------------------------------------------------------------
 * Main 
 *------------------------------------------------------------------------
 */


main()
{

  int i;
  AhList L;        

  j=0;

  while(1) {

  /* handle signals */
  for(i=1;i<32;i++) {
    signal(i,SigHandler);
  }
  
     alarm(5);

     while (j==0){};
     j=0;

  }

  L = SvConnCreateList();
        
  SvConnAppendService(L,"1234",1, TCPechod1); /* list, service, TCP, callBack */
        
  SvConnDestroyList(L);

  L = SvConnCreateList();
        
  SvConnAppendService(L,"1234",1, TCPechod1); /* list, service, TCP, callBack */

  SvConnAppendService(L,"1235",1, TCPechod1);
        
  SvConnAppendService(L,"1236",1, TCPechod1);

  SvConnAppendService(L,"1237",1, TCPechod2);
  
  SvConnAppendService(L,"1238",1, TCPechod2);
  
  SvConnSelect(L);
       
  /* not reached  */

}

