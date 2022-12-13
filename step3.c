/**
 * CS2850 Operating Systems
 * C Mini Project - Step 3
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
void pushInt(struct list *pList, int *counter, int i);
void pullInt(struct list *pList, int *counter, int i);
void clearList(struct list *pList, int *counter);

// Listing 4: Pushing and pulling
int main()
{
    int counter = 0;
    struct list myList;
    initialiseList(&myList, &counter);
    printList(&myList, &counter);
    int N = 6;
    int i = 0;
    while (i < N)
    {
        pushInt(&myList, &counter, i % 9);
        printList(&myList, &counter);
        pullInt(&myList, &counter, 0);
        printList(&myList, &counter);
        i++;
    }
    clearList(&myList, &counter);
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
        struct node *temp = allocator(sizeof(struct node), counter);
        temp->next = pList->left->next;
        temp->prev = pList->left;
        temp->next->prev = temp;
        pList->left->next = temp;
        temp->i = -1;
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
        pList->left->next->next->prev = pList->left;
        pList->left->next = temp->next;
        deAllocator(temp, counter);
        (pList->length)--;
    }
}

void pushInt(struct list *pList, int *counter, int i)
{
    // if there are no free nodes in the gap
    if (pList->length == 0)
    {
        // call allocateBlock, with BLOCKSIZE as a third argument
        allocateBlock(pList, counter, BLOCKSIZE);
    }

    // check if i is odd or even
    if (i % 2 == 1)
    {
        // if i is odd, store it on the first available node on the right of the head-side stack
        pList->left = pList->left->next;
        pList->left->i = i;
    }
    else
    {
        // if i is even, store it on the first available node on the left of the tail-side stack
        pList->right = pList->right->prev;
        pList->right->i = i;
    }
    // decrease empty length after push
    (pList->length)--;
}

void pullInt(struct list *pList, int *counter, int i)
{
    // check if i is odd or even
    if (i % 2 == 1)
    {
        // if i is odd, check that pList−>left != pList−>head, i.e. there is at least one odd integer in the list
        if (pList->left != pList->head)
        {
            // replace the digit in the node pointed by pList->left with -1
            pList->left->i = -1;

            // move pList->left to pList-left->prev
            pList->left = pList->left->prev;

            // increase pList->length by one
            (pList->length)++;
        }
    }
    else
    {
        // if i is even, check that pList−>right != pList−>tail, i.e. there is at least one even integer in the list
        if (pList->right != pList->tail)
        {
            // replace the digit in the node pointed by pList->right with -1
            pList->right->i = -1;

            // move pList->right to pList-right->next
            pList->right = pList->right->next;

            // increase pList->length by one
            (pList->length)++;
        }
    }

    // check if pList−>length ≥B LOCKSIZE and, if so, call deAllocateBlock to remove BLOCKSIZE free nodes from the free space
    if (pList->length > BLOCKSIZE)
    {
        deAllocateBlock(pList, counter, BLOCKSIZE);
    }
}

void clearList(struct list *pList, int *counter)
{
    // remove all digits in the head-side stack by calling pullInt with i = 1 until left points to the head of the list
    while (pList->left != pList->head)
        pullInt(pList, counter, 1);

    // remove all digits in the tail-side stack by calling pullInt with i = 0 until right points to the tail of the list
    while (pList->right != pList->tail)
        pullInt(pList, counter, 0);

    // remove all free nodes by calling deAllocateBlock with length as the third argument
    deAllocateBlock(pList, counter, pList->length);

    // free the list by calling freeList
    freeList(pList, counter);
}