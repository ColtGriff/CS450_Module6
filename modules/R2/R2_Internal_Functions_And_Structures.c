//C structures

#include <string.h>
#include <core/serial.h>
#include "../mpx_supt.h"
#include "R2_Internal_Functions_And_Structures.h"
#include "../R3/R3commands.h"


queue *ready;
queue *blocked;
queue *suspendedReady;
queue *suspendedBlocked;

// a function to allocate memory for the queues and initialize the queues.

PCB *allocatePCB() //Returns the created PCB pointer if successful, returns NULL if an error occurs.
{
    //COLTON WILL PROGRAM THIS FUNCTION

    //allocatePCB() will use sys_alloc_mem() to allocate memory for a new PCB, possible including the stack, and perform any reasonable initialization.
    PCB *newPCB = (PCB *)sys_alloc_mem(sizeof(PCB));

    char name[20] = "newPCB";
    strcpy(newPCB->processName, name);

    newPCB->suspendedStatus = 1;
    newPCB->runningStatus = -1;
    newPCB->stackTop = (newPCB->stack + 1024) - sizeof(context);
    newPCB->stackBase = newPCB->stack;
    newPCB->priority = 0;

    // Setting the PCBs prev and next PCB
    newPCB->nextPCB = NULL;
    newPCB->prevPCB = NULL;

    newPCB->processClass = NULL;

    return newPCB;
}

int freePCB(PCB *PCB_to_free) //Return 0 is success code, reurn 1 is error code.
{
    // ANASTASE WILL PROGRAM THIS FUNCTION

    //freePCB() will use sys_free_mem() to free all memory associated with a given PCB (the stack, the PCB itself, etc.)

    return sys_free_mem(PCB_to_free);
}

PCB *setupPCB(char *processName, unsigned char processClass, int processPriority) //Returns the created PCB pointer if successful, returns NULL if an error occurs.
{
    //COLTON WILL PROGRAM THIS FUNCTION

    //setupPcb() will call allocatePCB() to create an empty PCB, initializes the PCB information, sets the PCB state to ready, not suspended.

    PCB *returnedPCB = allocatePCB();

    if (findPCB(processName)->processName == processName)
    {
        char message[] = "There is already a PCB with this name.\n";
        int messLength = strlen(message);
        sys_req(WRITE, DEFAULT_DEVICE, message, &messLength);

        returnedPCB = NULL;
    }
    else
    {

        strcpy(returnedPCB->processName, processName);
        returnedPCB->processClass = processClass;
        returnedPCB->priority = processPriority;
        returnedPCB->runningStatus = 0;
        returnedPCB->suspendedStatus = 1;
        returnedPCB->stackBase = returnedPCB->stack;
        returnedPCB->stackTop = returnedPCB->stack + 1024 - sizeof(context);
        returnedPCB->nextPCB = NULL;
        returnedPCB->prevPCB = NULL;
    }

    return returnedPCB;
}

PCB *findPCB(char *processName) //Returns the created PCB pointer if successful, returns NULL if PCB cannot be found.
{
    // ANASTASE WILL PROGRAM THIS FUNCTION

    //findPCB() will search all queues for a process with a given name.

    if (strlen(processName) > 20)
    {

        char error_message[30] = "Invalid process name.\n";
        int error_size = strlen(error_message);
        sys_req(WRITE, DEFAULT_DEVICE, error_message, &error_size);
        return NULL;
        //return cz we have to stop if the process name is too long
    }
    else
    {
        PCB *tempPCB = ready->head;
        int value = 0;
        while (value < ready->count)
        {
            if (strcmp(tempPCB->processName, processName) == 0)
            {
                return tempPCB;
            }
            else
            {
                tempPCB = tempPCB->nextPCB;
                value++;
            }
        }

        tempPCB = blocked->head;
        value = 0;
        while (value < blocked->count)
        {
            if (strcmp(tempPCB->processName, processName) == 0)
            {
                return tempPCB;
            }
            else
            {
                tempPCB = tempPCB->nextPCB;
                value++;
            }
        }

        tempPCB = suspendedBlocked->head;
        value = 0;
        while (value < suspendedBlocked->count)
        {
            if (strcmp(tempPCB->processName, processName) == 0)
            {
                return tempPCB;
            }
            else
            {
                tempPCB = tempPCB->nextPCB;
                value++;
            }
        }

        tempPCB = suspendedReady->head;
        value = 0;
        while (value < suspendedReady->count)
        {
            if (strcmp(tempPCB->processName, processName) == 0)
            {
                return tempPCB;
            }
            else
            {
                tempPCB = tempPCB->nextPCB;
                value++;
            }
        }

        return NULL;
    }
}

void insertPCB(PCB *PCB_to_insert)
{
    //BENJAMIN WILL PROGRAM THIS FUNCTION

    //insertPCB() will insert a PCB into the appropriate queue.
    //Note: The ready queue is a priority queue and the blocked queue is a FIFO queue.

    if (PCB_to_insert->runningStatus == 0 && PCB_to_insert->suspendedStatus == 1)
    { // Insert into ready queue
        PCB *tempPtr = ready->head;

        if (tempPtr != NULL)
        {
            int temp = 0;
            while (temp < ready->count)
            {
                if (PCB_to_insert->priority > ready->head->priority)
                { // insert at head
                    PCB_to_insert->nextPCB = tempPtr;
                    tempPtr->prevPCB = PCB_to_insert;
                    ready->head = PCB_to_insert;
                    ready->count++;
                    break;
                }
                else if (PCB_to_insert->priority <= ready->tail->priority)
                { // insert at tail
                    ready->tail->nextPCB = PCB_to_insert;
                    PCB_to_insert->prevPCB = ready->tail;
                    ready->tail = PCB_to_insert;
                    ready->count++;
                    break;
                }
                else if (PCB_to_insert->priority > tempPtr->priority)
                { // insert at middle
                    PCB *prevPtr = tempPtr->prevPCB;

                    prevPtr->nextPCB = PCB_to_insert;

                    PCB_to_insert->prevPCB = prevPtr;
                    PCB_to_insert->nextPCB = tempPtr;

                    tempPtr->prevPCB = PCB_to_insert;

                    ready->count++;
                    break;
                }
                else
                { // move tempPtr through the queue
                    tempPtr = tempPtr->nextPCB;
                }
                temp++;
            }
            //ready->count++;
        }
        else
        {
            ready->head = PCB_to_insert;
            ready->tail = PCB_to_insert;
            ready->count++;
        }
    }
    else if (PCB_to_insert->runningStatus == 0 && PCB_to_insert->suspendedStatus == 0)
    { // Insert into suspended ready queue
        PCB *tempPtr = suspendedReady->head;

        if (tempPtr != NULL)
        {
            int temp = 0;
            while (temp < suspendedReady->count)
            {
                if (PCB_to_insert->priority > suspendedReady->head->priority)
                { // insert at head
                    PCB_to_insert->nextPCB = tempPtr;
                    tempPtr->prevPCB = PCB_to_insert;
                    suspendedReady->head = PCB_to_insert;
                    suspendedReady->count++;
                    break;
                }
                else if (PCB_to_insert->priority <= suspendedReady->tail->priority)
                { // insert at tail
                    suspendedReady->tail->nextPCB = PCB_to_insert;
                    PCB_to_insert->prevPCB = suspendedReady->tail;
                    suspendedReady->tail = PCB_to_insert;
                    suspendedReady->count++;
                    break;
                }
                else if (PCB_to_insert->priority > tempPtr->priority)
                { // insert at middle
                    PCB *prevPtr = tempPtr->prevPCB;

                    prevPtr->nextPCB = PCB_to_insert;

                    PCB_to_insert->prevPCB = prevPtr;
                    PCB_to_insert->nextPCB = tempPtr;

                    tempPtr->prevPCB = PCB_to_insert;

                    ready->count++;
                    break;
                }
                else
                { // move tempPtr through the queue
                    tempPtr = tempPtr->nextPCB;
                }
                temp++;
            }
            //suspendedReady->count++;
        }
        else
        {
            suspendedReady->count++;
            suspendedReady->head = PCB_to_insert;
            suspendedReady->tail = PCB_to_insert;
        }
    }
    else if (PCB_to_insert->runningStatus == -1 && PCB_to_insert->suspendedStatus == 1)
    { // Insert into blocked queue
        if (blocked->head != NULL)
        {
            blocked->tail->nextPCB = PCB_to_insert;
            PCB_to_insert->prevPCB = blocked->tail;
            blocked->tail = PCB_to_insert;
            blocked->count++;
        }
        else
        {
            blocked->head = PCB_to_insert;
            blocked->tail = PCB_to_insert;
            blocked->count++;
        }
    }
    else if (PCB_to_insert->runningStatus == -1 && PCB_to_insert->suspendedStatus == 0)
    { // Insert into suspended blocked queue
        if (suspendedBlocked->head != NULL)
        {
            suspendedBlocked->tail->nextPCB = PCB_to_insert;
            PCB_to_insert->prevPCB = suspendedBlocked->tail;
            suspendedBlocked->tail = PCB_to_insert;
            suspendedBlocked->count++;
        }
        else
        {
            suspendedBlocked->head = PCB_to_insert;
            suspendedBlocked->tail = PCB_to_insert;
            suspendedBlocked->count++;
        }
    }
}

int removePCB(PCB *PCB_to_remove) //Return 0 is success code, return 1 is error code.
{
    //BENJAMIN WILL PROGRAM THIS FUNCTION

    //removePCB() will remove a PCB from the queue in which it is currently stored.

    if (PCB_to_remove == NULL)
    {
        return 1;
    }
    else if (PCB_to_remove == ready->head)
    {
        //PCB *removedNext = PCB_to_remove->nextPCB;

        ready->head = PCB_to_remove->nextPCB;
        ready->head->prevPCB = NULL;
        PCB_to_remove->nextPCB = NULL;
        ready->count--;
        return 0;
    }
    else if (PCB_to_remove == blocked->head)
    {
        PCB *removedNext = PCB_to_remove->nextPCB;
        blocked->head = removedNext;
        removedNext->prevPCB = NULL;
        PCB_to_remove->nextPCB = NULL;
        blocked->count--;
        return 0;
    }
    else if (PCB_to_remove == suspendedReady->head)
    {
        PCB *removedNext = PCB_to_remove->nextPCB;

        suspendedReady->head = removedNext;
        removedNext->prevPCB = NULL;
        PCB_to_remove->nextPCB = NULL;
        suspendedReady->count--;
        return 0;
    }
    else if (PCB_to_remove == suspendedBlocked->head)
    {
        PCB *removedNext = PCB_to_remove->nextPCB;

        suspendedBlocked->head = removedNext;
        removedNext->prevPCB = NULL;
        PCB_to_remove->nextPCB = NULL;
        suspendedBlocked->count--;
        return 0;
    }
    else if (PCB_to_remove == ready->tail)
    {
        PCB *removedPrev = PCB_to_remove->prevPCB;

        ready->tail = removedPrev;
        removedPrev->nextPCB = NULL;
        PCB_to_remove->prevPCB = NULL;
        ready->count--;
        return 0;
    }
    else if (PCB_to_remove == blocked->tail)
    {
        PCB *removedPrev = PCB_to_remove->prevPCB;

        blocked->tail = removedPrev;
        removedPrev->nextPCB = NULL;
        PCB_to_remove->prevPCB = NULL;
        blocked->count--;
        return 0;
    }
    else if (PCB_to_remove == suspendedReady->tail)
    {
        PCB *removedPrev = PCB_to_remove->prevPCB;

        suspendedReady->tail = removedPrev;
        removedPrev->nextPCB = NULL;
        PCB_to_remove->prevPCB = NULL;
        suspendedReady->count--;
        return 0;
    }
    else if (PCB_to_remove == suspendedBlocked->tail)
    {
        PCB *removedPrev = PCB_to_remove->prevPCB;

        suspendedBlocked->tail = removedPrev;
        removedPrev->nextPCB = NULL;
        PCB_to_remove->prevPCB = NULL;
        suspendedBlocked->count--;
        return 0;
    }
    else
    {
        PCB *tempPrev = PCB_to_remove->prevPCB;
        PCB *tempNext = PCB_to_remove->nextPCB;

        tempPrev->nextPCB = tempNext;
        tempNext->prevPCB = tempPrev;

        PCB_to_remove->nextPCB = NULL;
        PCB_to_remove->prevPCB = NULL;

        if (PCB_to_remove->runningStatus == 0 && PCB_to_remove->suspendedStatus == 1)
        {
            ready->count--;
        }
        else if (PCB_to_remove->runningStatus == -1 && PCB_to_remove->suspendedStatus == 1)
        {
            blocked->count--;
        }
        else if (PCB_to_remove->runningStatus == 0 && PCB_to_remove->suspendedStatus == 0)
        {
            suspendedReady->count--;
        }
        else if (PCB_to_remove->runningStatus == -1 && PCB_to_remove->suspendedStatus == 0)
        {
            suspendedBlocked->count--;
        }

        return 0;
    }
}

void allocateQueues()
{
    ready = sys_alloc_mem(sizeof(queue));
    ready->count = 0;
    ready->head = NULL;
    ready->tail = NULL;

    blocked = sys_alloc_mem(sizeof(queue));
    blocked->count = 0;
    blocked->head = NULL;
    blocked->tail = NULL;

    suspendedReady = sys_alloc_mem(sizeof(queue));
    suspendedReady->count = 0;
    suspendedReady->head = NULL;
    suspendedReady->tail = NULL;

    suspendedBlocked = sys_alloc_mem(sizeof(queue));
    suspendedBlocked->count = 0;
    suspendedBlocked->head = NULL;
    suspendedBlocked->tail = NULL;
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