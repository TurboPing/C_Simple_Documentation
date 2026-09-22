/* ----------------------------------------------------------------------------
 * MainSock.c -- Module for testing the Socket Library.
 * ----------------------------------------------------------------------------
 *
 * (c) 1995 Alcatel STR, Zuerich, Switzerland.
 *
 * ----------------------------------------------------------------------------
 */
 
#include <stdio.h>
#include <sys/time.h>
#include "a8620types.h"
#include "AhList.h"
#include "SvConn.h"


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
 * Main 
 *------------------------------------------------------------------------
 */


main()
{

  struct timeval timeout;
  AhList serviceList;        


  serviceList = SvConnCreateList();

  /* list, service, TCP, callBack */        
  SvConnAppendService(serviceList,"1234",1, TCPechod1);
   
  SvConnAppendService(serviceList,"1235",1, TCPechod1);
        
  SvConnAppendService(serviceList,"1236",1, TCPechod1);

  SvConnAppendService(serviceList,"1237",1, TCPechod2);
  
  SvConnAppendService(serviceList,"1238",1, TCPechod2);
  
  timeout.tv_sec = 10;
  timeout.tv_usec = 0;

  while (1) {

    SvConnSelect(serviceList, &timeout);
    
    /* code by timeout expired */
    printf("\n>>>>>> SvConnSelect timeout begin <<<<<<<<\n\n");
    sleep(3);
    printf("\n>>>>>> SvConnSelect timeout end <<<<<<<<<<\n\n");

  }     
  
  /* not reached  */

}

