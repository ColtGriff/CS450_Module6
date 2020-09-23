//C structures

#include <string.h>
#include "../mpx_supt.h"
#include "R2_Internal_Functions_And_Structures.h"

queue ready;
queue blocked;
queue suspendedReady;
queue suspendedBlocked;

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
    return sys_free_mem(PCB_to_free) ;

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
    // searching in ready queue

    PCB**foundPCB; // this is a pointer to another pointer (** starts). Need testing!
    foundPCB=searchPCB(ready, processName);
    if(foundPCB){
        return foundPCB;
    }

    // searching PCB in blocked queue
    PCB**foundPCB;
    foundPCB=searchPCB(blocked, processName);
    if(foundPCB){
        return foundPCB;
    }

    // searching PCB in suspendedReady queue
    PCB**foundPCB;
    foundPCB=searchPCB(suspendedReady, processName);
    if(foundPCB){
        return foundPCB;
    }

    // searching PCB in suspendedBlocked queue
    PCB**foundPCB;
    foundPCB=searchPCB(getSuspendedBlocked, processName);
    if(foundPCB){
        return foundPCB;
    }


    //return NULL is done by searchPCB()
}

PCB* searchPCB(queue*PCB_container,char*processName){
 // PCB_container has PCB*head and PCB*tail pointers
    queue*tempQueue;
    PCB*tempPCB=tempQueue->head;

    int found=0;// not found signal
    // detecting buffer overflow
    if(strlen(processName)>20){

       char error_message[30]="Invalid process name.";
        int error_size=strlen(error);
        sys_req(WRITE, DEFAULT_DEVICE, error_message, &error_size);
        return;
    }

     // check if the queue exist.  WILL BE REMOVED because we now have control of what queue to search in.
    if(strcmp(PCB_container,"ready"!=0)||strcmp(PCB_container,"blocked" !=0)// will need to revise this
        ||strcmp(PCB_container,"suspendedReady"!=0)||strcmp(PCB_container,"suspendedBlocked" !=0)){
        // the queue entered is invalid.
        char error_message[30]="The queue does not exit.";
        int error_size=strlen(error);
        sys_req(WRITE, DEFAULT_DEVICE, error_message, &error_size);
        return;
    }

    while(tempPCB !=NULL){
        if(strcmp(tempPCB->processName,processName)==0){
            found=1;// found signal
            return tempPCB;
            break;
        }

        tempPCB=tempPCB->nextPCB;
        
    }


    if (found==0){
        char result_message[30]="The process was not found.";
        int result_size=strlen(error);
        sys_req(WRITE, DEFAULT_DEVICE, result_message, &result_size);
        return NULL;
    }

}


void insertPcb(PCB *PCB_to_insert)
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
    return &ready;
}

queue *getBlocked()
{
    return &blocked;
}

queue *getSuspendedReady()
{
    return &suspendedReady;
}

queue *getSuspendedBlocked()
{
    return &suspendedBlocked;
}