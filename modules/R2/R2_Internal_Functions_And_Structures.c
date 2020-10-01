//C structures

#include <string.h>
#include "../mpx_supt.h"
#include "R2_Internal_Functions_And_Structures.h"

// Allocate memory for the queues
queue *ready;
queue *blocked;
queue *suspendedReady;
queue *suspendedBlocked;

PCB *allocatePCB() //Returns the created PCB pointer if successful, returns NULL if an error occurs.
{
    //COLTON WILL PROGRAM THIS FUNCTION

    //allocatePCB() will use sys_alloc_mem() to allocate memory for a new PCB, possible including the stack, and perform any reasonable initialization.

    return NULL;
}

int freePCB(PCB *PCB_to_free) //Return 0 is success code, reurn 1 is error code.
{
    // ANASTASE WILL PROGRAM THIS FUNCTION

    //freePCB() will use sys_free_mem() to free all memory associated with a given PCB (the stack, the PCB itself, etc.)

    (void)PCB_to_free;

    return 0;
}

PCB *setupPCB(char *processName, unsigned char processClass, int processPriority) //Returns the created PCB pointer if successful, returns NULL if an error occurs.
{
    //COLTON WILL PROGRAM THIS FUNCTION

    //setupPcb() will call allocatePCB() toto create an empty PCB, initializes the PCB information, sets the PCB state to ready, not suspended.

    (void)processName;
    (void)processClass;
    (void)processPriority;

    return NULL;
}

PCB *findPCB(char *processName) //Returns the created PCB pointer if successful, returns NULL if PCB cannot be found.
{
    // ANASTASE WILL PROGRAM THIS FUNCTION

    //findPCB() will search all queues for a process with a given name.

    (void)processName;

    return NULL;
}

void insertPCB(PCB *PCB_to_insert)
{
    //BENJAMIN WILL PROGRAM THIS FUNCTION

    //insertPCB() will insert a PCB into the appropriate queue.
    //Note: The ready queue is a priority queue and the blocked queue is a FIFO queue.

    if (PCB_to_insert->runningStatus == 0 && PCB_to_insert->suspendedStatus == 1)
    { // Insert into ready queue

        queue *ready = getReady();
        PCB *tempPtr = ready->head;

        if (tempPtr != NULL)
        {
            int temp = 0;
            while (temp < ready->count)
            {
                if (PCB_to_insert->priority < tempPtr->priority)
                {
                    tempPtr = tempPtr->nextPCB;
                }
                else if (PCB_to_insert->priority >= tempPtr->priority)
                {
                    PCB_to_insert->nextPCB = tempPtr;
                    PCB_to_insert->prevPCB = tempPtr->prevPCB;
                    tempPtr->prevPCB = PCB_to_insert;
                }
                else if (PCB_to_insert->priority < tempPtr->priority && tempPtr->nextPCB == NULL)
                {
                    tempPtr->nextPCB = PCB_to_insert;
                    PCB_to_insert->prevPCB = tempPtr;
                }
                temp++;
            }
            ready->count++;
        }
        else
        {
            tempPtr = PCB_to_insert;
            ready->count++;
        }
    }
    else if (PCB_to_insert->runningStatus == 0 && PCB_to_insert->suspendedStatus == 0)
    { // Insert into suspended ready queue
        queue *suspendedReady = getSuspendedReady();
        PCB *tempPtr = suspendedReady->head;

        if (tempPtr != NULL)
        {
            int temp = 0;
            while (temp < suspendedReady->count)
            {
                if (PCB_to_insert->priority < tempPtr->priority)
                {
                    tempPtr = tempPtr->nextPCB;
                }
                else if (PCB_to_insert->priority >= tempPtr->priority)
                {
                    PCB_to_insert->nextPCB = tempPtr;
                    PCB_to_insert->prevPCB = tempPtr->prevPCB;
                    tempPtr->prevPCB = PCB_to_insert;
                }
                else if (PCB_to_insert->priority < tempPtr->priority && tempPtr->nextPCB == NULL)
                {
                    tempPtr->nextPCB = PCB_to_insert;
                    PCB_to_insert->prevPCB = tempPtr;
                }
                temp++;
            }
            suspendedReady->count++;
        }
        else
        {
            tempPtr = PCB_to_insert;
            suspendedReady->count++;
        }
    }
    else if (PCB_to_insert->runningStatus == -1 && PCB_to_insert->suspendedStatus == 1)
    { // Insert into blocked queue
        queue *blocked = getBlocked();
        PCB *tempPtr = blocked->tail;

        tempPtr->nextPCB = PCB_to_insert;
        PCB_to_insert->prevPCB = tempPtr;
    }
    else if (PCB_to_insert->runningStatus == -1 && PCB_to_insert->suspendedStatus == 0)
    { // Insert into suspended blocked queue
        queue *suspendedBlocked = getSuspendedBlocked();
        PCB *tempPtr = suspendedBlocked->tail;

        tempPtr->nextPCB = PCB_to_insert;
        PCB_to_insert->prevPCB = tempPtr;
    }
}

int removePCB(PCB *PCB_to_remove) //Return 0 is success code, return 1 is error code.
{
    //BENJAMIN WILL PROGRAM THIS FUNCTION

    //removePCB() will remove a PCB from the queue in which it is currently stored.

    PCB *tempPrev = PCB_to_remove->prevPCB;
    PCB *tempNext = PCB_to_remove->nextPCB;

    tempPrev->nextPCB = tempNext;
    tempNext->prevPCB = tempPrev;

    PCB_to_remove->nextPCB = NULL;
    PCB_to_remove->prevPCB = NULL;

    int result = sys_free_mem(PCB_to_remove);

    if (result == -1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

queue *getReady()
{
    return ready;
}

queue *getBlocked()
{
    return blocked;
}

queue *getSuspendedReady()
{
    return suspendedReady;
}

queue *getSuspendedBlocked()
{
    return suspendedBlocked;
}