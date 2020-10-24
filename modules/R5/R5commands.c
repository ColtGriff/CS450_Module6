// R5 commands
// All of the commands in this file are temporary!

#include <core/serial.h>
#include <string.h>
#include "../mpx_supt.h"
#include <core/io.h>
#include <mem/heap.h>
#include "../R2/R2_Internal_Functions_And_Structures.h"
#include "R5commands.h"

queue *freeList;
queue *allocatedList;

u32int memStart;

u32int initializeHeap(int heapSize)
{
    memStart = kmalloc(heapSize + sizeof(CMCB) + sizeof(LMCB));

    freeList->count = 0;
    freeList->head = NULL;
    freeList->tail = NULL;

    allocatedList->count = 0;
    allocatedList->head = NULL;
    allocatedList->tail = NULL;

    return memStart;
}

void allocateMemory()
{
}

void freeMemory()
{
}

void isEmpty()
{
}

void showFreeMemory()
{
}

void showAllocatedMemory()
{
}