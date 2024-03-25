#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#include "list.h"
#include "dataStructures.h"

static List* readyQueue[3];    // 3 ready queues

static List* sendList;      // queue of processes waiting on send operation
static List* receiveList;   // queue of processes waiting on receive operation

static semaphore* semaphores[5];        // 5 semaphores
static pcb* runningProcess;
static pcb* init;

static long currPID = 0;

/*-------------------------------------- misc -----------------------------------------------------*/

// pid equals comparator
bool equalsComparator(void* pItem, void* pComparisonArg) {
    return (((pcb*)pItem)->pid == *(int*)pComparisonArg);
}

// free item 
void freeItem(void* pItem) {
    free(pItem);
    pItem = NULL;
}

/** ------------------------------------------- Scheduling ------------------------------------------ **/
// NEED TO FIX STARVATION PROBLEM !!!!
void cpu_scheduler() {
    // go through the three different queue levels
    for (int i = 0; i < 3; i++) {
        List* currList = readyQueue[i];
        if (List_count(currList) == 0) {
            continue;
        }

        pcb* newRunningProcess = List_trim(currList);
        newRunningProcess->processState = RUNNING;

        runningProcess = newRunningProcess;
        return; 
    }
    // if nothing in queues
    runningProcess = init;
}



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
        printf("input process priority (0 = high, 1 = norm, 2 = low). input -1 to return to menu ");
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
    newProcess->currentPriority = prio;
    newProcess->processState = READY;
    newProcess->message = (proc_message*)malloc(sizeof(proc_message));

    int success = List_prepend(readyQueue[prio], newProcess);
    
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


/**--------------------------------------------- Fork --------------------------------------------------**/

// returns -1 on failure, pid on success. 
int forkCommandInterface() {

    if (runningProcess->pid == 0) {         // 0 = pid of init
        return -1;
    }

    pcb* processCopy = (pcb*)malloc(sizeof(pcb));
    processCopy->pid = currPID;
    currPID++;
    processCopy->priority = runningProcess->currentPriority;
    processCopy->processState = READY;

    int success = List_prepend(readyQueue[processCopy->priority], processCopy);

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


/**--------------------------------------------- Kill --------------------------------------------------**/ 

// KILL EVERY PROCESS OCCURENCE IN ALL QUEUES RAHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH

int killProcessInterface() {
    int pid;

    printf("input pid. input -1 to return to menu ");
    scanf("%d", &pid);
    
    if (pid == -1) {
        return -1;
    }

    int occurenceCounter = killProcess(pid);
    return occurenceCounter;
}


int killProcess(int pid) {

    if (pid == 0) {
        // terminate program. 
    }

    int occurenceCounter = 0;
    // check running process
    if (runningProcess->pid == pid) {
        free(runningProcess);

        cpu_scheduler();                    // get next scheduled process from queue
    }


    for (int i = 0; i < 3; i++) {           // check through all ready queues
        List* currList = readyQueue[i];
        occurenceCounter += removeProcessInList(currList, &pid);
    }

    // check through send list
    occurenceCounter += removeProcessInList(sendList, &pid);
    // check through receive list
    occurenceCounter += removeProcessInList(receiveList, &pid);

    // check through all semaphore queues
    for (int i = 0; i < 5; i++) {           
        semaphore* currSemaphore = semaphores[i];
        List* currList = currSemaphore->processList;
        occurenceCounter += removeProcessInList(currList, &pid);
    }

    return occurenceCounter;
}

// // add old running process back into a queue
//         List_prepend(readyQueue[runningProcess->priority], runningProcess);
//         runningProcess->priority = READY;
//         // runningProcess->proc_message = //idk do something with the message


int removeProcessInList(List* pList, int* pid) {
    List_first(pList);
    pcb* process = List_search(pList, equalsComparator, pid);
    if (process != NULL) {
        List_remove(pList);
        return 1;
    }
    return 0;
}


void killReport(int num) {
    if (num < 0) {
        printf("Kill failed");
    }
    else if (num == 0) {
        printf("No process with matching id found");
    }
    else {
        printf("%d instances of process killed", &num);
    }
}


/**--------------------------------------------- Exit --------------------------------------------------**/ 

void exit() {
    killProcess(runningProcess->pid);
    printf("process %d has control of the CPU", &runningProcess->pid);
}

/**--------------------------------------------- Quantum --------------------------------------------------**/ 

void quantum() {
    cpu_scheduler();
    printf("process %d has control of the CPU", &runningProcess->pid);
}

/**--------------------------------------------- Send --------------------------------------------------**/ 

// return 0 on success, -1 on failure
int sendInterface() {
    int pid;
    char* msg = malloc(40);

    printf("input pid: ");
    scanf("%d", &pid);

    printf("input message (40 characters max): \n");
    scanf("%s", &msg);

    
    // check queues waiting on a send
    if (attemptSend(receiveList, pid, msg) == 0) {
        return 0;
    }
    // check ready queues for process. 
    for (int i = 0; i < 3; i++) {
        List* pList = readyQueue[i];
        if (attemptSend(pList, pid, msg) == 0) {
            return 0;
        }
    }  

    // check semaphore queues for process.
    for (int i = 0; i < 5; i++) {
        semaphore* currSem = semaphores[i];
        List* pList = currSem->processList;
        if (attemptSend(pList, pid, msg) == 0) {
            return 0;
        } 
    }
    // no matches
    return -1;
}


int attemptSend(List* pList, int pid, char* msg) {
    List_first(pList);
    pcb* process = List_search(pList, equalsComparator, pid);

    if (process != NULL) {                  // process has been found
        process->message->msg = msg;
        process->message->from = runningProcess->pid;       // came from currently running process
        
        // add process waiting on receive back onto the ready queue
        process->processState = READY;
        List_prepend(readyQueue[process->pid], process);

        // block the process that called for a "send"
        runningProcess->processState = BLOCKED;
        List_prepend(pList, runningProcess);
        cpu_scheduler();
        return 0;
    }
    return -1;
}

// make the reports nice later
void sendReport(int status) {
    if (status == -1) {
        printf("failed to send");
    }
    else {
        printf("successfully sent");
    }
}

/**--------------------------------------------- Receive --------------------------------------------------**/
// return 0 for block, 1 for no block. 

int receive() {
    // check if theres a proc message of type RECEIVE.
    if (runningProcess->message->type == RECEIVE) {
        printf("received a message from process %d: %s", runningProcess->message->from, 
                runningProcess->message->msg);
        return 1;
    }

    // if nothing to be received, throw running process back onto ready queue.
    else {
        runningProcess->processState = BLOCKED;
        List_prepend(readyQueue, runningProcess);
        cpu_scheduler();
        return 0;
    }

}

void receiveReport(int status) {
    if (status == 0) {
        printf("");     // some scheduling stuff. 
    }
    else {
        printf("Current process is still running.");
    }
}

/**--------------------------------------------- Reply --------------------------------------------------**/

int replyInterface() {
    int pid;
    char* msg = malloc(40);

    printf("input pid: ");
    scanf("%d", &pid);

    printf("input message (40 characters max): \n");
    scanf("%s", &msg);

    
    // check queues waiting on a send
    if (attemptSend(receiveList, pid, msg) == 0) {
        return 0;
    }
    // check ready queues for process. 
    for (int i = 0; i < 3; i++) {
        List* pList = readyQueue[i];
        if (attemptReply(pList, pid, msg) == 0) {
            return 0;
        }
    }  

    // check semaphore queues for process.
    for (int i = 0; i < 5; i++) {
        semaphore* currSem = semaphores[i];
        List* pList = currSem->processList;
        if (attemptSend(pList, pid, msg) == 0) {
            return 0;
        } 
    }
    // no matches
    return -1;
}


int attemptReply(List* pList, int pid, char* msg) {
    List_first(pList);
    pcb* process = List_search(pList, equalsComparator, pid);

    if (process != NULL) {                                  // process has been found
        process->message->msg = msg;
        process->message->type = REPLY;
        process->message->from = runningProcess->pid;       // came from currently running process
        
        // add process waiting on reply back onto the ready queue
        process->processState = READY;
        List_prepend(readyQueue[process->pid], process);

        return 0;
    }
    return -1;
}

void replyReport(int status) {
    if (status == -1) {
        printf("reply failed\n");
    } 
    else {
        printf("reply successful\n");
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
            int fork = forkCommandInterface();
            forkReport(fork);
            break;

        case 'K':
            int kill = killProcessInterface();
            killReport(kill);
            break;
        case 'E':
            exit();
            break;
        case 'Q': 
            quantum();
            break;
        case 'S': 
            int sendStatus = sendInterface();
            sendReport(sendStatus);
            break;
        case 'R':
            int receiveStatus = receive();
            receiveReport(receiveStatus);
            break;
        case 'Y':
            int replyStatus = replyInterface();
            replyReport(replyStatus);
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


void initializeLists() {
    for (int i = 0; i < 3; i++) {
        readyQueue[i] = List_create();
    }

    sendList = List_create();
    receiveList = List_create();

    for (int i = 0; i < 5; i++) {
        semaphore* curr = semaphores[i];
        curr->processList = List_create();
    }
}

int main() {
    char* userInput;
    init = createInitProcess();
    runningProcess = init;

    initializeLists();


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