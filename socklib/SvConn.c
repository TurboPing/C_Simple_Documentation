/* ----------------------------------------------------------------------------
 * SvConn.c -- Basic library module for socket handling.
 * ----------------------------------------------------------------------------
 *
 * (c) 1995 Alcatel STR, Zuerich, Switzerland.
 *
 * ----------------------------------------------------------------------------
 *
 * $Id: SvConn.c,v 1.4 1995/09/25 06:28:21 ----- Exp $
 *
 * Status : Release 1.0
 *
 * ----------------------------------------------------------------------------
 * Description
 *
 * This module implements socket as abstract data-types. The following
 * operations on socket are provided:
 *
 * SvConnCreateList      -- create a empty service list
 * SvConnDestroyList     -- destroy a service list
 * SvConnAppendService   -- append server service to list
 * SvConnSelect          -- select a connection request
 * SvConnRead            -- read 
 * SvConnWrite           -- write
 * SvConnClose           -- close
 *
 * ----------------------------------------------------------------------------
 */

static char *SvConn_c_rcsid = "$Id: SvConn.c,v 1.4 1995/09/25 06:28:21 herren Exp $";

/* ----------------------------------------------------------------------------
 * INCLUDES
 */
 
#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/errno.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <netinet/in.h>
  
#include <stdio.h>
#include "a8620assert.h"
#include "a8620types.h"
#include "AhList.h"

/* ----------------------------------------------------------------------------
 * DEFINES
 */
#define MAX_SELECT_FD  256

/* ----------------------------------------------------------------------------
 * TYPES
 */

typedef struct {
  char *serviceName;
  int serviceProt;
  int  serviceSock;
  int (*serviceFunc)();
} serviceDesc;

/* ----------------------------------------------------------------------------
 * GLOBAL VARIABLES
 */
Public int	SvConnErrNo;	/* Error number - not used */
Public char	*SvConnErrMsg;	/* Error message - not used */

extern int errno;

/* ----------------------------------------------------------------------------
 * LOCAL VARIABLES
 */

Private char	*SvConnErrors[2] = {        /* not used  */
	"No Error occured.",
	"Could not allocate memory."
};


/* ----------------------------------------------------------------------------
 * PROCEDURES
 */

/* Ein Element der Liste ausdrucken */
void print_service(char *text,  serviceDesc  *sP)
{

  printf("%s Name=%s, TCP=%d, Socket=%d\n", text, sP->serviceName, sP->serviceProt, sP->serviceSock);

}

/* ----------------------------------------------------------------------------
 * SvConnCreateList -- Create a new list.
 *
 * Arguments:     None.
 *
 * Returns:       
 *
 * Side Effects:  
 *
 * Algorithm:   
 *
 */
Public
AhList
SvConnCreateList()
{	
  AhList L;
  
  if((L = AhListCreate()) == NULL){
    fprintf(stderr, "could not create list\n");
    return((AhList)NULL);
  }
      
  return (L);
	
} /* End of SvConnCreateList */

/* ----------------------------------------------------------------------------
 * SvConnDestroyList -- Destroy a list.
 *
 * Arguments:     None.
 *
 * Returns:       
 *
 * Side Effects:  
 *
 * Algorithm:   
 *
 */
Public
Boolean
SvConnDestroyList(AhList list)
{	
  Boolean answer;
  serviceDesc  *serviceP;

  while (AhListNelem(list) > 0) {
    serviceP = AhListGetHead(list);

    print_service("SvConnDestroyList",serviceP);

    SvConnClose(serviceP->serviceSock);
    free(serviceP);
  }

  answer = AhListDestroy(list);   
  return (answer);
	
} /* End of SvConnDestroyList */

/* ----------------------------------------------------------------------------
 * SvConnAppendService  -- append server service
 *
 * Arguments:     list -- A valid AhList.
 *
 * Returns:     
 *
 * Side Effects:
 *
 * Algorithm:   
 *
 */
Public
Boolean
SvConnAppendService(AhList list, char *service, int prot, int (*callBack)())
{

  serviceDesc  *serviceP;
  
  if ((serviceP = (serviceDesc *)calloc(1, sizeof(serviceDesc))) == NULL) {
    
    return (false);
  }

  serviceP->serviceName = service;
  serviceP->serviceProt = prot;
  serviceP->serviceFunc = callBack;

  if (serviceP->serviceProt == 1) {
    serviceP->serviceSock = passiveTCP(serviceP->serviceName, 5);   /* passivesock/passiveTCP/passiveUDP/errexit */
  }
  else {
    serviceP->serviceSock = passiveUDP(serviceP->serviceName);   /* passivesock/passiveTCP/passiveUDP/errexit */
  }

  print_service("SvConnAppendService",serviceP);

  AhListAppend(list, serviceP);

  return(true);
	
} /* End of SvConnAppendService */

/* ----------------------------------------------------------------------------
 * SvConnSelect -- Select a connection request
 *
 * Arguments:     list -- A valid AhList.
 *            
 * Returns:
 *
 * Side Effects:
 *
 * Algorithm:
 *
 */
Public
Boolean
SvConnSelect(AhList list, struct timeval *timeout, int (*timeoutCallback)())
{

  int ret;
  int callbackReturn;
    
  fd_set  afds, rfds;
  int fd;
  int sockint;

  struct sockaddr_in fsin;	/* the request from address	*/
  int	alen;			/* from-address length		*/
  int ssock;
  
  serviceDesc  *serviceP;
  serviceDesc  *saveServiceP;   /* save the callBack routine    */
  AhList tempList;

  if ((tempList = AhListCreate()) == NULL) {
     printf("can't create list\n");
     return (false);
  }   

  FD_ZERO(&afds);  
  while (AhListNelem(list) > 0) {
    serviceP = AhListGetHead(list);
    FD_SET(serviceP->serviceSock,&afds);
    AhListAppend(tempList, serviceP);    
  }

  /* restore the serviceQueue  */
  while (AhListNelem(tempList) > 0) {
    serviceP = AhListGetHead(tempList);
    AhListAppend(list, serviceP);
  }
  
  while(1) {
    memcpy((char *)&rfds, (char *)&afds, sizeof(rfds));

    ret = select(MAX_SELECT_FD, &rfds, (fd_set *)0, (fd_set *)0,
				(struct timeval *)timeout);
				
    if ((ret == -1) && (errno == EINTR)) {
        printf("interrupted system call : select\n");
	continue;
    }
    else if (ret == 0) {
        /* timeout expired */
        timeoutCallback();
        continue;
    }
     
    for (fd=0; fd<MAX_SELECT_FD; ++fd) {
	if (FD_ISSET(fd, &rfds)) {

          printf("select from Service %d\n",fd);

          alen = sizeof(fsin);
          ssock = accept(fd, (struct sockaddr *)&fsin, &alen);
          if (ssock < 0)
	    errexit("accept %d\n",ssock);
	  switch (fork()) {
          case 0:	
		break;
          case -1:
		errexit("fork\n");
	  default:
		SvConnClose(ssock);
		continue;		/* parent */
          }
          /* child */

          /* search serviceFunc in service list */
          while (AhListNelem(list) > 0) {
             serviceP = AhListGetHead(list);         
             if (serviceP->serviceSock == fd) {
                saveServiceP = serviceP;
             }
             AhListAppend(tempList, serviceP);    
          }

          /* restore the serviceQueue  */
          while (AhListNelem(tempList) > 0) {
            serviceP = AhListGetHead(tempList);
            AhListAppend(list, serviceP);
          }
          
          /* close all fd except 0,1,2 */        
          for (fd = NOFILE; fd >= 0; --fd) {
            if ((fd != ssock) && (fd > 2))    /* fd 0,1,2 fuer test kein close */
              SvConnClose(fd);         
          }

          /* call function */
          callbackReturn = saveServiceP->serviceFunc(ssock);
          SvConnClose(ssock);
          exit(callbackReturn);          
        }
      }
  }

  /* not reached */  
  return(0);
	
} /* End of SvConnSelect */


/* ----------------------------------------------------------------------------
 * SvConnRead -- not implemented
 *
 * Arguments:
 *
 * Returns:
 *
 * Side Effects:
 *
 * Algorithm:
 *
 */
Public
int
SvConnRead(int fd, char *buf, int buflen)
{
  int bufReadLen;

  bufReadLen = read(fd, buf, buflen);

  return(bufReadLen);
      
} /* End of SvConnRead */

/* ----------------------------------------------------------------------------
 * SvConnWrite -- not implemented
 *
 * Arguments:
 *
 * Returns:
 *
 * Side Effects:
 *
 * Algorithm:
 *
 */
Public
int
SvConnWrite(int fd, char *buf, int buflen)
{
   int bufWrite;

   bufWrite = write(fd, buf, buflen);
   
   return(bufWrite);

} /* End of SvConnWrite */

/* ----------------------------------------------------------------------------
 * SvConnClose -- not implemented
 *
 * Arguments:
 *
 * Returns:
 *
 * Side Effects:
 *
 * Algorithm:
 *
 */
Public
int
SvConnClose(int fd)
{

    close(fd);
    return(0);
    
} /* End of SvConnClose */


/* End of SvConn.c */

