#include <stdio.h>
#include "list.h"

static List* availableLists[LIST_MAX_NUM_HEADS];    // global statically allocated list of possible unique Lists --> Array of List pointers
static Node availableNodes[LIST_MAX_NUM_NODES];         // global statically allocated list of nodes.

// Makes a new, empty list, and returns its reference on success. 
// Returns a NULL pointer on failure.
List* List_create(){
    List* newList = NULL;      // returns NULL if past LIST_MAX_NUM_HEADS

    // initial setup of the data structure (first call of List_create())
    if (initialSetup){
        for (int i = 0; i < LIST_MAX_NUM_HEADS; i++){
            availableLists[i]->head = NULL;               // initialize everything to NULL  
            availableLists[i]->current = NULL;
        }
        initialSetup = false;   
    }

    if ((numList - 1 ) < LIST_MAX_NUM_NODES){
        availableLists[listSize]->head = &availableNodes[/*empty list node, need to figure out how this works :D*/1];           // head to the new list. need to find an empty list node. 
        newList = availableLists[listSize];

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