/* forkTest.c */

#include <stdio.h>
#include <sys/errno.h>
#include <sys/wait.h>

extern errno;

int execCommand(char *command) {

	int pid;
	int status;
		
	if ((pid = fork()) < 0) {
		printf("Could not fork"); 
		exit(1);
	}
	else if (pid == 0) {

		freopen("/dev/null","r",stdin);
		freopen("/dev/null","w",stdout);
		freopen("/dev/null","w",stderr);
		
		/* We're in the child */
		if (execlp(command, command, "50", NULL) < 0) {
			printf("Could not execl command"); 
			exit(1);
		}
	}

	sleep(10);
        exit(0);

	while (1) {
		if ((pid = wait3(&status, WNOHANG, 0)) > 0) {
	    		printf("reap child process.(pid=%d):(errno:%d)\n", 
			 	pid, errno);
			break;
		}
		else if (pid == 0) {
			/* nothing pending just wait */
	    	}
		else if ((pid == -1) && (errno==EINTR)) {
	    		printf("wait3() interrupted by signal.(pid=%d):(errno:%d)\n",
			 	pid, errno);
		}
		else {
	    		printf("Error in wait3().(pid=%d):(errno:%d)\n",
	    			pid, errno);
	    		break;
		}
    	}

	if (WIFEXITED(status)) {
		/* Normal termination */
		printf("command exited with %d\n", WEXITSTATUS(status));
	}


}

int main()
{

        execCommand("/usr/bin/sleep");
        
	exit(0);
}
