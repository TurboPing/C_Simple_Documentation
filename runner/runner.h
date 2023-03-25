/* runner.h */

typedef struct runner *Runner;

int setRunner(int id, char *firstname, char *lastname, char category); 

int setId(int id);

int setFirstname(int id, char *name);

int setLastname(int id, char *name);

int setTime(int id, int time, int value);

int getIdPtr(Runner *runner);

char* getFirstname(int id);

char* getFirstnamePtr(Runner *runner);

char* getLastname(int id);

char* getLastnamePtr(Runner *runner);

char getCategoryPtr(Runner *runner);

int getTime(Runner *runner, int i);

Runner* getRunner(int id);

Runner** getAllRunner();

char* getRunnerTitleLine();

