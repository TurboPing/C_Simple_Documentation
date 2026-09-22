#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

typedef	void	DaemonLibSigfunc(int);	/* for signal handlers */

int i;

/* ----------------------------------------------------------------------------
 * DaemonLibSignal -- Reliable version of signal(), using POSIX sigaction().
 *
 * Arguments:       signo    signal to catch.
 *                  func     Handler to install.
 *
 * Returns:         Previous signal disposition.
 *
 * Algorithm:       POSIX.1 compliant replacement for signal-function.
 *                  This is to be used when we want to use SIGALRM to timeout
 *                  I/O operations.
 *                  This should work for 4.3+BSD, SVR4 and  SunOs 4.1.3
 *                  (Stevens,1992, section 10.14
 *                   Advanced Programming in the UNIX Environment)
 */
void *
DaemonLibSignal(int signo, void *func)
{
	struct sigaction	act, oact;

	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (signo == SIGALRM) {
#ifdef	SA_INTERRUPT
		act.sa_flags |= SA_INTERRUPT;	/* SunOS */
#endif
	} else {
#ifdef	SA_RESTART
		act.sa_flags |= SA_RESTART;		/* SVR4, 44BSD */
#endif
	}
	if (sigaction(signo, &act, &oact) < 0)
		return(SIG_ERR);
	return(oact.sa_handler);

} /* End of DaemonLibSignal */

/* ----------------------------------------------------------------------------
 * SigHandler -- Catch all Signals.
 *
 * Arguments:      None.
 *
 * Returns:        Nothing.
 *
 * Side Effects:   None.
 *
 * Algorithm:      Nothing crazy.
 *
 */
void
SigHandler(int signo)
{

    switch(signo) {
    case	SIGHUP:
	printf("Caught signal SIGHUP -- Hangup\n");
	break;
    case	SIGINT:
	printf("Caught signal SIGINT -- Interrupt\n");
	break;
    case	SIGQUIT:
	printf("Caught signal SIGQUIT -- Quit\n");
	break;
    case	SIGILL:
	printf("Caught signal SIGILL -- Illegal Instruction\n");
	break;
    case	SIGTRAP:
	printf("Caught signal SIGTRAP -- Trace/Breakpoint Trap\n");
	break;
    case	SIGABRT:
	printf("Caught signal SIGABRT -- Abort\n");
	break;
    case	SIGEMT:
	printf("Caught signal SIGEMT -- Emulation Trap\n");
	break;
    case	SIGFPE:
	printf("Caught signal SIGFPE -- Arithmetic Exception\n");
	break;
    case	SIGKILL:
	printf("Caught signal SIGKILL -- Killed\n");
	break;
    case	SIGBUS:
	printf("Caught signal SIGBUS -- Bus Error\n");
	break;
    case	SIGSEGV:
	printf("Caught signal SIGSEGV -- Segmentation Fault\n");
	break;
    case	SIGSYS:
	printf("Caught signal SIGSYS -- Bad System Call\n");
	break;
    case	SIGPIPE:
	printf("Caught signal SIGPIPE -- Broken Pipe\n");
	break;
    case	SIGALRM:
	printf("Caught signal SIGALRM -- Alarm Clock\n");
	break;
    case	SIGTERM:
	printf("Caught signal SIGTERM -- Termination Signal\n");
	break;
    case	SIGUSR1:
	printf("Caught signal SIGUSR1 -- User Signal 1\n");
	break;
    case	SIGUSR2:
	printf("Caught signal SIGUSR2 -- User Signal 2\n");
	break;
    case	SIGCHLD:
	printf("Caught signal SIGCHLD -- Child Status Changed\n");
	break;
#ifdef SVR4
    case	SIGPWR:
	printf("Caught signal SIGPWR -- Power Fail/Restart\n");
	break;
#endif
    case	SIGWINCH:
	printf("Caught signal SIGWINCH -- Window Size Change\n");
	break;
    case	SIGURG:
	printf("Caught signal SIGURG -- Urgent Socket Condition\n");
	break;
#ifdef SYSV
    case	SIGPOLL:
	printf("Caught signal SIGIO/SIGPOLL -- I/O pending\n");
	break;
#else
    case  SIGIO:
	printf("Caught signal SIGIO/SIGPOLL -- I/O pending\n");
	break;
#endif
    case	SIGSTOP:
	printf("Caught signal SIGSTOP -- Stopped (signal)\n");
	break;
    case	SIGTSTP:
	printf("Caught signal SIGTSTP -- Stopped (user)\n");
	break;
    case	SIGCONT:
	printf("Caught signal SIGCONT -- Continued\n");
	break;
    case	SIGTTIN:
	printf("Caught signal SIGTTIN -- Stopped (tty input)\n");
	break;
    case	SIGTTOU:
	printf("Caught signal SIGTTOU -- Stopped (tty output)\n");
	break;
    case	SIGVTALRM:
	printf("Caught signal SIGVTALRM -- Virtual Timer Expired\n");
	break;
    case	SIGPROF:
	printf("Caught signal SIGPROF -- Profiling Timer Expired\n");
	break;
    case	SIGXCPU:
	printf("Caught signal SIGXCPU -- CPU time limit exceeded\n");
	break;
    case	SIGXFSZ:
	printf("Caught signal SIGXFSZ -- File size limit exceeded\n");
	break;
    default:
	printf("Caught signal %d -- Unknown signal\n", signo);
	break;
    }

} /* End of SigHandler */

/* ---------------------------------------------------------------------*
 * Function Name:  Main                                                 *
 *                                                                      *
 * Description:                                                         *
 *                                                                      *
 * Arguments:                                                           *
 *                                                                      *
 * Returns:                                                             *
 *                                                                      *
 * Side Effects:                                                        *
 *                                                                      *
 -----------------------------------------------------------------------*/
int
main(int argc, char ** argv, char ** envp)
{


  /* Handle signals */

  for(i = 1; i < NSIG; i++) {
    DaemonLibSignal(i, SigHandler);
  }

/*
  for(i = 1; i < NSIG; i++) {
    signal(i, SigHandler);
  }
*/

  while (1) {
        printf("i am still here at %d\n", time(NULL));
	sleep(5);  
  }

} /* End of main */
