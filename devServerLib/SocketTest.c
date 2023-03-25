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
#include <signal.h>
#include <sys/wait.h>

#include <a8620types.h>
#include <AhList.h>
#include <LogLib.h>
#include <SocketLib.h>


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

  while (cc = SvConnRead(fd, buf, sizeof buf)) {
    if (cc < 0)
      LogQuit("echo read\n");

    buf[cc]=0;
    printf("TCPechod1 ->  %s",buf);
    
    sleep(1);

    if (SvConnWrite(fd, buf, cc) < 0)
      LogQuit("echo write\n");
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

  while (cc = SvConnRead(fd, buf, sizeof buf)) {
    if (cc < 0)
      LogQuit("echo read\n");

    buf[cc]=0;
    printf("TCPechod2 ->  %s",buf);

    sleep(1);
    
    if (SvConnWrite(fd, buf, cc) < 0)
      LogQuit("echo write\n");
  }
  return 0;
}

/*------------------------------------------------------------------------
 * Timeout callBack Routine
 *------------------------------------------------------------------------
 */
int
timeoutCallback()
{
    /* code by timeout expired */
    printf("\n>>>>>> SvConnSelect timeout begin <<<<<<<<\n\n");
    sleep(3);
    printf("\n>>>>>> SvConnSelect timeout end <<<<<<<<<<\n\n");
}

/*------------------------------------------------------------------------
 * reaper - clean up zombie children
 *------------------------------------------------------------------------
 */
int
reaper()
{
	int	status;

        printf("child reaper\n");

	while (wait3(&status, WNOHANG, (struct rusage *)0) >= 0)
		/* empty */;
}


/*------------------------------------------------------------------------
 * Main 
 *------------------------------------------------------------------------
 */


main()
{

  struct timeval timeout;
  AhList serviceList;        

  (void) signal(SIGCHLD, reaper);

  serviceList = SvConnCreateList();

  /* list, service, TCP, callBack */        
  SvConnAppendService(serviceList,"1234",1, TCPechod1);
   
  SvConnAppendService(serviceList,"1235",1, TCPechod1);
        
  SvConnAppendService(serviceList,"1236",1, TCPechod1);

  SvConnAppendService(serviceList,"1237",1, TCPechod2);
  
  SvConnAppendService(serviceList,"1238",1, TCPechod2);
  
  timeout.tv_sec = 5;
  timeout.tv_usec = 0;

  SvConnSelect(serviceList, &timeout, timeoutCallback);     
  
  /* not reached  */

}

