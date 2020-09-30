//C structures

#include <string.h>
#include "../mpx_supt.h"
#include "R2_Internal_Functions_And_Structures.h"

queue*ready;
queue*blocked;
queue*suspendedReady;
queue*suspendedBlocked;

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

    PCB*found_ready_pcb; // this is a pointer to another pointer (** starts). Need testing!
    found_ready_pcb=searchPCB(ready, processName);
    if(found_ready_pcb){
        return found_ready_pcb;
    }

    // searching PCB in blocked queue
    PCB*found_blocked_pcb;
    found_blocked_pcb=searchPCB(blocked, processName);
    if(found_blocked_pcb){
        return found_blocked_pcb;
    }

    // searching PCB in suspendedReady queue
    PCB*found_suspended_ready_pcb;
    found_suspended_ready_pcb=searchPCB(suspendedReady, processName);
    if(found_suspended_ready_pcb){
        return found_suspended_ready_pcb;
    }

    // searching PCB in suspendedBlocked queue
    PCB*found_suspended_blocked_pcb;
    found_suspended_blocked_pcb=searchPCB(suspendedBlocked, processName);
    if(found_suspended_blocked_pcb){
        return found_suspended_blocked_pcb;
    }


    return NULL ;// for testing
}

PCB* searchPCB(queue*PCB_container,char*processName){
 // PCB_container has PCB*head and PCB*tail pointers
    //queue*tempQueue;

    PCB*tempPtr =PCB_container->head; 

    int count =PCB_container->count;// tempQueue->count;

    int found=0;// not found signal
    // detecting buffer overflow
    if(strlen(processName)>20){

       char error_message[30]="Invalid process name.";
        int error_size=strlen(error_message);
        sys_req(WRITE, DEFAULT_DEVICE, error_message, &error_size);
        //return cz we have to stop if the process name is too long
    }
    
    int value=0;
    while(value<=count){
        if(strcmp(tempPtr->processName,processName)==0){
            found=1;// found signal
            return tempPtr;
            break;
        }

        tempPtr = tempPtr->nextPCB;// don't know why this line is giving assignment from incompatible pointer type error.
        value++;
        
    }


    if (found==0){
        char result_message[30]="The process was not found.";
        int result_size=strlen(result_message);
        sys_req(WRITE, DEFAULT_DEVICE, result_message, &result_size);
        return NULL; // Why are this return not recognized??
    }
   return NULL; // for testing.
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