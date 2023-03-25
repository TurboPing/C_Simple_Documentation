/*
 * setuser - test setuid and setgid library calls.
 *
 * ls -l setuser
 * -rwxr-xr-x   1 root     other      13768 Jun  5 10:54 setuser*
 */

#include <sys/types.h>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>


void PrintPwEntry(struct passwd *pw)
{
  printf("\n Password entry \n\n");
  printf("user's login name          %s\n", pw->pw_name);    
  printf("no longer used             %s\n", pw->pw_passwd);  
  printf("user's uid                 %d\n", pw->pw_uid);    
  printf("user's gid                 %d\n", pw->pw_gid);     
  printf("not used                   %d\n", pw->pw_age);     
  printf("not used                   %d\n", pw->pw_comment); 
  printf("typically user's full name %s\n", pw->pw_gecos);   
  printf("user's home dir            %s\n", pw->pw_dir);     
  printf("user's login shell         %s\n", pw->pw_shell);   
}


int main (int argc, char *argv[], char *envp[])
{
  struct passwd  *pswd;
  struct group   *grp;
  char           *home;
  char            homeVar[MAXPATHLEN];
  
 
  printf("\n Own users values \n\n");

  printf("UID %d EUID %d GID %d EGID %d\n",
	 getuid(), geteuid(), getgid(), getegid());

  if ((home = getenv("HOME")) == NULL) {
    perror("getenv $HOME failed");
  } else {
    printf("$HOME %s\n", home);
  }

  printf("Current Dir %s\n", getwd(homeVar));


 
  if ((pswd = getpwnam(argv[1])) == NULL) {

    perror("getpwnam failed");
    exit (1);

  }  else {

    PrintPwEntry(pswd);

    /*
     * Set new group and user id.
     * group id must be set first, because only root user can modify the
     * group.
     */
    if ((setgid(pswd->pw_gid)) == -1) {
      perror("setgid failed");
    }
    if ((setuid(pswd->pw_uid)) == -1) {
      perror("setuid failed");
    }

    /*
     * Set home variable and curren directory according to password
entry.
     */
    sprintf(homeVar, "HOME=%s", pswd->pw_dir);
    if ((putenv(homeVar)) != 0)
      perror("putenv failed");

    if ((chdir(pswd->pw_dir)) == -1) {
      perror("chdir failed");
    }
  }


  printf("\n\n Return values \n\n");

  printf("UID %d EUID %d GID %d EGID %d\n",
	 getuid(), geteuid(), getgid(), getegid());

  if ((home = getenv("HOME")) == NULL) {
    perror("getenv $HOME failed");
  } else {
    printf("$HOME %s\n", home);
  }

  printf("Current Dir %s\n", getwd(homeVar));

  if ((pswd = getpwnam(argv[1])) == NULL) {

    perror("getpwnam failed");
    exit (1);

  }  else {

    PrintPwEntry(pswd);

  }
  
  while (1) {}
  
  return (0);
}
