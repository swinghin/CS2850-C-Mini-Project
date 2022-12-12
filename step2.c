/**
 * CS2850 Operating Systems
 * C Mini Project - Step 2
 *
 * Wing Hin Shih
 * 100967435
 * wjis132
 */

#include <stdio.h>
#include <stdlib.h>

#define BLOCKSIZE 1

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
void allocateBlock(struct list *pList, int *counter, int nNodes);
void deAllocateBlock(struct list *pList, int *counter, int nNodes);

// Listing 3: Free node allocation
int main()
{
    int counter = 0;
    struct list myList;
    initialiseList(&myList, &counter);
    printList(&myList, &counter);
    allocateBlock(&myList, &counter, BLOCKSIZE);
    printList(&myList, &counter);
    deAllocateBlock(&myList, &counter, BLOCKSIZE);
    printList(&myList, &counter);
    freeList(&myList, &counter);
    printList(&myList, &counter);
}

void *allocator(int size, int *counter)
{
    void *n = malloc(size);
    if (n != NULL)
    {
        (*counter)++;
        return n;
    }
    return NULL;
}

void deAllocator(void *p, int *counter)
{
    if (p != NULL)
    {
        free(p);
        (*counter)--;
    }
}

void initialiseList(struct list *pList, int *counter)
{
    pList->head = allocator(sizeof(struct node), counter);
    pList->tail = allocator(sizeof(struct node), counter);
    pList->head->next = pList->tail;
    pList->tail->prev = pList->tail;
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
    if (!pList->length && pList->head == pList->left && pList->tail == pList->right)
    {
        deAllocator(pList->head, counter);
        deAllocator(pList->tail, counter);
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

void allocateBlock(struct list *pList, int *counter, int nNodes)
{
    int n = 0;
    while (n < nNodes)
    {
        // get node after head
        struct node *temp = pList->left->next;

        // allocate a new object of type struct node by calling allocator
        struct node *new = allocator(sizeof(struct node), counter);

        // link the new node to the existing ones so that the doubly-linked structure of the list is preserved
        new->next = temp;
        new->prev = pList->left;
        pList->left->next = new;

        // set i of the new node to −1
        new->i = -1;

        // increase pList->length by one
        (pList->length)++;
        n++;
    }
}

// Listing 7: deAllocateBlock
void deAllocateBlock(struct list *pList, int *counter, int nNodes)
{
    int n = (pList->length) - nNodes;
    while ((pList->length) > n)
    {
        struct node *temp = pList->left->next;
        pList->left->next->prev = pList->left;
        pList->left->next = temp->next;
        deAllocator(temp, counter);
        (pList->length)--;
    }
}
