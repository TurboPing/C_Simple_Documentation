/* syslog.c */

#include <stdio.h>
#include <signal.h>
#include <syslog.h>

int main()
{

  int i;
  int j;
  char buf[10000];

  openlog("applicationName", LOG_PID|LOG_CONS|LOG_NDELAY|LOG_NOWAIT, LOG_DAEMON);

  syslog(LOG_ERR, "This is a error log message");
  syslog(LOG_LOCAL0 | LOG_DEBUG, "This is a debug log message"); 
  syslog(LOG_INFO, "This is a info log message");
  
}


