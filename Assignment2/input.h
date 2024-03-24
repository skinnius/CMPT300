#ifndef _INPUT_H_
#define _INPUT_H_

#include <pthread.h>
#include "list.h"

void input_init(List* myList, pthread_cond_t* cond, pthread_mutex_t* mutex, pthread_mutex_t* syncm);

void input_shutdown(void);

#endif 