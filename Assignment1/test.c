#include "list.c"

static void test_create(){
    // Test suite 1: testing for creation of lists
    for (int i = 0; i < LIST_MAX_NUM_HEADS; i++){
        List* newList = List_create();
        assert(newList != NULL);
        assert(newList->currStatus == LIST_OOB_START);
        assert(newList->currNode == NULL);
        assert(newList->listSize == 0);
        assert(newList->head == NULL);
        assert(newList->tail == NULL);


        if (numList == LIST_MAX_NUM_HEADS){
            assert(newList->next == NULL);
        }
        else{
            assert(newList->next != NULL);
        }
    }
    printf("current number of heads in list: %d\n",numList);

    // attempting to create a new list after reaching maximum capacity
    List* failList = List_create();
    assert(failList == NULL);

    // extra tests for good measure
    for (numList = 10; numList < 10000; numList++){
        List* newList = List_create();
        assert(newList == NULL);
    }

    

}


int main(void){

    test_create();
    printf("all test cases passed!");
}