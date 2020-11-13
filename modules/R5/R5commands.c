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

void showMCB(CMCB *mem);

memList freeList;
memList allocatedList;

// void allocateMemLists()
// {
//     freeList = (memList *)kmalloc(sizeof(memList));      //////
//     allocatedList = (memList *)kmalloc(sizeof(memList)); //////   These two were throwing errors so I added the (memList*) typeCast
// }

u32int initializeHeap(u32int heapSize)
{
    u32int memStart = kmalloc(heapSize + sizeof(CMCB));
    CMCB *temp = (CMCB *)memStart;

    // Create the first free block
    temp->type = 'f';
    temp->beginningAddr = memStart + sizeof(CMCB);
    temp->size = heapSize;
    //strcpy(temp->name, "first");
    temp->nextCMCB = NULL;
    temp->prevCMCB = NULL;

    // freeList = (memList *)allocateMemory(sizeof(memList));
    // allocatedList = (memList *)allocateMemory(sizeof(memList));
    // Initialize free list
    // freeList->count = 0;
    // freeList->head = NULL;
    // freeList->tail = NULL;

    // Initialize alllocated list
    allocatedList.count = 0;
    allocatedList.head = NULL;
    allocatedList.tail = NULL;

    // Place first free block into the free list
    freeList.count++;
    freeList.head = temp;
    freeList.tail = temp;

    // // Place the LMCB at the bottom of the heap and initialize it.
    // LMCB *temp2 = (&temp->beginningAddr + heapSize);
    // temp2->size = sizeof(LMCB);
    // temp2->type = 'f';

    return memStart;
}

void insertToList(CMCB *current, memList *list)
{
    if (list->head == NULL) // current is put into an empty list.
    {
        list->head = current;
        list->tail = current;
        list->count++;
    }
    else if (current->beginningAddr < list->head->beginningAddr) // current goes at the start of the list.
    {
        current->nextCMCB = list->head;
        list->head->prevCMCB = current;
        list->head = current;
        list->count++;
    }
    else if (current->beginningAddr > list->tail->beginningAddr) // current goes at the end of the list.
    {
        current->prevCMCB = list->tail;
        list->tail->nextCMCB = current;
        list->tail = current;
        list->count++;
    }
    else // current goes in the middle of list.
    {
        CMCB *temp = list->head;
        while (temp != NULL)
        {
            if (current->beginningAddr < temp->beginningAddr)
            {
                current->nextCMCB = temp;
                current->prevCMCB = temp->prevCMCB;
                temp->prevCMCB->nextCMCB = current;
                temp->prevCMCB = current;
                list->count++;
                break;
            }
            else
            {
                temp = temp->nextCMCB;
            }
        }
    }
}

u32int *allocateMemory(u32int size)
{
    if (freeList.head != NULL)
    {
        CMCB *current = freeList.head;

        // get to block of appropriate size
        while (current != NULL)
        {
            if (current->size == size + sizeof(CMCB) && freeList.count == 1)
            {
                // remove from free list.
                current->nextCMCB->prevCMCB = current->prevCMCB;
                current->prevCMCB->nextCMCB = current->nextCMCB;
                current->nextCMCB = NULL;
                current->prevCMCB = NULL;
                // place current in alloc list.
                insertToList(current, &allocatedList);

                // change current marker to 'a'.
                current->type = 'a';

                // remove all freeList pointers to current.
                freeList.head = NULL;
                freeList.tail = NULL;

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
                insertToList(current, &allocatedList);

                // change current marker to 'a'.
                current->type = 'a';

                // return allocated block.
                return (u32int *)current->beginningAddr;
            }
            else if (current->size > size + sizeof(CMCB)) // current is greater than the size requested
            {
                // remove from free list.
                CMCB *new = (CMCB *)(current->beginningAddr + size);                 // This CMCB pertains to the head of the free list at the new memory address
                new->beginningAddr = (current->beginningAddr + size + sizeof(CMCB)); // Could be tmp->beginningAddr + size + sizeof(CMCB)
                new->size = current->size - size - sizeof(CMCB);
                new->type = 'f';
                new->nextCMCB = current->nextCMCB;
                new->prevCMCB = current->prevCMCB;

                if (current->prevCMCB != NULL)
                {
                    new->prevCMCB->nextCMCB = new;
                }

                if (current->nextCMCB != NULL)
                {
                    new->nextCMCB->prevCMCB = new;
                }

                if (freeList.head == current && freeList.tail == current)
                {
                    freeList.head = new;
                    freeList.tail = new;
                }
                else if (freeList.head == current)
                {
                    freeList.head = new;
                }
                else if (freeList.tail == current)
                {
                    freeList.tail = new;
                }

                current->size = size;
                current->nextCMCB = NULL;
                current->prevCMCB = NULL;

                // place current in alloc list.
                insertToList(current, &allocatedList);

                // change current marker to 'a'.
                current->type = 'a';

                // return allocated block.
                return (u32int *)current->beginningAddr;
            }
            current = current->nextCMCB;
        }
    }

    return NULL;
}

void removeFromAlloc(CMCB *temp)
{
    // Remove temp from allocatedList
    if (temp == allocatedList.head)
    {
        allocatedList.head = temp->nextCMCB;
        temp->nextCMCB = NULL;
        allocatedList.count--;
    }
    else if (temp == allocatedList.tail)
    {
        allocatedList.tail = temp->prevCMCB;
        allocatedList.tail->nextCMCB = NULL;
        temp->prevCMCB = NULL;
        allocatedList.count--;
    }
    else
    {
        temp->prevCMCB->nextCMCB = temp->nextCMCB;
        temp->nextCMCB->prevCMCB = temp->prevCMCB;
        temp->nextCMCB = NULL;
        temp->prevCMCB = NULL;
        allocatedList.count--;
    }
}

int freeMemory(u32int *memToFree) /////////// Needs return statements
{
    if (isEmpty())
    {
        printMessage("There is no memory to free!\n");
        return 1;
    }

    CMCB *temp = allocatedList.head;

    if (temp == NULL)
    {
        printMessage("There is no allocated memory at that address!\n");
        return 1;
    }
    else
    {
        while (temp->beginningAddr != (u32int)memToFree)
        {
            temp = temp->nextCMCB;
        }

        if (temp == NULL)
        {
            printMessage("There is no allocated memory at that address!\n");
            return 1;
        }
        else
        {

            // Remove memToFree from the allocatedList.
            removeFromAlloc(temp);

            // Insert memToFree into the freeList in increasing order.
            insertToList(temp, &freeList);
            temp->type = 'f';

            // Merge memToFree to other free CMCBs if possible.
            if (freeList.count >= 1)
            {
                CMCB *temp = freeList.head;

                while (temp != NULL)
                {
                    if ((temp->beginningAddr + temp->size) == (temp->nextCMCB->beginningAddr - sizeof(CMCB)))
                    {
                        if (temp->nextCMCB->nextCMCB != NULL)
                        {
                            temp->size += (temp->nextCMCB->size + sizeof(CMCB));
                            temp->nextCMCB = temp->nextCMCB->nextCMCB;
                            temp->nextCMCB->nextCMCB->prevCMCB = temp;
                            temp->nextCMCB->nextCMCB = NULL;
                            temp->nextCMCB->prevCMCB = NULL;
                            freeList.count--;
                        }
                        else
                        {
                            temp->size += (temp->nextCMCB->size + sizeof(CMCB));
                            temp->nextCMCB->nextCMCB = NULL;
                            temp->nextCMCB->prevCMCB = NULL;
                            temp->nextCMCB = NULL;
                            freeList.count--;
                        }
                    }
                    else
                    {
                        temp = temp->nextCMCB;
                    }
                }
            }
            else
            {
                freeList.head = temp;
                freeList.tail = temp;
                freeList.count = 1;
            }
        }
    } // end of else statement to free memory.
    return 0;
} // end of Function.

int isEmpty()
{
    if (allocatedList.head == NULL && freeList.count == 1)
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
    memset(size, '\0', 20);
    strcpy(size, itoa(mem->size, size));
    sizeLen = strlen(size);
    printMessage("The size is: ");
    sys_req(WRITE, DEFAULT_DEVICE, size, &sizeLen);
    printMessage(" bytes.\n");

    // Print the block beginning address.
    char temp[20];
    memset(temp, '\0', 20);
    strcpy(temp, itoa(mem->beginningAddr, temp));
    sizeLen = strlen(temp);
    printMessage("The beginning address of the block is: ");
    sys_req(WRITE, DEFAULT_DEVICE, temp, &sizeLen);
    printMessage(".\n\n");
}

void showFreeMemory()
{
    if (freeList.head == NULL)
    {
        printMessage("There is no free memory!\n");
    }

    CMCB *temp = freeList.head;
    while (temp != NULL)
    {
        showMCB(temp);
        temp = temp->nextCMCB;
    }
}

void showAllocatedMemory()
{
    if (allocatedList.head == NULL)
    {
        printMessage("There is no allocated memory!\n");
    }

    CMCB *temp = allocatedList.head;
    while (temp != NULL)
    {
        showMCB(temp);
        temp = temp->nextCMCB;
    }
}