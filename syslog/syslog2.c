/* syslog2.c */

#include <stdio.h>
#include <signal.h>
#include <syslog.h>

int main()
{

  openlog("syslog2", LOG_PID, LOG_DAEMON);
 
  while (1)
  {

    /* Logging User Processes   */
    
    syslog(LOG_ERR, "Log Error");

    syslog(LOG_USER | LOG_ERR, "Log User/Error");

    syslog(LOG_USER | LOG_WARNING, "Log User/Warning");

    syslog(LOG_USER | LOG_NOTICE, "Log User/Notice");

    syslog(LOG_USER | LOG_INFO, "Log User/Info");

    syslog(LOG_USER | LOG_DEBUG, "Log User/Debug");

    /* Logging Debug Level 1  */
  
    syslog(LOG_LOCAL1 | LOG_DEBUG, "Log Local 1 Debug");

    /* Logging Info Level 1  */
  
    syslog(LOG_LOCAL1 | LOG_INFO, "Log Local 1 Info");

    /* Logging Debug Level 2  */
  
    syslog(LOG_LOCAL2 | LOG_DEBUG, "Log Local 2 Debug");

    /* Logging Debug Level 3  */

    syslog(LOG_LOCAL3 | LOG_DEBUG, "Log Local 3 Debug");

    /* Logging Debug Level 4  */

    syslog(LOG_LOCAL4 | LOG_DEBUG, "Log Local 4 Debug");

    /* Logging Debug Level 5  */

    syslog(LOG_LOCAL5 | LOG_DEBUG, "Log Local 5 Debug");

    /* Logging Debug Level 6  */

    syslog(LOG_LOCAL6 | LOG_DEBUG, "Log Local 6 Debug");

    /* Logging Debug Level 7 */

    syslog(LOG_LOCAL7 | LOG_DEBUG, "Log Local 7 Debug");
  
    sleep(5);

  }
  /* not reached */
}


