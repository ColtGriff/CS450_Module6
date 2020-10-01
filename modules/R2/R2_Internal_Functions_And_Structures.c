//C structures

#include <string.h>
#include "../mpx_supt.h"
#include "R2_Internal_Functions_And_Structures.h"

queue* ready;
queue* blocked;
queue* suspendedReady;
queue* suspendedBlocked;

PCB *allocatePCB() //Returns the created PCB pointer if successful, returns NULL if an error occurs.
{
    //COLTON WILL PROGRAM THIS FUNCTION

    //allocatePCB() will use sys_alloc_mem() to allocate memory for a new PCB, possible including the stack, and perform any reasonable initialization.
    PCB* newPCB = (PCB*)sys_alloc_mem(sizeof(PCB));


    char name[20] = "newPCB";
    strcpy(newPCB->processName, name);

    newPCB->suspendedStatus = 1;
    newPCB->runningStatus = -1;
    newPCB->stackTop = (newPCB->stackTop + 1024);
    newPCB->stackBase = newPCB->stackBase;
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

    (void)PCB_to_free;

    return 0;
}

PCB *setupPCB(char *processName, unsigned char processClass, int processPriority) //Returns the created PCB pointer if successful, returns NULL if an error occurs.
{
    //COLTON WILL PROGRAM THIS FUNCTION

    //setupPcb() will call allocatePCB() to create an empty PCB, initializes the PCB information, sets the PCB state to ready, not suspended.

    PCB* tempPCB = allocatePCB();

    PCB* returnedPCB;
    
    if(findPCB(processName)->processName == processName){

        char message[] = "There is already a PCB with this name.\n";
        int messLength = strlen(message);
        sys_req(WRITE, DEFAULT_DEVICE, message, &messLength);

        returnedPCB = NULL;

    }
    else{

        strcpy(tempPCB->processName, processName);
        tempPCB->processClass = processClass;
        tempPCB->priority = processPriority;
        tempPCB->runningStatus = 0;
        tempPCB->suspendedStatus = 1;

        returnedPCB = tempPCB;

    }

    return returnedPCB;

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

    (void)PCB_to_insert;

}

int removePCB(PCB *PCB_to_remove) //Return 0 is success code, reurn 1 is error code.
{
    //BENJAMIN WILL PROGRAM THIS FUNCTION

    //removePCB() will remove a PCB from the queue in which it is currently stored.

    (void)PCB_to_remove;

    return 0;
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