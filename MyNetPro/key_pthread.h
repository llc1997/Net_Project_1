#ifndef KEY_PTHREAD_H
#define KEY_PTHREAD_H

#include <pthread.h>
#include <termios.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

extern void *key_pthread(void *arg);
char mygetch();
void help(void);

#endif