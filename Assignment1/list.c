#include <stdio.h>
#include "list.h"

static List availableLists[LIST_MAX_NUM_HEADS];    // global statically allocated list of possible unique Lists --> Array of List pointers
static Node availableNodes[LIST_MAX_NUM_NODES];         // global statically allocated list of nodes.
static int numList = 0;
static int numNodes = 0;
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
        for (int i = 0; i < LIST_MAX_NUM_HEADS; i++){
            availableLists[i].head = NULL;               // initialize everything to NULL  
            availableLists[i].tail = NULL;
            availableLists[i].current = NULL;
            availableLists[i].listSize = 0;

            if (i != 0 && i != LIST_MAX_NUM_HEADS - 1){
                availableLists[i].next = &availableLists[i + 1];
                availableLists[i].prev = &availableLists[i - 1];
            }
        }

        listHead = &availableLists[0];
        listHead->next = &availableLists[1];
        listHead->prev = NULL;

        listTail = &availableLists[LIST_MAX_NUM_HEADS - 1];
        listTail->next = NULL;
        listTail->prev = &availableLists[LIST_MAX_NUM_HEADS];


        for (int i = 1; i < LIST_MAX_NUM_NODES - 1; i++){
            availableNodes[i].item = NULL;
            availableNodes[i].next = NULL;
            availableNodes[i].prev = NULL;

            if (i != 0 && i != LIST_MAX_NUM_NODES - 1){
                availableNodes[i].next = &availableNodes[i + 1];
                availableNodes[i].prev = &availableNodes[i - 1];
            }
        }

        nodeHead = &availableNodes[0];
        nodeHead->next = &availableNodes[1];
        nodeHead->prev = NULL;

        nodeTail = &availableNodes[LIST_MAX_NUM_NODES - 1];
        nodeTail->next = NULL;
        nodeTail->prev = &availableNodes[LIST_MAX_NUM_NODES];

        initialSetup = false;
    }

    if (numList < LIST_MAX_NUM_HEADS){

        listHead = listHead->next;          // give the new list the memory position of the next available "list position"
        newList = listHead;
        numList++;
    }
    
    return newList;
}

int List_count(List* pList){               // returns number of elements in the list, O(1).
    assert(pList != NULL);
    return pList->listSize;
}


void* List_first(List* pList){
    assert(pList != NULL);
    if (pList->listSize != 0){
        pList->current = pList->head;
        return pList->current;
    }
    return NULL;
}

void* List_last(List* pList){
    assert(pList != NULL);
    if (pList->listSize != 0){
        pList->current = pList->tail;
        return pList->current;
    }
    return NULL;
}

void* List_next(List* pList){
    assert(pList != NULL);
    if (pList->next != NULL){
        pList->current = pList->next;
        return pList->current;
    }
    pList->current = LIST_OOB_END;
    return NULL;
}

void* List_prev(List* pList){
    assert(pList != NULL);
    if (pList->prev != NULL){
        pList->current = pList->prev;
        return pList->current;
    }
    pList->current = LIST_OOB_START;
    return NULL;
}

void* List_curr(List* pList){
    assert(pList != NULL);
    return pList->current;
}


int List_append(List* pList, void* pItem){
    assert(pList != NULL);
    Node* lastItem = pList->tail;

    if (numNodes < LIST_MAX_NUM_NODES){
        lastItem->next = nodeHead;
        Node* newItem = lastItem->next;
        nodeHead = nodeHead->next;

        numNodes++;
        
    }

    return 0;
}


