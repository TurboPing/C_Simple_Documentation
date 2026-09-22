#include <stdio.h>
#include <stdlib.h>

#include "module.h"

#define MAX_NAME_LEN 100
#define MAX_TEXT_LEN 1000
#define MAX_NUMBER_OF_FIELDS 10000

typedef struct FieldDesc {
  int number;
  int time;
} FieldDesc;

typedef struct HandleDesc {
  int i;
  char name[MAX_NAME_LEN];
  char *text;
  int field_number;
  FieldDesc field[MAX_NUMBER_OF_FIELDS];  
} HandleDesc;

Handle Open() {

  printf("Open\n");

  Handle handle;

  if ((handle = (Handle)calloc(1, sizeof(HandleDesc))) == NULL) {
    return (Handle)NULL;
  }

  if ((handle->text = (char *)calloc(1, MAX_TEXT_LEN)) == NULL) {
    return (Handle)NULL;
  }

  handle->i = 1;
  sprintf(handle->name, "%s%d", "name", handle->i);
  sprintf(handle->text, "%s", "example text");

  handle->field_number = 0;
  
  return (handle);

}

int Close(Handle handle) {

  printf("Close\n");

  free(handle);

  return (0);

}

int Change(Handle handle) {

  printf("Change\n");

  handle->i++;
  sprintf(handle->name, "%s%d", "name", handle->i);

  return (0);

}

int Create(Handle handle, int number, int time) {

  printf("Create number:%d|time:%d\n",
         number, time);

  /* fehler behandlung overflow */

  handle->field[handle->field_number].number = number;
  handle->field[handle->field_number].time = time;
  handle->field_number++;

  return (0);

}

int Get(Handle handle, int number) {

  int i;

  if (handle->field_number > 0) {
    for (i = 0; i < handle->field_number; i++) {
      if (handle->field[i].number == number) {
	return handle->field[i].time;
      }
    }
  }

  return -1;
  
}

int Print(Handle handle) {

  int i;

  printf("Print\n");

  printf("-- Handle i:%d|name:%s|text:%s|field_number:%d\n", 
	 handle->i,
         handle->name,
         handle->text,
         handle->field_number);

  if (handle->field_number > 0) {
    for (i = 0; i < handle->field_number; i++) {
      printf("-- field[%d]:number:%d|time:%d\n",
	     i,
             handle->field[i].number,
	     handle->field[i].time);
    }
  }

  return (0);

}
