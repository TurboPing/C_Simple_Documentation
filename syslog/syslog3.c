/* syslog3.c */

#include <stdio.h>
#include <signal.h>
#include <syslog.h>

int main()
{

  openlog("syslog3", LOG_PID, LOG_DAEMON);
 
  while (1)
  {

    /* Logging Debug Level 1  */
  
    syslog(LOG_LOCAL1 | LOG_DEBUG, "Log Local 1 Debug");

    /* Logging Info Level 1  */
  
    syslog(LOG_INFO | LOG_LOCAL1, "Log Local 1 Info");

    /* Logging Info Level 1  */
  
    syslog(LOG_NOTICE | LOG_LOCAL1, "Log Local 1 Notice");
  
    sleep(5);

  }
  /* not reached */
}


