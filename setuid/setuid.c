/* setuid.c */

     #include <sys/types.h>
     #include <unistd.h>

int main()
{
    int answer;

    answer = setuid("7108");
    printf("answer from setuid is %s", answer);
 
    while (1) {
    
    }     
}
