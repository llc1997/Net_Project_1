#ifndef KEY_PTHREAD_H
#define KEY_PTHREAD_H

<<<<<<< HEAD


extern void *key_pthread(void *arg);
=======
#include <pthread.h>
#include <termios.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

extern void *key_pthread(void *arg);
char mygetch();
>>>>>>> 5ef0503a1cde32bc6f28def86654aaa7e72b40ab
void help(void);

#endif