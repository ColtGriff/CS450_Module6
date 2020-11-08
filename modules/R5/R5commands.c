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
#include "../R1/R1commands.h"

memList *freeList;
memList *allocatedList;

u32int totalSize;

u32int memStart;

void allocateMemLists()
{
    freeList = (memList *)kmalloc(sizeof(memList));      //////
    allocatedList = (memList *)kmalloc(sizeof(memList)); //////   These two were throwing errors so I added the (memList*) typeCast
}

u32int initializeHeap(u32int heapSize)
{
    CMCB *temp = (CMCB *)kmalloc(heapSize + sizeof(CMCB));
    memStart = sizeof(CMCB) + 1; //////////////// This is throwing errors.  Unsure if needs to be temp->beginningAddress, or temp->beginningAddress - sizeof(CMCB)

    // Create the first free block
    temp->type = 'f';
    temp->beginningAddr = memStart;
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

void insertToList(CMCB *current, memList *list)
{
    if (isEmpty())
    {
        list->head = current;
        list->tail = current;
        list->count++;
    }
    else if (current->beginningAddr < list->head->beginningAddr)
    {
        current->nextCMCB = list->head;
        list->head->prevCMCB = current;
        list->head = current;
        list->count++;
    }
    else if (current->beginningAddr > list->tail->beginningAddr)
    {
        current->prevCMCB = list->tail;
        list->tail->nextCMCB = current;
        list->tail = current;
        list->count++;
    }
    else
    {
        // current goes in the middle of allocatedList.
        CMCB *temp = list->head;
        while (current->beginningAddr > temp->beginningAddr && temp->nextCMCB != NULL)
        {
            temp = temp->nextCMCB;
        }

        current->nextCMCB = temp;
        current->prevCMCB = temp->prevCMCB;
        temp->prevCMCB->nextCMCB = current;
        temp->prevCMCB = current;
        list->count++;
    }
}

u32int *allocateMemory(u32int size)
{

    if (freeList->head != NULL)
    {
        CMCB *current = freeList->head;

        // get to block of appropriate size
        while (current->nextCMCB != NULL && current->size < size + sizeof(CMCB))
        {
            current = current->nextCMCB;
        }

        // found block of appropriate size.
        if (freeList->tail == current && current->size < size + sizeof(CMCB)) //current can't alloc size, and is the last in the list.
        {
            return NULL;
        }
        else if (current->size == size + sizeof(CMCB) && freeList->count == 1)
        {
            // remove from free list.
            current->nextCMCB->prevCMCB = current->prevCMCB;
            current->prevCMCB->nextCMCB = current->nextCMCB;
            current->nextCMCB = NULL;
            current->prevCMCB = NULL;
            // place current in alloc list.
            insertToList(current, allocatedList);

            // change current marker to 'a'.
            current->type = 'a';

            // remove all freeList pointers to current.
            freeList->head = NULL;
            freeList->tail = NULL;

            // return allocated block.
            return (u32int *)current->beginningAddr;
        }
        else if (current->size == size + sizeof(CMCB)) // current is excetly the size requested.
        {
            // remove from free list.
            current->nextCMCB->prevCMCB = current->prevCMCB;
            current->prevCMCB->nextCMCB = current->nextCMCB;
            current->nextCMCB = NULL;
            current->prevCMCB = NULL;
            // place current in alloc list.
            insertToList(current, allocatedList);

            // change current marker to 'a'.
            current->type = 'a';

            // return allocated block.
            return (u32int *)current->beginningAddr;
        }
        else // current is greater than the size requested
        {
            // remove from free list.
            CMCB *new = (CMCB *)current->beginningAddr + size;                 // This CMCB pertains to the head of the free list at the new memory address
            new->beginningAddr = current->beginningAddr + size + sizeof(CMCB); // Could be tmp->beginningAddr + size + sizeof(CMCB)
            new->size = current->size - size - sizeof(CMCB);
            new->type = 'f';
            new->nextCMCB = current->nextCMCB;
            new->prevCMCB = current->prevCMCB;
            new->prevCMCB->nextCMCB = new;
            new->nextCMCB->prevCMCB = new;

            if (freeList->head == current && freeList->tail == current)
            {
                freeList->head = new;
                freeList->tail = new;
            }
            else if (freeList->head == current)
            {
                freeList->head = new;
            }
            else if (freeList->tail == current)
            {
                freeList->tail = new;
            }

            current->size -= new->size;
            current->nextCMCB = NULL;
            current->prevCMCB = NULL;

            // place current in alloc list.
            insertToList(current, allocatedList);

            // change current marker to 'a'.
            current->type = 'a';

            // return allocated block.
            return (u32int *)current->beginningAddr;
        }
    }
    else
    {
        return NULL;
    }

    // if (freeList->head != NULL)
    // {
    //     CMCB *temp = freeList->head;

    //     while ((temp->size < size + sizeof(CMCB)) && (temp->nextCMCB != NULL))
    //     {
    //         temp = temp->nextCMCB;
    //     }
    //     if (temp->nextCMCB == NULL && temp->size < size + sizeof(CMCB))
    //     {
    //         return NULL;
    //     }
    //     else if (temp->size == size + sizeof(CMCB))
    //     { // If temp->size = size, no need to create a new CMCB for the remainder, since there is none

    //         // Temp becomes an allocated block since the allocation is first fit, temp is the first block large enough.  Easier to just use its already created CMCB
    //         temp->type = 'a';

    //         if (allocatedList->count == 0)
    //         { // If first memory block being allocated
    //             allocatedList->head = temp;
    //             allocatedList->tail = temp;
    //             temp->prevCMCB = NULL;
    //             temp->nextCMCB = NULL;
    //             allocatedList->count++;
    //         }
    //         else
    //         { // If not first allocated block, linked in order of beginning address by increasing	address

    //             CMCB *alreadyAllocated = allocatedList->head;

    //             if (temp->beginningAddr < alreadyAllocated->beginningAddr)
    //             {
    //                 alreadyAllocated->prevCMCB = temp;
    //                 temp->nextCMCB = alreadyAllocated;
    //                 allocatedList->head = temp;
    //                 allocatedList->count++;
    //             }
    //             else
    //             {
    //                 while (temp->beginningAddr > alreadyAllocated->beginningAddr && alreadyAllocated->nextCMCB != NULL)
    //                 { // Finding a block with a greater address than the one we are trying to place
    //                     alreadyAllocated = alreadyAllocated->nextCMCB;
    //                 }

    //                 if (alreadyAllocated->nextCMCB == NULL)
    //                 {
    //                     alreadyAllocated->nextCMCB = temp;
    //                     temp->prevCMCB = alreadyAllocated;
    //                     allocatedList->tail = temp;

    //                     allocatedList->count++;
    //                 }
    //                 else
    //                 {
    //                     temp->nextCMCB = alreadyAllocated; // Since the block has a greater address, the new allocated block (temp) comes before it.
    //                     temp->prevCMCB = alreadyAllocated->prevCMCB;
    //                     alreadyAllocated->prevCMCB = temp;

    //                     allocatedList->count++;
    //                 }
    //             }
    //         }

    //         return (u32int *)temp->beginningAddr;
    //     }
    //     else // if temp->size > size
    //     {
    //         CMCB *new = (CMCB *)temp->beginningAddr + size; // This CMCB pertains to the head of the free list at the new memory address
    //         new->beginningAddr = size + sizeof(CMCB);       // Could be tmp->beginningAddr + size + sizeof(CMCB)
    //         new->size = totalSize - size - sizeof(CMCB);
    //         new->type = 'f';
    //         new->nextCMCB = temp->nextCMCB;
    //         new->prevCMCB = temp->prevCMCB;
    //         new->prevCMCB->nextCMCB = new;
    //         new->nextCMCB->prevCMCB = new;

    //         // Temp becomes an allocated block since the allocation is first fit, temp is the first block large enough.  Easier to just use its already created CMCB
    //         temp->type = 'a';

    //         if (allocatedList->count == 0)
    //         { // If first memory block being allocated
    //             allocatedList->head = temp;
    //             allocatedList->tail = temp;
    //             temp->prevCMCB = NULL;
    //             temp->nextCMCB = NULL;
    //             allocatedList->count++;
    //         }
    //         else
    //         {
    //             // If not first allocated block, linked in order of beginning address by increasing	address
    //             CMCB *alreadyAllocated = allocatedList->head;

    //             if (temp->beginningAddr < alreadyAllocated->beginningAddr)
    //             {
    //                 alreadyAllocated->prevCMCB = temp;
    //                 temp->nextCMCB = alreadyAllocated;
    //                 allocatedList->head = temp;
    //                 allocatedList->count++;
    //             }
    //             else
    //             {

    //                 while (temp->beginningAddr > alreadyAllocated->beginningAddr && alreadyAllocated->nextCMCB != NULL)
    //                 { // Finding a block with a greater address than the one we are trying to place
    //                     alreadyAllocated = alreadyAllocated->nextCMCB;
    //                 }

    //                 if (alreadyAllocated->nextCMCB == NULL)
    //                 {
    //                     alreadyAllocated->nextCMCB = temp;
    //                     temp->prevCMCB = alreadyAllocated;
    //                     allocatedList->tail = temp;

    //                     allocatedList->count++;
    //                 }
    //                 else
    //                 {
    //                     temp->nextCMCB = alreadyAllocated; // Since the block has a greater address, the new allocated bloc (temp) comes before it.
    //                     temp->prevCMCB = alreadyAllocated->prevCMCB;
    //                     alreadyAllocated->prevCMCB = temp;

    //                     allocatedList->count++;
    //                 }
    //             }
    //         }

    //         return (u32int *)temp->beginningAddr;
    //     }
    // }
    // else
    // {
    //     return NULL;
    // }
}

int freeMemory(CMCB *memToFree) /////////// Needs return statements
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
        if (memToFree->nextCMCB != NULL && memToFree->prevCMCB != NULL)
        {
            CMCB *prev = memToFree->prevCMCB;
            CMCB *next = memToFree->nextCMCB;

            if (next->nextCMCB != NULL)
            {
                if (((prev->beginningAddr + prev->size) == memToFree->beginningAddr - sizeof(CMCB) - 1) && ((next->beginningAddr - sizeof(CMCB) - 1 == memToFree->beginningAddr + memToFree->size)))
                {
                    prev->size += (memToFree->size + next->size);
                    prev->nextCMCB = next->nextCMCB;
                    next->nextCMCB->prevCMCB = prev;
                    memToFree->nextCMCB = NULL;
                    memToFree->prevCMCB = NULL;
                    next->nextCMCB = NULL;
                    next->prevCMCB = NULL;
                    freeList -= 2;
                }
                else if ((prev->beginningAddr + prev->size) == memToFree->beginningAddr - sizeof(CMCB) - 1)
                {
                    prev->size += memToFree->size;
                    prev->nextCMCB = memToFree->nextCMCB;
                    memToFree->nextCMCB->prevCMCB = prev;
                    memToFree->nextCMCB = NULL;
                    memToFree->prevCMCB = NULL;
                    freeList->count--;
                }
                else if ((next->beginningAddr - sizeof(CMCB) - 1) == memToFree->beginningAddr + memToFree->size)
                {
                    memToFree->size += next->size;
                    memToFree->nextCMCB = next->nextCMCB;
                    next->nextCMCB->prevCMCB = memToFree;
                    next->nextCMCB = NULL;
                    next->prevCMCB = NULL;
                    freeList->count--;
                }
            }
            else
            {
                if (((prev->beginningAddr + prev->size) == memToFree->beginningAddr - sizeof(CMCB) - 1) && ((next->beginningAddr - sizeof(CMCB) - 1 == memToFree->beginningAddr + memToFree->size)))
                {
                    prev->size += (memToFree->size + next->size);
                    prev->nextCMCB = NULL;
                    memToFree->nextCMCB = NULL;
                    memToFree->prevCMCB = NULL;
                    next->nextCMCB = NULL;
                    next->prevCMCB = NULL;
                    freeList -= 2;
                }
                else if ((prev->beginningAddr + prev->size) == memToFree->beginningAddr - sizeof(CMCB) - 1)
                {
                    prev->size += memToFree->size;
                    prev->nextCMCB = memToFree->nextCMCB;
                    memToFree->nextCMCB->prevCMCB = prev;
                    memToFree->nextCMCB = NULL;
                    memToFree->prevCMCB = NULL;
                    freeList->count--;
                }
                else if ((next->beginningAddr - sizeof(CMCB) - 1) == memToFree->beginningAddr + memToFree->size)
                {
                    memToFree->size += next->size;
                    memToFree->nextCMCB = NULL;
                    next->nextCMCB = NULL;
                    next->prevCMCB = NULL;
                    freeList->count--;
                }
            }
        }
        else if (memToFree->nextCMCB != NULL)
        {
            CMCB *next = memToFree->nextCMCB;

            if (next->nextCMCB != NULL)
            {
                if ((next->beginningAddr - sizeof(CMCB) - 1) == memToFree->beginningAddr + memToFree->size)
                {
                    memToFree->size += next->size;
                    memToFree->nextCMCB = next->nextCMCB;
                    next->nextCMCB->prevCMCB = memToFree;
                    next->nextCMCB = NULL;
                    next->prevCMCB = NULL;
                    freeList->count--;
                }
            }
            else
            {
                if ((next->beginningAddr - sizeof(CMCB) - 1) == memToFree->beginningAddr + memToFree->size)
                {
                    memToFree->size += next->size;
                    memToFree->nextCMCB = NULL;
                    next->nextCMCB = NULL;
                    next->prevCMCB = NULL;
                    freeList->count--;
                }
            }
        }
        else if (memToFree->prevCMCB != NULL)
        {
            CMCB *prev = memToFree->prevCMCB;

            if ((prev->beginningAddr + prev->size) == memToFree->beginningAddr - sizeof(CMCB) - 1)
            {
                prev->size += memToFree->size;
                prev->nextCMCB = NULL;
                memToFree->nextCMCB = NULL;
                memToFree->prevCMCB = NULL;
                freeList->count--;
            }
        }
        else
        {
            freeList->head = memToFree;
            freeList->tail = memToFree;
            freeList->count--;
        }
    } // end of else statement to free memory.
    return 0;
} // end of Function.

int isEmpty()
{
    if (allocatedList->head == NULL && freeList->count == 1)
    {
        printMessage("The allocated list is empty.\n");
        return TRUE;
    }
    else
    {
        printMessage("The allocated list is not empty.\n");
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
    char size[20];
    strcpy(size, itoa(mem->size, size));
    sizeLen = strlen(size);
    printMessage("The size is: ");
    sys_req(WRITE, DEFAULT_DEVICE, size, &sizeLen);
    printMessage(" bytes.\n");

    // Print the block beginning address.
    char temp[20];
    strcpy(temp, itoa((int)mem->beginningAddr, size));
    sizeLen = strlen(temp);
    printMessage("The beginning address of the block is: ");
    sys_req(WRITE, DEFAULT_DEVICE, temp, &sizeLen);
    printMessage(".\n\n");
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

memList *getFree()
{
    return freeList;
}

memList *getAlloc()
{
    return allocatedList;
}