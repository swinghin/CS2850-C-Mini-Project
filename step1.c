/**
 * CS2850 Operating Systems
 * C Mini Project - Step 1
 *
 * Wing Hin Shih
 * 100967435
 * wjis132
 */

#include <stdio.h>
#include <stdlib.h>

// node structure
struct node
{
    int i;
    struct node *next;
    struct node *prev;
};

// list handle structure
struct list
{
    struct node *head;
    struct node *tail;
    struct node *right;
    struct node *left;
    int length;
};

void *allocator(int size, int *counter);
void deAllocator(void *p, int *counter);
void initialiseList(struct list *pList, int *counter);
void freeList(struct list *pList, int *counter);
void printList(struct list *pList, int *counter);

// // Listing 1: Explicit initialisation
// int main()
// {
//     int counter = 0;
//     struct list myList;
//     myList.head = malloc(sizeof(struct node));
//     counter++;
//     myList.tail = malloc(sizeof(struct node));
//     counter++;
//     myList.head->next = myList.tail;
//     myList.tail->prev = myList.head;
//     myList.head->prev = NULL;
//     myList.tail->next = NULL;
//     myList.head->i = 0;
//     myList.tail->i = 0;
//     myList.left = myList.head;
//     myList.right = myList.tail;
//     myList.length = 0;
//     printList(&myList, &counter);
//     if (!myList.length && myList.head == myList.left && myList.tail == myList.right)
//     {
//         free(myList.head);
//         counter--;
//         free(myList.tail);
//         counter--;
//         if (!counter)
//         {
//             myList.head = NULL;
//             myList.left = NULL;
//             myList.tail = NULL;
//             myList.right = NULL;
//             myList.length = -1;
//         }
//     }
//     printList(&myList, &counter);
// }

// Listing 2: Compact initialisation
int main()
{
    int counter = 0;
    struct list myList;
    initialiseList(&myList, &counter);
    printList(&myList, &counter);
    freeList(&myList, &counter);
    printList(&myList, &counter);
}

void *allocator(int size, int *counter)
{
    // allocate size bytes of memory by calling malloc
    void *n = malloc(size);

    // check if malloc returned a valid pointer, i.e. a non-null pointer
    if (n != NULL)
    {
        // if so, increase the value of the counter by one
        (*counter)++;

        // and return the pointer returned by malloc.
        return n;
    }
    return NULL;
}

void deAllocator(void *p, int *counter)
{
    // check that the first argument is a valid, i.e. a non-null pointer
    if (p != NULL)
    {
        // if so, free the memory pointed by the first argument
        free(p);

        // decrease the counter by one
        (*counter)--;
    }
}

void initialiseList(struct list *pList, int *counter)
{
    // call allocator to allocate the head and the tail of the list
    pList->head = allocator(sizeof(struct node), counter);
    pList->tail = allocator(sizeof(struct node), counter);

    // initialise the members of the list as in Listing 1
    pList->head->next = pList->tail;
    pList->tail->prev = pList->head;
    pList->head->prev = NULL;
    pList->tail->next = NULL;
    pList->head->i = 0;
    pList->tail->i = 0;
    pList->left = pList->head;
    pList->right = pList->tail;
    pList->length = 0;
}

void freeList(struct list *pList, int *counter)
{
    // check that left points to the same node as head, right points to the same node as tail, and length is zero
    if (!pList->length && pList->head == pList->left && pList->tail == pList->right)
    {
        // if the list is empty, call deAllocator twice to deallocate the head and the tail of the list
        deAllocator(pList->head, counter);
        deAllocator(pList->tail, counter);

        // if the deallocation is successful, set all pointers to NULL and length to -1
        pList->head = NULL;
        pList->left = NULL;
        pList->tail = NULL;
        pList->right = NULL;
        pList->length = -1;
    }
}

// Listing 6: printList
void printList(struct list *pList, int *counter)
{
    struct node *cur = pList->head;
    while (cur)
    {
        printf(" | %d | ", cur->i);
        cur = cur->next;
    }
    if (!(pList->head) || !(pList->tail) || *counter == 0)
        printf(" ||");
    printf("\ncounter = %d\n", *counter);
}