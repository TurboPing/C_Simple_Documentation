/* syslog.c */

#include <stdio.h>
#include <signal.h>
#include <syslog.h>

int main()
{

  openlog("applicationName", LOG_PID, LOG_DAEMON);
 
  while (1)
  {

    /* Logging User Processes   */

    syslog(LOG_USER | LOG_ERR, "Log User/Error");

    syslog(LOG_USER | LOG_WARNING, "Log User/Warning");

    syslog(LOG_USER | LOG_NOTICE, "Log User/Notice");

    syslog(LOG_USER | LOG_INFO, "Log User/Info");

    syslog(LOG_USER | LOG_DEBUG, "Log User/Debug");

    /* Logging Debug Level 0 */

    syslog(LOG_LOCAL0 | LOG_ERR  , "Log Local 0 Error");

    syslog(LOG_LOCAL0 | LOG_WARNING  , "Log Local 0 Warning");

    syslog(LOG_LOCAL0 | LOG_NOTICE  , "Log Local 0 Notice");

    syslog(LOG_LOCAL0 | LOG_INFO  , "Log Local 0 Info");

    syslog(LOG_LOCAL0 | LOG_DEBUG  , "Log Local 0 Debug");

    sleep(10);

  }
  /* not reached */
}


