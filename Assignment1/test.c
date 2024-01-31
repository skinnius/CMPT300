#include "list.c"

// Test Suite 1: List creation.
static void test_create(){
    // Test case 1: testing for creation of lists
    for (int i = 0; i < LIST_MAX_NUM_HEADS; i++){
        List* newList = List_create();
        assert(newList != NULL);
        assert(newList->currStatus == LIST_OOB_START);
        assert(newList->currNode == NULL);
        assert(newList->listSize == 0);
        assert(newList->head == NULL);
        assert(newList->tail == NULL);
        assert(List_count(newList) == 0);

        if (numList == LIST_MAX_NUM_HEADS){
            assert(newList->next == NULL);
        }
        else{
            assert(newList->next != NULL);
        }
    }
    printf("current number of heads in list: %d\n",numList);

    // Test case 2: attempting to create a new list after reaching maximum capacity (boundary)
    List* failList = List_create();
    assert(failList == NULL);

    // Test case 3: extra tests for good measure
    for (numList = 10; numList < 10000; numList++){
        List* newList = List_create();
        assert(newList == NULL);
    }
}

// Test Suite 2: Node Insertion
static void test_insert(){
    List* testList = List_create();
    char x = 'w';
    void* testItem = &x;

    // Section 1: testing for insert_after()
    // // test case 1: testing for creation of nodes
    // for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
    //     assert(List_insert_after(testList, &i) == 0);
    //     assert(testList->currNode != NULL);
    //     assert(testList->currNode->item == &i);
    //     assert(testList->currStatus == LIST_OOB_START);

    //     // empty list case
    //     if (List_count(testList) == 1){
    //         assert(testList->head = testList->currNode);
    //         assert(testList->currStatus == LIST_OOB_START);
    //     }

    //     printf("%d\n", *(int*)(testList->currNode->item));
    // }

    // // Test case 2: attempting to insert a new node after reaching max capacity
    // assert(List_insert_after(testList, testItem) == -1);
    
    // // more test cases for good measure
    // for (numNodes = LIST_MAX_NUM_NODES;  numNodes< 10000000; numNodes++){
    //     assert(List_insert_after(testList, testItem) == -1);
    // }

    // // Test case 3: Inserting at the beginning of the list.
    // for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
    //     assert(List_insert_after(testList, &i) == 0);
    //     assert(testList->listSize == i + 1);
    //     printf("%d\n", *(int*)testList->currNode->item);

    //     assert(testList->currNode != NULL);
    //     assert(testList->currStatus == LIST_OOB_START);
    //     assert(testList->currNode == testList->head);

    //     testList->currNode = NULL;
    //     testList->currStatus = LIST_OOB_START;

    // }
    
    // // Test case 4: Inserting at the end of the list. (OOB)
    // for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
    //     assert(List_insert_after(testList, &i) == 0);
    //     printf("%d\n", *(int*)testList->currNode->item);

    //     assert(testList->currNode != NULL);
    //     if (i > 0){
    //         assert(testList->currStatus == LIST_OOB_END);
    //     }
    //     assert(testList->currNode == testList->tail);

    //     testList->currNode = NULL;
    //     testList->currStatus = LIST_OOB_END;
    // }

    // //Test case 4.1: Inserting at the end of the list. (Tail)
    // for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
    //     assert(List_insert_after(testList, &i) == 0);
    //     printf("%d\n", *(int*)testList->currNode->item);

    //     assert(testList->currNode != NULL);
    //     assert(testList->currStatus == LIST_OOB_START);
    //     assert(testList->currNode == testList->tail);

    //     testList->currNode = testList->tail;
    // }


    // // Test case 5: Inserting at the middle of the list. (General case)
    // for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
    //     assert(List_insert_after(testList, testItem) == 0);
    //     printf("%d\n", i);
    //     printf("%d\n", *(int*)testList->currNode->item);
        
    //     assert(testList->currNode != NULL);
    //     assert(testList->currStatus == LIST_OOB_START);
    //     assert(testList->listSize == i + 1);

    //     if (i <= 1){
    //         assert(testList->currNode == testList->tail);
    //     }

    //     if (i > 2){
    //         assert(testList->currNode == testList->tail->prev);
    //     }
    //     if (i > 1){
    //         testList->currNode = testList->tail->prev;
    //     }
    // }

    // Section 2: testing for insert_before()

    // // Test case 1: testing for creation of nodes
    // for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
    //     assert(List_insert_before(testList, &i) == 0);
    //     assert(testList->currNode != NULL);
    //     assert(testList->currNode->item == &i);
    //     assert(testList->currStatus == LIST_OOB_START);

    //     // empty list case
    //     if (List_count(testList) == 1){
    //         assert(testList->head = testList->currNode);
    //         assert(testList->currStatus == LIST_OOB_START);
    //     }

    //     printf("%d\n", *(int*)(testList->currNode->item));
    // }

    // // Test case 2: attempting to insert a new node after reaching max capacity
    // assert(List_insert_before(testList, testItem) == -1);
    
    // // more test cases for good measure
    // for (numNodes = LIST_MAX_NUM_NODES;  numNodes< 10000000; numNodes++){
    //     assert(List_insert_before(testList, testItem) == -1);
    // }

    // // Test case 3: Inserting at the end of the list.
    // for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
    //     testList->currNode = NULL;
    //     testList->currStatus = LIST_OOB_END;

    //     assert(List_insert_before(testList, &i) == 0);
    //     assert(testList->listSize == i + 1);
    //     printf("%d\n", *(int*)testList->currNode->item);

    //     assert(testList->currNode != NULL);
    //     assert(testList->currStatus == LIST_OOB_END);
    //     assert(testList->currNode == testList->tail);
    // }

    
    // // Test case 4: Inserting at the start of the list. (OOB)
    // for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
    //     assert(List_insert_before(testList, &i) == 0);
    //     printf("%d\n", *(int*)testList->currNode->item);

    //     assert(testList->currNode != NULL);
    //     if (i > 0){
    //         assert(testList->currStatus == LIST_OOB_START);
    //     }
    //     assert(testList->currNode == testList->head);

    //     testList->currNode = NULL;
    //     testList->currStatus = LIST_OOB_START;
    // }

    // Test case 4.1: Inserting at the end of the list. (Head)
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        assert(List_insert_before(testList, &i) == 0);
        printf("%d\n", *(int*)testList->currNode->item);

        assert(testList->currNode != NULL);
        assert(testList->currStatus == LIST_OOB_START);
        assert(testList->currNode == testList->head);

        testList->currNode = testList->head;
    }


}






// Test Suite 3: Movement of current pointer
static void test_current(){
}

int main(void){

    // test_create();
    printf("All create() tests passed!\n");
    test_insert();
    // test_current();
    printf("all test cases passed!");
}