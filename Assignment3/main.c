#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#include "list.h"
#include "dataStructures.h"

static List readyQueue[3];    // 3 ready queues

static List sendList;      // queue of processes waiting on send operation
static List receiveList;   // queue of processes waiting on receive operation
static List blockedList;   // queue of processes that are blocked. 

static semaphore semaphores[5];        // 5 semaphores
static pcb* runningProcess;


static long currPID = 0;


/* ---------------------------------------- init --------------------------------------------------*/

// creating the init process -- called once and never again. 
pcb* createInitProcess() {
    pcb* init = (pcb*)malloc(sizeof(pcb));
    init->pid = currPID;
    currPID++;
    init->processState = RUNNING;

    return init;
}












/** ------------------------------------------- User Input------------------------------------------ **/


// error checking
bool inputError(char* userInput) {
    if (strlen(userInput) != 1) {
        return true;
    }
    return false;
}

// returns 1 on success, -1 on failure. 
int chooseFunction(char input) {       
    switch(input) {
        case 'C':
            // do something 
            break;
        case 'F':
            // do something
            break;
        case 'K':
            // do something 
            break;
        case 'E':
            // do something
            break;
        case 'Q': 
            // do something
            break;
        case 'S': 
            // do soemthing
            break;
        case 'R':
            // do something
            break;
        case 'Y':
            // do something 
            break;
        case 'N':
            // do something
            break;
        case 'P':
            // do soemthing
            break;
        case 'V':
            // do something
            break;
        case 'I':
            // do seomthing 
            break;
        case 'T':
            // do something
            break;
        default:
            return -1;          // no matches
    }
}



int main() {
    char* userInput;
    pcb* init = createInitProcess();
    runningProcess = init;


    while (runningProcess != NULL) {
        printf("Enter a command: ");
        scanf("%s", &userInput);

        if (inputError(userInput)) {
            printf("invalid input");
            continue;
        }
        
        if (chooseFunction(toupper(userInput[0])) == -1) {
            printf("invalid input");
            continue;
        }

        


    }




}