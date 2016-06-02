#ifndef wait_h
#define wait_h

#include "type.h"

int sleep(int event);
int wakeup(int event);
int exit(int exitValue);
int wait(int status);

#endif
