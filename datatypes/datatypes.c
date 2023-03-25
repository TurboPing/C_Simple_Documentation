/* printf.c */

#include <stdio.h>
#include <stdlib.h>

char charType;
short shortType;
int intType;
float floatType;
double doubleType;
long longType;
long long longLongType;

typedef struct {
  int intType[10];
} int10;

typedef struct {
  char charType[20];
} char20;

typedef struct {
  int10 int10Name;
  char charName[1000];
  double doubleName;
} mixedType;

void *voidPtr;
char *charPtr;
int *intPtr;
int10 *int10Ptr;
mixedType *mixedTypePtr;

int main()
{

    printf("char:%d\n", sizeof(charType));
    printf("short:%d\n", sizeof(shortType));
    printf("int:%d\n", sizeof(intType));
    printf("float:%d\n", sizeof(floatType));
    printf("double:%d\n", sizeof(doubleType));
    printf("long:%d\n", sizeof(longType));
    printf("long long:%d\n", sizeof(longLongType));
    printf("\n");
    printf("int10:%d\n", sizeof(int10));
    printf("char20:%d\n", sizeof(char20));
    printf("mixed:%d\n", sizeof(mixedType));
    printf("\n");
    printf("void*:%d\n", sizeof(void *));
    printf("char*:%d\n", sizeof(char *));
    printf("int*:%d\n", sizeof(int *));
    printf("int10*:%d\n", sizeof(int10 *));
    printf("mixedType*:%d\n", sizeof(mixedType *));

}
