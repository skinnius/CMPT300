#include "list.c"

static void test_create(){
    // Test suite 1: testing for creation of lists
    for (numList = 0; numList < 10; numList++){
        List* newList = List_create();
        assert(newList != NULL);
        assert(newList->current == LIST_OOB_START);
        assert(newList->currNode == NULL);
        assert(newList->listSize = 0);
    }
    
    for (numList = 10; numList < 10000; numList++){
        List* newList = List_create();
        assert(newList == NULL);
    }

}


int main(void){

    test_create();
    printf("all test cases passed!");
}