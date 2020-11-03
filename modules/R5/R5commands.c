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

u32int totalSize;

u32int memStart;

void allocateMemLists()
{
    freeList = kmalloc(sizeof(memList));
    allocatedList = kmalloc(sizeof(memList));
}

u32int initializeHeap(u32int heapSize)
{
    CMCB *temp = (CMCB *)kmalloc(heapSize + sizeof(CMCB));
    memStart = &temp;

    // Create the first free block
    temp->type = 'f';
    temp->beginningAddr = memStart + sizeof(CMCB);
    temp->size = heapSize;
    //strcpy(temp->name, "first");
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

    // // Place the LMCB at the bottom of the heap and initialize it.
    // LMCB *temp2 = (&temp->beginningAddr + heapSize);
    // temp2->size = sizeof(LMCB);
    // temp2->type = 'f';

    totalSize = heapSize;

    return memStart;
}

u32int *allocateMemory(u32int size)
{
    CMCB *temp = freeList->head;
    // if (isEmpty()){   /// Not actually sure if this if-else is needed since the while loop would still catch an entire free block
    //
    // }
    // else{
    while ((temp->size < size + sizeof(CMCB)) && (temp->nextCMCB != NULL))
    {
        temp = temp->nextCMCB;
    }
    if (temp->nextCMCB == NULL && temp->size < size + sizeof(CMCB))
    {
        return NULL;
    }
    else if(temp->size == size + sizeof(CMBC)){ // If temp->size = size, no need to create a new CMCB for the remainder, since there is none

    	// Temp becomes an allocated block since the allocation is first fit, temp is the first block large enough.  Easier to just use its already created CMCB
        temp->type = 'a';

        if (allocatedList->count == 0)
        { // If first memory block being allocated
            allocatedList->head = temp;
            allocatedList->tail = temp;
            temp->prevCMCB = NULL;
            temp->nextCMCB = NULL;
            allocatedList->count++;
        }
        else
        { // If not first allocated block, linked in order of beginning address by increasing	address

            CMCB *alreadyAllocated = allocatedList->head;

            while (temp->beginningAddr > alreadyAllocated->beginningAddr && alreadyAllocated->nextCMCB != NULL)
            { // Finding a block with a greater address than the one we are trying to place
                alreadyAllocated = alreadyAllocated->nextCMCB;
            }

            if (alreadyAllocated->nextCMCB == NULL)
            {
                alreadyAllocated->nextCMCB = temp;
                temp->prevCMCB = alreadyAllocated;
                allocatedList->tail = temp;

                allocatedList->count++;
            }
            else
            {
                temp->nextCMCB = alreadyAllocated; // Since the block has a greater address, the new allocated block (temp) comes before it.
                temp->prevCMCB = alreadyAllocated->prevCMCB;
                alreadyAllocated->prevCMCB = temp;

                allocatedList->count++;
            }
        }
        
       return temp->beginningAddr;
    }
    else // if temp->size > size
    {
        CMCB *new = (CMCB *)temp->beginningAddr + size; // This CMCB pertains to the head of the free list at the new memory address
        new->beginningAddr = size + sizeof(CMCB);
        new->size = totalSize - size - sizeof(CMCB);
        new->type = 'f';
        new->nextCMCB = temp->nextCMCB;
        new->prevCMCB = temp->prevCMCB;
        new->prevCMCB->nextCMCB = new;
        new->nextCMCB->prevCMCB = new;

        // Temp becomes an allocated block since the allocation is first fit, temp is the first block large enough.  Easier to just use its already created CMCB
        temp->type = 'a';

        if (allocatedList->count == 0)
        { // If first memory block being allocated
            allocatedList->head = temp;
            allocatedList->tail = temp;
            temp->prevCMCB = NULL;
            temp->nextCMCB = NULL;
            allocatedList->count++;
        }
        else 
        { 
        	// If not first allocated block, linked in order of beginning address by increasing	address
            CMCB *alreadyAllocated = allocatedList->head;

            while (temp->beginningAddr > alreadyAllocated->beginningAddr && alreadyAllocated->nextCMCB != NULL)
            { // Finding a block with a greater address than the one we are trying to place
                alreadyAllocated = alreadyAllocated->nextCMCB;
            }

            if (alreadyAllocated->nextCMCB == NULL)
            {
                alreadyAllocated->nextCMCB = temp;
                temp->prevCMCB = alreadyAllocated;
                allocatedList->tail = temp;

                allocatedList->count++;
            }
            else
            {
                temp->nextCMCB = alreadyAllocated; // Since the block has a greater address, the new allocated bloc (temp) comes before it.
                temp->prevCMCB = alreadyAllocated->prevCMCB;
                alreadyAllocated->prevCMCB = temp;

                allocatedList->count++;
            }
        }

        return temp->beginningAddr;
    }
}

int freeMemory(CMCB *memToFree)
{
    if (isEmpty())
    {
        printMessage("There is no memory to free!\n");
        return 1;
    }
    else if (memToFree->type == 'f')
    {
        printMessage("The memory block you are trying to free is already free!\n");
        return 1;
    }
    else
    {
        // Remove memToFree from the allocatedList.
        if (memToFree == allocatedList->head)
        {
            allocatedList->head = memToFree->nextCMCB;
            memToFree->nextCMCB = NULL;
            allocatedList->count--;
        }
        else if (memToFree == allocatedList->tail)
        {
            allocatedList->tail = memToFree->prevCMCB;
            memToFree->prevCMCB = NULL;
            allocatedList->count--;
        }
        else
        {
            memToFree->prevCMCB->nextCMCB = memToFree->nextCMCB;
            memToFree->nextCMCB->prevCMCB = memToFree->prevCMCB;
            memToFree->nextCMCB = NULL;
            memToFree->prevCMCB = NULL;
            allocatedList->count--;
        }

        // Insert memToFree into the freeList in increasing order.
        CMCB *alreadyFree = freeList->head;
        while (memToFree->beginningAddr > alreadyFree->beginningAddr && alreadyFree->nextCMCB != NULL)
        { // Finding a block with a greater address than the one we are trying to place
            alreadyFree = alreadyFree->nextCMCB;
        }
        if (alreadyFree->nextCMCB == NULL)
        {
            alreadyFree->nextCMCB = memToFree;
            memToFree->prevCMCB = alreadyFree;
            freeList->tail = memToFree;

            freeList->count++;
        }
        else
        {
            memToFree->nextCMCB = alreadyFree; // Since the block has a greater address, the new allocated bloc (temp) comes before it.
            memToFree->prevCMCB = alreadyFree->prevCMCB;
            alreadyFree->prevCMCB = memToFree;

            freeList->count++;
        }

        // Merge memToFree to other free CMCBs if possible.
        CMCB *prev = memToFree->prevCMCB;
        CMCB *next = memToFree->nextCMCB;

        if (((prev->beginningAddr + prev->size) == memToFree->beginningAddr - sizeof(CMCB) - 1) && ((next->beginningAddr - sizeof(CMCB) - 1) == memToFree->beginningAddr + memToFree->size))
        { // merge left and right.
            prev->size += (memToFree->size + next->size);
            prev->nextCMCB = next->nextCMCB;
            next->nextCMCB->prevCMCB = prev;
            memToFree->nextCMCB = NULL;
            memToFree->prevCMCB = NULL;
            next->nextCMCB = NULL;
            next->prevCMCB = NULL;
            freeList->count -= 2;
        }
        else if (((prev->beginningAddr + prev->size) == memToFree->beginningAddr - sizeof(CMCB) - 1))
        { // merge left only.
            prev->size += memToFree->size;
            prev->nextCMCB = memToFree->nextCMCB;
            memToFree->nextCMCB->prevCMCB = prev;
            memToFree->nextCMCB = NULL;
            memToFree->prevCMCB = NULL;
            freeList->count--;
        }
        else if ((next->beginningAddr - sizeof(CMCB) - 1) == memToFree->beginningAddr + memToFree->size)
        { // merge right only.
            memToFree->size += next->size;
            memToFree->nextCMCB = next->nextCMCB;
            next->nextCMCB->prevCMCB = memToFree;
            next->nextCMCB = NULL;
            next->prevCMCB = NULL;
            freeList->count--;
        }
        else
        { // don't merge.
            /* code */
        }
    }
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
        showMCB(temp);
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
        showMCB(temp);
        temp = temp->nextCMCB;
    }
}