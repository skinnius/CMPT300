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
            availableLists[i].currNode = NULL;
            availableLists[i].current = LIST_OOB_START;
            availableLists[i].listSize = 0;
            availableLists[i].next = NULL;
            availableLists[i].prev = NULL;

            if (i != 0){
                availableLists[i].prev = &availableLists[i - 1];
            }

            if (i != LIST_MAX_NUM_HEADS - 1){
                availableLists[i].next = &availableLists[i + 1];
            }

            listHead = &availableLists[0];
            listTail = &availableLists[LIST_MAX_NUM_HEADS - 1];
        }

        for (int i = 0; i < LIST_MAX_NUM_NODES - 1; i++){
            availableNodes[i].item = NULL;
            availableNodes[i].next = NULL;
            availableNodes[i].prev = NULL;

            if (i != 0){
                availableNodes[i].prev = &availableNodes[i - 1];
            }

            if (i != LIST_MAX_NUM_NODES - 1){
                availableNodes[i].next = &availableNodes[i + 1];
            }

            nodeHead = &availableNodes[0];
            nodeTail = &availableNodes[LIST_MAX_NUM_NODES - 1];
        }

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


// Returns a pointer to the first item in pList and makes the first item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_first(List* pList){
    assert(pList != NULL);
    if (pList->listSize != 0){
        pList->current = pList->head->item;
        return pList->current;
    }
    return NULL;
}

// Returns a pointer to the last item in pList and makes the last item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_last(List* pList){
    assert(pList != NULL);
    if (pList->listSize != 0){
        pList->current = pList->tail->item;
        return pList->current;
    }
    return NULL;
}

// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer 
// is returned and the current item is set to be beyond end of pList.
void* List_next(List* pList){
    assert(pList != NULL);
    if (pList->currNode->next != NULL){
        pList->currNode = pList->currNode->next;
        pList->current = pList->currNode->item;
        return pList->current;
    }
    pList->current = LIST_OOB_END;
    return NULL;
}

// Backs up pList's current item by one, and returns a pointer to the new current item. 
// If this operation backs up the current item beyond the start of the pList, a NULL pointer 
// is returned and the current item is set to be before the start of pList.
void* List_prev(List* pList){
    assert(pList != NULL);
    if (pList->prev != NULL){
        pList->currNode = pList->prev;
        pList->current = pList->currNode->item;
        return pList->current;
    }
    pList->current = LIST_OOB_START;
    return NULL;
}


// Returns a pointer to the current item in pList.
void* List_curr(List* pList){           
    assert(pList != NULL);  
    return pList->current;
}

// Adds item to the end of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_append(List* pList, void* pItem){
    assert(pList != NULL);
    Node* temp = pList->tail;

    if (numNodes < LIST_MAX_NUM_NODES){
        temp->next = nodeHead;
        Node* newItem = nodeHead;
        nodeHead = nodeHead->next;

        newItem->prev = temp;
        newItem->item = pItem;
        numNodes++;
        return 0;
    }

    return -1;
}







