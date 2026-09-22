/* runner.c */

#include <string.h>

#define NAME_LEN 100

#define MAX_RUNNER 10000

struct runner {
  int id;
  char firstname[NAME_LEN];
  char lastname[NAME_LEN];
  char category;
  int time[10];
};

typedef struct runner Runner;

Runner runner[MAX_RUNNER];
Runner *runnerPtr[MAX_RUNNER];

int setRunner(int id, char *firstname, char *lastname, char category) {

  runner[id].id = id;
  strcpy(&runner[id].firstname[0], firstname);
  strcpy(&runner[id].lastname[0], lastname);
  runner[id].category = category;

  runnerPtr[id] = &runner[id];

  return 0;

}

int setId(int id) {

  runner[id].id = id;

  return 0;
}

int setFirstname(int id, char *name) {

  strcpy(&runner[id].firstname[0], name);
  runnerPtr[id] = &runner[id];

  return 0;

}

int setLastname(int id, char *name) {

  strcpy(&runner[id].lastname[0], name);

  return 0;

}

int setTime(int id, int time, int value) {

  runner[id].time[time] = value;

  return 0;
}

int getIdPtr(Runner *runner) {

  return runner->id;

}

char* getFirstname(int id) {

  return &runner[id].firstname[0];

}

char* getFirstnamePtr(Runner *runner) {

  return runner->firstname;

}

char* getLastname(int id) {

  return &runner[id].lastname[0];

}

char* getLastnamePtr(Runner *runner) {

  return runner->lastname;

}

char getCategoryPtr(Runner *runner) {

  return runner->category;

}

int getTime(Runner *runner, int i) {

  return runner->time[i];

}

Runner* getRunner(int id) {

  return &runner[id];

}

char* getRunnerFirstname(Runner* runner) {

  return runner->firstname;  

}

Runner** getAllRunner() {

  runnerPtr[0] = &runner[4];
  runnerPtr[1] = &runner[7];
  runnerPtr[2] = &runner[1];
  runnerPtr[3] = &runner[8];
  runnerPtr[4] = &runner[3];
  runnerPtr[5] = &runner[9];
  runnerPtr[6] = NULL;

  return &runnerPtr[0];  

}

char* getRunnerTitleLine() {

  return "id\tfirstname\tlastname\tcat\ttime0\ttime1\ttime2";

}
