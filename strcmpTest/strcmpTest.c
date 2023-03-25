/*/ hello.cpp */

#include <stdio.h>

int main()
{

int bNotUnique;
char string1[100] = "string1";
char string2[100] = "string2";

printf("bad code\n");
if ( (bNotUnique = (strcmp((char *)(string1), string1))) == 0 ) {}
printf("string1:%s|string2:%s|bNotUnique = %d\n", string1, string1, bNotUnique);
if ( (bNotUnique = (strcmp((char *)(string1), string2))) == 0 ) {}
printf("string1:%s|string2:%s|bNotUnique = %d\n", string1, string2, bNotUnique);

printf("good code\n");
if (bNotUnique = (strcmp((char *)(string1), string1) == 0)) {}
printf("string1:%s|string2:%s|bNotUnique = %d\n", string1, string1, bNotUnique);
if (bNotUnique = (strcmp((char *)(string1), string2) == 0)) {}
printf("string1:%s|string2:%s|bNotUnique = %d\n", string1, string2, bNotUnique);


}