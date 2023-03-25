/* printf.c */

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int i = 5;
    char name1[100] = "'das-ist-ein-langer-name-ueber-20-zeichen'";

    printf("a\tb\t\tc\t\t\td\n");

    printf("%16.16s   %s\n", name1, name1);

    printf("%20.-10s   %d\n", name1, i);

    printf("%-20.10s   %s\n", name1, name1);

    printf("%20.5s   %s\n", name1, name1);

    printf("%10.20s   %s\n", name1, name1);

    printf("%20.20s   %s\n", name1, name1);

    printf("%20.-10s   %s\n", name1, name1);

    printf("%20.-20s   %s\n", name1, name1);

}
