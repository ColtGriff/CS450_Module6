//R2 User Commands

#include "../mpx_supt.h"
#include "R2_Internal_Functions_And_Structures.h"

//TEMPORARY USER COMMANDS - WILL BE REMOVED FOR R3/R4
void createPCB(char *processName, unsigned char processClass, int processPriority)
{   // BENJAMIN WILL PROGRAM THIS FUNCTION
    /*
    The createPCB command will call setupPCB() and insert the PCB in the appropriate queue
    */

    /*
    Error Checking:
    Name must be unique and valid.
    Class must be valid.
    Priority must be valid.
    */

    (void)processName;
    (void)processClass;
    (void)processPriority;
}

void deletePCB(char *processName)
{   // BENJAMIN WILL PROGRAM THIS FUNCTION
    /*
    The deletePCB command will remove a PCB from the appropriate queue and then free all associated memory.
    This method will need to find the pcb, unlink it from the appropriate queue, and then free it.
    */

    /*
    Error Checking:
    Name must be valid.
    */

    (void)processName;
}

void blockPCB(char *processName)
{ // ANASTASE WILL PROGRAM THIS FUNCTION


    (void)processName;
    // find pcb and validate process name
    PCB*pcb_to_block=findPCB(processName);

    if(pcb_to_block){
        pcb_to_block->runningStatus=-1;// blocked
        removePCB(pcb_to_block);
        insertPCB(pcb_to_block);
    }
    
}

void unblockPCB(char *processName)
{ // ANASTASE WILL PROGRAM THIS FUNCTION

    /*
    Places a PCB in the unblocked state and reinserts it into the appropriate queue.
    */

    /*
    Error Checking:
    Name must be valid.

    */

    (void)processName;

    PCB*pcb_to_unblock=findPCB(processName);
    if(pcb_to_unblock){
        pcb_to_unblock->runningStatus=0;// ready
        removePCB(processName); // is this the right place to put that function? 
        insertPCB(pcb_to_unblock);
    }
    
   
}

//PERMANENT USER COMMANDS
void suspendPCB(char *processName)
{   // COLTON WILL PROGRAM THIS FUNCTION
    /*
    Places a PCB in the suspended state and reinserts it into the appropriate queue
    */

    /*
    Error Checking:
    Name must be valid.
    */

    (void)processName;
}

void resumePCB(char *processName)
{   // COLTON WILL PROGRAM THIS FUNCTION
    /*
    Places a PCB in the not suspended state and reinserts it into the appropriate queue
    */

    /*
    Error Checking:
    Name must be valid.
    */

    (void)processName;
}

void setPCBPriority(char *processName, int newProcessPriority)
{   // ANASTASE WILL PROGRAM THIS FUNCTION
    /*
    Sets a PCB's priority and reinserts the process into the correct place in the correct queue 

    /*
    Error Checking:
    Name must be valid.
    newPriority
    */

    (void)processName;
    (void)newProcessPriority;

    // find the process and validate the name
    PCB*tempPCB=findPCB(processName);

    if((tempPCB) && (newProcessPriority >= 0) && (newProcessPriority < 10)){
        tempPCB->priority = newProcessPriority;
        removePCB(tempPCB);
        insertPCB(tempPCB);
    }
}

void showPCB(char *processName)
{   // BENJAMIN WILL PROGRAM THIS FUNCTION
    /*
    Displays the following information for a PCB:
        Process Name
        Class
        State
        Suspended Status
        Priority
    */

    /*
    Error Checking:
    Name must be valid.
    */

    (void)processName;
}

void showReady()
{ // COLTON WILL PROGRAM THIS FUNCTION
    /*
    Displays the following information for each PCB in the ready queue:
        Process Name
        Class
        State
        Suspended Status
        Priority
    */

    /*
    Error Checking:
    None
    */
}

void showBlocked()
{ // ANASTASE WILL PROGRAM THIS FUNCTION
    /*
    Displays the following information for each PCB in the blocked queue:
        Process Name
        Class
        State
        Suspended Status
        Priority

    */

    char print_message[30]="The blocked queue has:\n";
    int message_size=strlen(error);
    sys_req(WRITE, DEFAULT_DEVICE, print_message, &message_size);
    
    printPCBs(blocked);
    
}

void showAll()
{ // COLTON WILL PROGRAM THIS FUNCTION
    /*
    Displays the following information for each PCB in the ready and blocked queues:
        Process Name
        Class
        State
        Suspended Status
        Priority
    */

    /*
    Error Checking:
    None
    */
}

void printPCBs(queue*PCB_container){ // Why is it giving unknown type 'queue'?
    // PCB_container has PCB*head and PCB*tail pointers
    // check if the input queue exist
    if(strcmp(PCB_container,"ready"!=0)||strcmp(PCB_container,"blocked" !=0)// will need to revise this
        ||strcmp(PCB_container,"suspendedReady"!=0)||strcmp(PCB_container,"suspendedBlocked" !=0)){

        // the queue entered is invalid.
        char error_message[30]="The queue does not exit.";
        int error_size=strlen(error);
        sys_req(WRITE, DEFAULT_DEVICE, error_message, &error_size);
        return;
    }
    // make a queue ptr that has pcb*head pointer.
    // make pcb ptr to equal the head of the queue ptr. 
    // Now you have full access to the pcbs in the queue
    // use the pcb ptr to loop over the pcbs in any queue

    queue*tempQueue;

    PCB*tempPtr =tempQueue->head; 

    int count =tempQueue->count;

    if(tempPtr==tail){
        // the queue is empty
        char error_message[30]="The queue is empty.";
        int error_size=strlen(error);
        sys_req(WRITE, DEFAULT_DEVICE, error_message, &error_size);
        return;
    }
    // The queue is not empty

    // int max_size =2; 
    int value=0;

    while(value<=count){ // testing for <== or <
        // Print process name
        sys_req(WRITE, DEFAULT_DEVICE,tempPtr->processName, &strlen(tempPtr->processName));
        
        // print process Class
        sys_req(WRITE, DEFAULT_DEVICE, tempPtr->processClass, &strlen(tempPtr->processClass));

        //print process running status,which  is -1 if blocked, 0 for ready, and 1 for a running process
        char runningStatus[2] = int_to_char(tempPtr->runningStatus); // int max_size =2; 
        sys_req(WRITE, DEFAULT_DEVICE,runningStatus , &strlen(runningStatus));
        
        // print process suspended status
        char suspendedStatus[2]=i nt_to_char(tempPtr->suspendedStatus);
        sys_req(WRITE, DEFAULT_DEVICE,suspendedStatus, &strlen(suspendedStatus));
        
        // print process priority
        char priority = int_to_char(tempPtr->priority);
        sys_req(WRITE, DEFAULT_DEVICE,priority, &strlen(priority));
        
        // increment pcb*tempPtr, the loop variable.
        tempPtr=tempPtr->nextPCB;
        value++;

    }

}

char* int_to_char(int value){
    // changes only an integer less than 10 to its equivalent character.
     
    if(value < 10){
       return value +'0' ;
    }
    else{
     char error[80]="Error while parsing the value: Unexpected value.";
     int error_size=strlen(error);
     sys_req(WRITE, DEFAULT_DEVICE, error, &error_size);
    }
    
}

    