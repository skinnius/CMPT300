#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "input.h"

#define MAX_BUFFER_LEN 1024

static List* list; 

static pthread_t threadPID;
static pthread_mutex_t* inputMutex;

static pthread_mutex_t* syncMutex;
static pthread_cond_t* inputSync;

int addToList(char* msg) 
{
    int ret = 0;
    pthread_mutex_lock(inputMutex);
    {
        if (List_prepend(list, msg) == -1) {
            printf("failed to append message to list\n");
            ret = -1;
        }
    }
    pthread_mutex_unlock(inputMutex);
    return ret;
}

void signalToSend()
{
    pthread_mutex_lock(syncMutex);
    {
        pthread_cond_signal(inputSync);
    }
    pthread_mutex_unlock(syncMutex);
}



void* inputRoutine(void* unused)
{
    char* msg;
    char buffer[MAX_BUFFER_LEN];

    while (1) {

        // empty out buffer
        memset(buffer, 0, MAX_BUFFER_LEN);

        if (fgets(buffer, MAX_BUFFER_LEN, stdin) == NULL) {             
            printf("stdin error\n");
            return NULL;
        }

        // newly allocate memory for list insertion
        msg = (char*)malloc(strlen(buffer) + 1);
        strncpy(msg, buffer, strlen(buffer) + 1);
        msg[strlen(buffer)] = '\0';

        // critical section (accessing the shared "input" list with send)
        if (addToList(msg) == -1) {
            break;
        }

        // signal to send() that something is ready  
        signalToSend();
    }
    return NULL;
}


void input_init(List* myList, pthread_cond_t* cond, pthread_mutex_t* mutex, pthread_mutex_t* syncm)
{
    list = myList;
    inputSync = cond;
    inputMutex = mutex;
    syncMutex = syncm;

    int threadStatus = pthread_create(&threadPID, NULL, &inputRoutine, NULL);
    if (threadStatus != 0) {
        printf("thread() error: %s\n", strerror(errno));
    }
}

void input_shutdown(void)
{
    pthread_cancel(threadPID);
    pthread_join(threadPID, NULL);
}
