/* mkdir.c */

#include <stdio.h>
#include <dirent.h>                /* Directory */
#include <sys/stat.h>

#define MODE_RESULTS_DIRECTORY     (S_IREAD|S_IWRITE|S_IEXEC|\
                                    S_IRGRP|S_IXGRP|\
                                    S_IROTH|S_IXOTH)

int main()
{

  DIR *dirp;
  struct dirent *direntp;

  dirp = opendir( "/project/rci/1.1/distrib/data/tests/LastResu/" );

  while ( (direntp = readdir( dirp )) != NULL )
     (void)printf( "%s\n", direntp->d_name );

  (void)closedir( dirp );
  return (0);
  
}
