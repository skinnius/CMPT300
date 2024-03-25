#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#include "list.h"
#include "dataStructures.h"

static List* readyQueue[3];    // 3 ready queues

static List* sendList;      // queue of processes waiting on send operation
static List* receiveList;   // queue of processes waiting on receive operation
static List* blockedList;   // queue of processes that are blocked. 

static semaphore semaphores[5];        // 5 semaphores
static pcb* runningProcess;


static long currPID = 0;

/*-------------------------------------- misc -----------------------------------------------------*/




/* ---------------------------------------- init --------------------------------------------------*/

// creating the init process -- called once and never again. 
pcb* createInitProcess() {
    pcb* init = (pcb*)malloc(sizeof(pcb));
    init->pid = currPID;
    currPID++;
    init->processState = RUNNING;

    return init;
}


/* ---------------------------------------- Create --------------------------------------------------*/

// creating a new process -- return pid on success, -1 on failure.

int createProcessInterface() {
    int prio;
    int pid;

    while (1) {
        printf("input process priority (0 = high, 1 = norm, 2 = low). -1 to return to menu ");
        scanf("%d", &prio);
        
        if (prio == -1) {
            return -1;
        }

        if (prio < -1 || prio > 2) {
            printf("invalid input.\n");
            continue;
        }
        
        pid = createNewProcess(prio);
        return pid;
    }
}


int createNewProcess(int prio) {
    pcb* newProcess = (pcb*)malloc(sizeof(pcb));

    if (newProcess = NULL) {
        return -1;
    }

    newProcess->pid = currPID;
    currPID++;
    newProcess->priority = prio;
    newProcess->processState = READY;

    int success = List_append(readyQueue[prio], newProcess);
    
    if (success < 0) {
        return -1;
    }

    return newProcess->pid;
}

void createReport(int pid) {
    if (pid < 0) {
        printf("Process creation failed.\n");
    }
    else {
        printf("Process %d successfully created.", &pid);
    }
}


/**---------------------------------------------Fork--------------------------------------------------**/

// returns -1 on failure, pid on success. 
int forkCommandInterface() {

    if (runningProcess->pid == 0) {         // 0 = pid of init
        return -1;
    }

    pcb* processCopy = (pcb*)malloc(sizeof(pcb));
    processCopy->pid = currPID;
    currPID++;
    processCopy->priority = runningProcess->priority;
    processCopy->processState = READY;

    int success = List_append(readyQueue[processCopy->priority], processCopy);

    if (success < 0) {
        return -1;
    }

    return processCopy->pid;
}

void forkReport(int successState) {
    if (successState < 0) {
        printf("Fork command failed. \n");
    }
    else {
        printf("Fork command successful. pid of the new process is %d", &successState);
    }
}




/** ------------------------------------------- User Input------------------------------------------ **/


// error checking
bool inputError(char* userInput) {
    if (strlen(userInput) != 1) {
        return true;
    }
    return false;
}

// returns 0 on success, -1 on failure. 
int chooseFunction(char input) {       
    switch(input) {
        case 'C':
            int pid = createProcessInterface();
            createReport(pid);
            break;

        case 'F':
            int success = forkCommandInterface();
            forkReport(success);
            break;

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
    return 0;
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