/* strstr.c */

#include <stdio.h>

int main()
{

  char *ptr;
  char string[100];

  sprintf(string, "c:\\dir1\\dir2/dir3\\dir4/dir5\\dir6/filename.ascii");
  printf("string is %s\n", string);

  /* if test file then delete file 
                  else create dir structure */
                  
  printf("\nsearch for backslash and change to slash\n\n");

  while ((ptr=(char *) strstr(string, "\\")) != NULL) {
    *ptr='/';
    printf("string : %s\n", string);
  }

  printf("\nsearch for slash and print out dir name\n\n");
  ptr = string; 
  while ((ptr=(char *) strstr(ptr, "/")) != NULL) { 
    *ptr='\0';
    printf("string : %s\n", string);

    /* testdir, mkdir */

    *ptr='/';
    ptr++;
  }

  /* end if else */
  
  /* copy file */

}