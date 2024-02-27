#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "input.h"

#define MAX_LEN 1024
#define MAX_BUFFER_LEN 256

static List* list;
static char buffer[MAX_BUFFER_LEN];

static pthread_t threadPID;
static pthread_mutex_t inputMutex = PTHREAD_MUTEX_INITIALIZER;

void* inputRoutine(void* unused)
{
    while (1) {
        int counter = 0;
        // empty buffer
        memset(buffer, 0, MAX_BUFFER_LEN);

        // code for reading one byte at a time inspired from: https://stackoverflow.com/questions/15883568/reading-from-stdin
        while (read(0, buffer, 1) > 0 && counter < MAX_BUFFER_LEN - 1) {
            memset(buffer, 0, MAX_BUFFER_LEN);
            if (buffer[counter] == '\n') {
                break;
            }
            counter++;
        }

        buffer[counter] = '/0';
        if ()


        char* msg = (char*)malloc(sizeof(buffer));

        
        // check for clean shutdown to terminate
        if ((msg[0] == '!') && (msg[1] == '\n') && (msg[2] == '\0')) {
            input_shutdown();
            free(msg);
            return NULL;
        }

        pthread_mutex_lock(&inputMutex);
        {
            int status = List_prepend(list, buffer);
            if (status == -1) {
                printf("unable to add to list\n");
                return NULL;
            }
        }
        pthread_mutex_unlock(&inputMutex);
    }

}



void input_init(List* myList)
{
    list = myList;
    int threadStatus = pthread_create(&threadPID, NULL, inputRoutine, NULL);
    if (threadStatus != 0) {
        printf("thread() error: %s\n", strerror(errno));
        return NULL;
    }
}

void input_shutdown(void)
{

}
