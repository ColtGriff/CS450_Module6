//R2 User Commands

#include <string.h>
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

    /*
    Finds a PCB sets it's state to blocked and reinserts it into the appropriate queue.
    */
    /*
    Error Checking:
    Name must be valid.
    */

    (void)processName;
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
}

//PERMANENT USER COMMANDS
void suspendPCB(char *processName)
{   // COLTON WILL PROGRAM THIS FUNCTION
    /*
    Places a PCB in the suspended state and reinserts it into the appropriate queue
    */
    ////////*
    ///////Error Checking:
    ///////Name must be valid.
    ///////*/

    PCB* PCBtoSuspend = findPCB(processName);
    removePCB(PCBtoSuspend);

    if(PCBtoSuspend == NULL || strlen(processName) > 20){
        char nameError[] = "This is not a valid name.\n";
        int printCount = strlen(nameError);
        sys_req(WRITE, DEFAULT_DEVICE, nameError, &printCount);
    }
    else{
        PCBtoSuspend->suspendedStatus = 0;
        insertPCB(PCBtoSuspend);
    }

}

void resumePCB(char *processName)
{   // COLTON WILL PROGRAM THIS FUNCTION
    /*
    Places a PCB in the not suspended state and reinserts it into the appropriate queue
    */
    //////*
    //////Error Checking:
    //////Name must be valid.
    //////*/

    PCB* PCBtoResume = findPCB(processName);
    removePCB(PCBtoResume);
    
    if(PCBtoResume == NULL || strlen(processName) > 20){
        char nameError[] = "This is not a valid name.\n";
        int printCount = strlen(nameError);
        sys_req(WRITE, DEFAULT_DEVICE, nameError, &printCount);
    }
    else{
        PCBtoResume->suspendedStatus = 1;
        insertPCB(PCBtoResume);
    }
}

void setPCBPriority(char *processName, int newProcessPriority)
{   // ANASTASE WILL PROGRAM THIS FUNCTION
    /*
    Sets a PCB's priority and reinserts the process into the correct place in the correct queue
    */
    /*
    Error Checking:
    Name must be valid.
    newPriority
    */

    (void)processName;
    (void)newProcessPriority;
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
    char spacer[1] = "\n";
    int spaceCount = strlen(spacer);


    char message[] = "Printing the ready queue:\n";
    int messLength = strlen(message);
    sys_req(WRITE, DEFAULT_DEVICE, message, &messLength);

    queue* tempQueue = getReady();
    PCB* tempPCB = tempQueue->head;
    
    int loop = 0;
    int count = tempQueue->count;

    while(loop <= count && tempPCB->nextPCB != NULL){

        PCB* tempNext = tempPCB->nextPCB;

        char name[20];
        strcpy(name, tempPCB->processName);
        int nameLength = strlen(name);

        unsigned char class = tempPCB->processClass;
        char test4[2] = {class, '\0'};
        int classLength = strlen(test4);

        int state = tempPCB->runningStatus;
        int susStat = tempPCB->suspendedStatus;

        int priority = tempPCB->priority;

        char test1[2] = {state, '\0'};
        int length1 = strlen(test1);
        char test2[2] = {susStat, '\0'};
        int length2 = strlen(test2);
        char test3[2] = {priority, '\0'};
        int length3 = strlen(test3);

        sys_req(WRITE, DEFAULT_DEVICE, name, &nameLength);
        sys_req(WRITE, DEFAULT_DEVICE, spacer, &spaceCount);

        sys_req(WRITE, DEFAULT_DEVICE, test4, &classLength);
        sys_req(WRITE, DEFAULT_DEVICE, spacer, &spaceCount);

        sys_req(WRITE, DEFAULT_DEVICE, test1, &length1);
        sys_req(WRITE, DEFAULT_DEVICE, spacer, &spaceCount);

        sys_req(WRITE, DEFAULT_DEVICE, test2, &length2);
        sys_req(WRITE, DEFAULT_DEVICE, spacer, &spaceCount);

        sys_req(WRITE, DEFAULT_DEVICE, test3, &length3);
        sys_req(WRITE, DEFAULT_DEVICE, spacer, &spaceCount);


        loop++;
        tempPCB = tempNext;
    }

}


void showSuspendedReady()
{ // COLTON WILL PROGRAM THIS FUNCTION
    /*
    Displays the following information for each PCB in the suspended ready queue:
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

    char spacer[1] = "\n";
    int spaceCount = strlen(spacer);


    char message[] = "Printing the suspended ready queue:\n";
    int messLength = strlen(message);
    sys_req(WRITE, DEFAULT_DEVICE, message, &messLength);

    queue* tempQueue = getSuspendedReady();
    PCB* tempPCB = tempQueue->head;
    
    int loop = 0;
    int count = tempQueue->count;

    while(loop < count && tempPCB->nextPCB != NULL){

        PCB* tempNext = tempPCB->nextPCB;

        char name[20];
        strcpy(name, tempPCB->processName);
        int nameLength = strlen(name);

        unsigned char class = tempPCB->processClass;
        char test4[2] = {class, '\0'};
        int classLength = strlen(test4);

        int state = tempPCB->runningStatus;
        int susStat = tempPCB->suspendedStatus;

        int priority = tempPCB->priority;

        char test1[2] = {state, '\0'};
        int length1 = strlen(test1);
        char test2[2] = {susStat, '\0'};
        int length2 = strlen(test2);
        char test3[2] = {priority, '\0'};
        int length3 = strlen(test3);

        sys_req(WRITE, DEFAULT_DEVICE, name, &nameLength);
        sys_req(WRITE, DEFAULT_DEVICE, spacer, &spaceCount);

        sys_req(WRITE, DEFAULT_DEVICE, test4, &classLength);
        sys_req(WRITE, DEFAULT_DEVICE, spacer, &spaceCount);

        sys_req(WRITE, DEFAULT_DEVICE, test1, &length1);
        sys_req(WRITE, DEFAULT_DEVICE, spacer, &spaceCount);

        sys_req(WRITE, DEFAULT_DEVICE, test2, &length2);
        sys_req(WRITE, DEFAULT_DEVICE, spacer, &spaceCount);

        sys_req(WRITE, DEFAULT_DEVICE, test3, &length3);
        sys_req(WRITE, DEFAULT_DEVICE, spacer, &spaceCount);

        loop++;
        tempPCB = tempNext;
    }

}

void showSuspendedBlocked()
{ // COLTON WILL PROGRAM THIS FUNCTION
    /*
    Displays the following information for each PCB in the suspended blocked queue:
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

    char spacer[1] = "\n";
    int spaceCount = strlen(spacer);


    char message[] = "Printing the suspended blocked queue:\n";
    int messLength = strlen(message);
    sys_req(WRITE, DEFAULT_DEVICE, message, &messLength);

    queue* tempQueue = getSuspendedBlocked();
    PCB* tempPCB = tempQueue->head;
    
    int loop = 0;
    int count = tempQueue->count;

    while(loop < count && tempPCB->nextPCB != NULL){

        PCB* tempNext = tempPCB->nextPCB;

        char name[20];
        strcpy(name, tempPCB->processName);
        int nameLength = strlen(name);

        unsigned char class = tempPCB->processClass;
        char test4[2] = {class, '\0'};
        int classLength = strlen(test4);

        int state = tempPCB->runningStatus;
        int susStat = tempPCB->suspendedStatus;

        int priority = tempPCB->priority;

        char test1[2] = {state, '\0'};
        int length1 = strlen(test1);
        char test2[2] = {susStat, '\0'};
        int length2 = strlen(test2);
        char test3[2] = {priority, '\0'};
        int length3 = strlen(test3);

        sys_req(WRITE, DEFAULT_DEVICE, name, &nameLength);
        sys_req(WRITE, DEFAULT_DEVICE, spacer, &spaceCount);

        sys_req(WRITE, DEFAULT_DEVICE, test4, &classLength);
        sys_req(WRITE, DEFAULT_DEVICE, spacer, &spaceCount);

        sys_req(WRITE, DEFAULT_DEVICE, test1, &length1);
        sys_req(WRITE, DEFAULT_DEVICE, spacer, &spaceCount);

        sys_req(WRITE, DEFAULT_DEVICE, test2, &length2);
        sys_req(WRITE, DEFAULT_DEVICE, spacer, &spaceCount);

        sys_req(WRITE, DEFAULT_DEVICE, test3, &length3);
        sys_req(WRITE, DEFAULT_DEVICE, spacer, &spaceCount);


        loop++;
        tempPCB = tempNext;
    }

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
    /*
    Error Checking:
    None
    */


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

    showReady();
    showSuspendedReady();
    showBlocked();
    showSuspendedBlocked();
    
}