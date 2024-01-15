#include <stdio.h>
#include "list.h"

static List availableLists[LIST_MAX_NUM_HEADS];    // global statically allocated list of possible unique Lists --> Array of List pointers
static Node availableNodes[LIST_MAX_NUM_NODES];         // global statically allocated list of nodes.
static int listSize = 0;
static int numList = 0;
static bool initialSetup = true;
static Node* nodeHead;
static Node* nodeTail;
static List* listHead;
static List* listTail;

// Makes a new, empty list, and returns its reference on success. 
// Returns a NULL pointer on failure.
List* List_create(){
    List* newList = NULL;      // returns NULL if past LIST_MAX_NUM_HEADS

    // initial setup of the data structure (first call of List_create())
    if (initialSetup){
        for (int i = 1; i < LIST_MAX_NUM_HEADS - 1; i++){
            availableLists[i].head = NULL;               // initialize everything to NULL  
            availableLists[i].current = NULL;
        }

        listHead = &availableLists[0];
        listHead->next = &availableLists[1];
        listHead->prev = NULL;

        listTail = &availableNodes[LIST_MAX_NUM_HEADS - 1];
        listTail->next = NULL;
        listTail->prev = &availableNodes[LIST_MAX_NUM_HEADS];


        for (int i = 1; i < LIST_MAX_NUM_NODES - 1; i++){
            availableNodes[i].next = &availableNodes[i + 1];
            availableNodes[i].prev = &availableNodes[i - 1];
        }

        nodeHead = &availableNodes[0];
        nodeHead->next = &availableNodes[1];
        nodeHead->prev = NULL;

        nodeTail = &availableNodes[LIST_MAX_NUM_NODES - 1];
        nodeTail->next = NULL;
        nodeTail->prev = &availableNodes[LIST_MAX_NUM_NODES];

        initialSetup = false;
    }

    if ((numList - 1 ) < LIST_MAX_NUM_HEADS){
        availableLists[listSize].head = nodeHead;           // head to the new list. need to find an empty list node. 
        newList = &availableLists[listSize];

        Node* listInit = newList->head;
        listInit->prev = NULL;
        listInit->next = NULL;
        numList++;
    }
    
    return newList;
}

int List_count(List* pList){               // returns number of elements in the list, O(1).
    assert(pList != NULL);
    return listSize;
}