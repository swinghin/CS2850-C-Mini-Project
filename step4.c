/**
 * CS2850 Operating Systems
 * C Mini Project - Step 4
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
int getInput(char *s);

static int maxInput = 5;

// Listing 5: main
int main()
{
    int counter = 0;
    struct list myList;
    initialiseList(&myList, &counter);
    printList(&myList, &counter);
    char sPush[maxInput];
    char sPull[maxInput];
    int lenPush = getInput(sPush);
    int lenPull = getInput(sPull);
    int i = 0;
    int j = 0;
    while ((i + j) < (lenPush + lenPull))
    {
        if (i < lenPush)
        {
            pushInt(&myList, &counter, *(sPush + i) - 48);
            i++;
            printList(&myList, &counter);
        }
        if (j < lenPull)
        {
            pullInt(&myList, &counter, *(sPull + j) - 48);
            j++;
            printList(&myList, &counter);
        }
    }
    printList(&myList, &counter);
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
    if (pList->length == 0)
        allocateBlock(pList, counter, BLOCKSIZE);
    if (i % 2 == 1)
    {
        pList->left = pList->left->next;
        pList->left->i = i;
    }
    else
    {
        pList->right = pList->right->prev;
        pList->right->i = i;
    }
    (pList->length)--;
}

void pullInt(struct list *pList, int *counter, int i)
{
    if (i % 2 == 1)
    {
        if (pList->left != pList->head)
        {
            pList->left->i = -1;
            pList->left = pList->left->prev;
            (pList->length)++;
        }
    }
    else
    {
        if (pList->right != pList->tail)
        {
            pList->right->i = -1;
            pList->right = pList->right->next;
            (pList->length)++;
        }
    }
    if (pList->length > BLOCKSIZE)
        deAllocateBlock(pList, counter, BLOCKSIZE);
}

void clearList(struct list *pList, int *counter)
{
    while (pList->left != pList->head)
        pullInt(pList, counter, 1);
    while (pList->right != pList->tail)
        pullInt(pList, counter, 0);
    deAllocateBlock(pList, counter, pList->length);
    freeList(pList, counter);
}

int getInput(char *s)
{
    // declare a variable, c, of type char
    char c;
    // declare a variable, i, of type int
    int i;
    // set c to the null-character
    c = '\0';
    // set i to 0
    i = 0;

    // while c is not a new line character and i is smaller than maxInput do
    while (c != '\n' && i < maxInput)
    {
        // read a new character from the terminal and store it in c
        c = getchar();

        // if c is a digit, i.e. c ∈ {0, . . . , 9} then
        if (c >= '0' && c <= '9')
        {
            // copy c to the ith entry of s
            *(s + i) = c;

            // increase i by one
            i++;
        }
    }

    // if i is equal to maxInput then
    if (i == maxInput)
    {
        // while c is not a new line character do
        while (c != '\n')
        {
            // read a new character from the terminal and do nothing
            c = getchar();
        }
    }

    // return i
    return i;
}