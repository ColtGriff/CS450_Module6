// R5 commands
// All of the commands in this file are temporary!

#include <core/serial.h>
#include <string.h>
#include "../mpx_supt.h"
#include <core/io.h>
#include <mem/heap.h>
#include "../utilities.h"
#include "../R2/R2commands.h"
#include "../R2/R2_Internal_Functions_And_Structures.h"
#include "R5commands.h"

memList *freeList;
memList *allocatedList;

u32int memStart;

void allocateMemLists()
{
    freeList = kmalloc(sizeof(memList));
    allocatedList = kmalloc(sizeof(memList));
}

u32int initializeHeap(int heapSize)
{
    CMCB *temp = (CMCB *)kmalloc(heapSize + sizeof(CMCB) + sizeof(LMCB));
    memStart = &temp;

    // Create the first free block
    temp->type = 'f';
    temp->beginningAddr = memStart;
    temp->size = heapSize;
    strcpy(temp->name, "first");
    temp->nextCMCB = NULL;
    temp->prevCMCB = NULL;

    // Initialize free list
    freeList->count = 0;
    freeList->head = NULL;
    freeList->tail = NULL;

    // Initialize alllocated list
    allocatedList->count = 0;
    allocatedList->head = NULL;
    allocatedList->tail = NULL;

    // Place first free block into the free list
    freeList->count++;
    freeList->head = temp;
    freeList->tail = temp;

    // Place the LMCB at the bottom of the heap and initialize it.
    LMCB *temp2 = (&temp->beginningAddr + heapSize);
    temp2->size = sizeof(LMCB);
    temp2->type = 'f';

    return memStart;
}

void *allocateMemory(u32int size)
{
}

void freeMemory()
{
}

int isEmpty()
{
    if (allocatedList->head == NULL && freeList->count == 1)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void showMCB(CMCB *mem)
{
    int sizeLen;

    // Print the block name.
    printMessage("The name of the memory block is: ");
    sizeLen = strLen(mem->name);
    sys_req(WRITE, DEFAULT_DEVICE, mem->name, &sizeLen);
    printMessage(".\n");

    // Print the block type.
    if (mem->type == 'a')
    {
        printMessage("The CMCBs type is: allocated.\n");
    }
    else if (mem->type == 'f')
    {
        printMessage("The CMCBs type is: free.\n");
    }

    // Print the block size.
    char size[20] = itoa(mem->size, size);
    sizeLen = strlen(size);
    printMessage("The size is: ");
    sys_req(WRITE, DEFAULT_DEVICE, size, &sizeLen);
    printMessage(" bytes.\n");

    // Print the block beginning address.
    char temp[20] = itoa((int)mem->beginningAddr, size);
    sizeLen = strlen(temp);
    printMessage("The beginning address of the block is: ");
    sys_req(WRITE, DEFAULT_DEVICE, temp, &sizeLen);
    printMessage(".\n");
}

void showFreeMemory()
{
    if (freeList->head == NULL)
    {
        printMessage("There is no free memory!\n");
    }

    int loop;
    CMCB *temp = freeList->head;
    for (loop = 0; loop <= freeList->count; loop++)
    {
        showMCB(temp->name);
        temp = temp->nextCMCB;
    }
}

void showAllocatedMemory()
{
    if (allocatedList->head == NULL)
    {
        printMessage("There is no allocated memory!\n");
    }

    int loop;
    CMCB *temp = allocatedList->head;
    for (loop = 0; loop <= allocatedList->count; loop++)
    {
        showMCB(temp->name);
        temp = temp->nextCMCB;
    }
}