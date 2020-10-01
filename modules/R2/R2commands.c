//R2 User Commands

#include "../mpx_supt.h"
#include "R2_Internal_Functions_And_Structures.h"
#include "R2commands.h"
#include <string.h>
#include <core/serial.h>


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
        removePCB(pcb_to_unblock); // is this the right place to put that function? 
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
    
   // Sets a PCB's priority and reinserts the process into the correct place in the correct queue 

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
        // check

    */

    char print_message[30]="The blocked queue:\n";
    int message_size=strlen(print_message);
    sys_req(WRITE, DEFAULT_DEVICE, print_message, &message_size);
    
    // printPCBs(blocked);
     queue*tempQueue=getBlocked();
     PCB*tempPtr =tempQueue->head; //PCB_container->head; 
    int count =tempQueue->count;
     serial_println("made blocked queue"); 

    if(count ==0){
        serial_println("queue is epty");      // the queue is empty
        char error_message[30]="The queue is empty.";
        int error_size=strlen(error_message);
        sys_req(WRITE, DEFAULT_DEVICE, error_message, &error_size);
        return;
    }
    // The queue is not empty

    int value=0;
    // Testing purpose
    //char print_message[38]="The blocke queue testing:\n";
    //int message_size=strlen(print_message);
    //sys_req(WRITE, DEFAULT_DEVICE, print_message, &message_size);

    while(value<=count){ // testing for <== or <
        // Print process name
        int len=strlen(tempPtr->processName);
        sys_req(WRITE, DEFAULT_DEVICE,tempPtr->processName, &len);
        // Spacing
        char space[2]="\n";
        int size=strlen(space);
        sys_req(WRITE, DEFAULT_DEVICE,space, &size);
        
        // print process Class
        //char processClass[2];
        //processClass[2]=tempPtr->processClass;
        char class[30];
        if (tempPtr-> processClass =='a'){
            strcpy(class,"ProcessClass:Application\n");
        }else{
            strcpy(class,"ProcessClass:SystemProcess\n");
        }

        len=strlen(class);
        sys_req(WRITE, DEFAULT_DEVICE, class, &len);

        //print process running status,which  is -1 if blocked, 0 for ready, and 1 for a running process
        //char runningStatus[17];
        //strcpy(runningStatus,"RunningStatus: \0");
       // runningStatus[15] =tempPtr->runningStatus +'0'; 
        char runningStatus_message[25];
        if( tempPtr->runningStatus == -1){

           strcpy(runningStatus_message,"RunningStatus:Blocked\n");

        }else if(tempPtr->runningStatus==0){

            strcpy(runningStatus_message,"RunningStatus:Ready\n");

        }else{
            strcpy(runningStatus_message,"RunningStatus:Running\n");
        }

        len=strlen(runningStatus_message); 
        sys_req(WRITE, DEFAULT_DEVICE,runningStatus_message , &len);
        
        // print process suspended status
        //char suspendedStatus[19]= "SuspendedStatus: \0";
        //suspendedStatus[17]=tempPtr->suspendedStatus +'0';
        char message[30];
        if(tempPtr->suspendedStatus==0){
            strcpy(message,"SuspendedStatus:Suspended\n");
        }else{
            strcpy(message,"SuspendedStatus:Not_suspended\n");
        }
       
        len=strlen(message);
        sys_req(WRITE, DEFAULT_DEVICE,message, &len);
        
        // print process priority
        char priority [12]= "Priority: \0";
        priority[10]=tempPtr->priority +'0';
        len=strlen(priority);
        sys_req(WRITE, DEFAULT_DEVICE,priority, &len);
        
        // increment pcb*tempPtr, the loop variable.
        tempPtr=tempPtr->nextPCB;
        value++;

    }

    
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
/*

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

    //queue*tempQueue;

    PCB*tempPtr =PCB_container->head; 

    int count =PCB_container->count;

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
*/
    