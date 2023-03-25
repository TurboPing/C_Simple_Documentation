/* setuid.c */

     #include <sys/types.h>
     #include <unistd.h>

int main()
{
    char *login;

  if ((login = cuserid(NULL)) == NULL) {
    printf("Your login name is unknown, please check the installation\n");
    login = "unknown";
  }

  printf("login:%s\n", login);

}
