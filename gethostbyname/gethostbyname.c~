#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

void pr_hostent(struct hostent *p_hostent)
{
   char **ppc_alias;
   char **ppc_addr;
   struct in_addr in;
         

   if (p_hostent != NULL)
   {
      for (ppc_addr=p_hostent->h_addr_list; *ppc_addr != NULL; ppc_addr++)
      {
         memcpy(&in.s_addr, *ppc_addr, sizeof(in.s_addr));
         printf("%s\t%s", inet_ntoa(in), p_hostent->h_name);
         for (ppc_alias = p_hostent->h_aliases; *ppc_alias != NULL; ppc_alias++)
            printf(" %s", *ppc_alias);
            
         putchar('\n');
      }
   }
   else
   {
      printf("Not found\n");
   }
}   

int main(int argc, char *argv[])
{
   struct hostent * p_hostent;
   
   if (argc<2)
   {
      printf("usage: %s hostname\n", argv[0]);
      return -1;
   }

   p_hostent = gethostbyname(argv[1]);

   pr_hostent(p_hostent);

   return 0;
}

