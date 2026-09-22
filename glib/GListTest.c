/* glibTest.c
 *
 *
 * gcc -I/cc/vobs/buildtools/sparc-SunOS-5.10/include/glib-2.0/ 
 *     -L/cc/vobs/buildtools/sparc-SunOS-5.10/lib -lglib-2.0 glibTest.c
 */

#include <stdio.h>

#include "glib.h"

int main () {

  GList *list;
  GList *tempList;

  char name[100] = "hello world"; 
  char *tempName;

  list = g_list_append(list, (gpointer)&name[0]);
  list = g_list_append(list, (gpointer)&name[0]);


/*
  tempList = list;
  tempList = g_list_first (tempList);
  while (tempList != NULL) {
    tempName = tempList->data;
    printf("name:%s\n", "*tempName");
    tempList = g_list_next(tempList);
  }
*/

  printf("done\n");
  return 0;

}
