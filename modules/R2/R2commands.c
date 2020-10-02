//R2 User Commands

#include <string.h>
#include "../mpx_supt.h"
#include "R2_Internal_Functions_And_Structures.h"
#include "R2commands.h"
#include <string.h>
#include <core/serial.h>

//TEMPORARY USER COMMANDS - WILL BE REMOVED FOR R3/R4
void createPCB(char *processName, unsigned char processClass, int processPriority)
{ // BENJAMIN WILL PROGRAM THIS FUNCTION
    /*
    The createPCB command will call setupPCB() and insert the PCB in the appropriate queue
    */
    /*
    Error Checking:
    Name must be unique and valid.
    Class must be valid.
    Priority must be valid.
    */

    if (findPCB(processName) != NULL || strlen(processName) > 20)
    { // Check if the process has a unique name, and if it has a valid name.
        char errMsg[125];
        strcpy(errMsg, "The PCB could not be created as it either does not have a unique name or the name is longer than 20 characters!\n");
        int errLen = strlen(errMsg);
        sys_req(WRITE, DEFAULT_DEVICE, errMsg, &errLen);
    }
    else if (processClass != 'a' || processClass != 's')
    { // Check if the process has a valid class.
        char errMsg[100];
        strcpy(errMsg, "The PCB could not be created as it does not have a valid class!\n");
        int errLen = strlen(errMsg);
        sys_req(WRITE, DEFAULT_DEVICE, errMsg, &errLen);
    }
    else if (processPriority < 0 || processPriority > 9)
    { // Check if the process has a valid priority.
        char errMsg[100];
        strcpy(errMsg, "The PCB could not be created as it does not have a valid priority!\n");
        int errLen = strlen(errMsg);
        sys_req(WRITE, DEFAULT_DEVICE, errMsg, &errLen);
    }
    else
    { // Make the PCB
        PCB *createdPCB = setupPCB(processName, processClass, processPriority);
        insertPCB(createdPCB);
    }
}

void deletePCB(char *processName)
{ // BENJAMIN WILL PROGRAM THIS FUNCTION
    /*
    The deletePCB command will remove a PCB from the appropriate queue and then free all associated memory.
    This method will need to find the pcb, unlink it from the appropriate queue, and then free it.
    */
    /*
    Error Checking:
    Name must be valid.
    */

    if (strlen(processName) > 20)
    { // Check if the process has a valid name.
        char errMsg[100];
        strcpy(errMsg, "The PCB could not be deleted as the name is longer than 20 characters!\n");
        int errLen = strlen(errMsg);
        sys_req(WRITE, DEFAULT_DEVICE, errMsg, &errLen);
    }

    PCB *PCB_to_delete = findPCB(processName);

    if (PCB_to_delete == NULL)
    {
        char errMsg[42] = "The PCB you want to remove does not eist\n";
        int errMsgLen = 42;
        sys_req(WRITE, DEFAULT_DEVICE, errMsg, &errMsgLen);
    }
    else
    {
        int result = removePCB(PCB_to_delete);

        if (result == 1)
        {
            char errMsg[50];
            strcpy(errMsg, "The PCB could not be successfully deleted\n");
            int errLen = strlen(errMsg);
            sys_req(WRITE, DEFAULT_DEVICE, errMsg, &errLen);
        }
        else
        {
            char msg[50];
            strcpy(msg, "The desired PCB was deleted\n");
            int msgLen = strlen(msg);
            sys_req(WRITE, DEFAULT_DEVICE, msg, &msgLen);
        }
    }
}

void blockPCB(char *processName)
{ // ANASTASE WILL PROGRAM THIS FUNCTION

    // find pcb and validate process name
    PCB *pcb_to_block = findPCB(processName);

    if (pcb_to_block != NULL)
    {
        pcb_to_block->runningStatus = -1; // blocked
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

    PCB *pcb_to_unblock = findPCB(processName);
    if (pcb_to_unblock != NULL)
    {
        pcb_to_unblock->runningStatus = 0; // ready
        removePCB(pcb_to_unblock);         // is this the right place to put that function?
        insertPCB(pcb_to_unblock);
    }
}

//PERMANENT USER COMMANDS
void suspendPCB(char *processName)
{ // COLTON WILL PROGRAM THIS FUNCTION
    /*
    Places a PCB in the suspended state and reinserts it into the appropriate queue
    */
    ////////*
    ///////Error Checking:
    ///////Name must be valid.
    ///////*/

    PCB *PCBtoSuspend = findPCB(processName);
    removePCB(PCBtoSuspend);

    if (PCBtoSuspend == NULL || strlen(processName) > 20)
    {
        char nameError[] = "This is not a valid name.\n";
        int printCount = strlen(nameError);
        sys_req(WRITE, DEFAULT_DEVICE, nameError, &printCount);
    }
    else
    {
        PCBtoSuspend->suspendedStatus = 0;
        insertPCB(PCBtoSuspend);
    }
}

void resumePCB(char *processName)
{ // COLTON WILL PROGRAM THIS FUNCTION
    /*
    Places a PCB in the not suspended state and reinserts it into the appropriate queue
    */
    //////*
    //////Error Checking:
    //////Name must be valid.
    //////*/

    PCB *PCBtoResume = findPCB(processName);

    if (PCBtoResume == NULL || strlen(processName) > 20)
    {
        char nameError[] = "This is not a valid name.\n";
        int printCount = strlen(nameError);
        sys_req(WRITE, DEFAULT_DEVICE, nameError, &printCount);
    }
    else
    {
        removePCB(PCBtoResume);
        PCBtoResume->suspendedStatus = 1;
        insertPCB(PCBtoResume);
    }
}

void setPCBPriority(char *processName, int newProcessPriority)
{ // ANASTASE WILL PROGRAM THIS FUNCTION

    // Sets a PCB's priority and reinserts the process into the correct place in the correct queue

    /*
    Error Checking:
    Name must be valid.
    newPriority
    */

    // find the process and validate the name
    PCB *tempPCB = findPCB(processName);

    if ((tempPCB != NULL) && (newProcessPriority >= 0) && (newProcessPriority < 10))
    {
        tempPCB->priority = newProcessPriority;
        removePCB(tempPCB);
        insertPCB(tempPCB);
    }
}

void showPCB(char *processName)
{ // BENJAMIN WILL PROGRAM THIS FUNCTION
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

    if (strlen(processName) > 20)
    { // Check if the process has a valid name.
        char errMsg[100];
        strcpy(errMsg, "The PCB could not be shown as the name is longer than 20 characters!\n");
        int errLen = strlen(errMsg);
        sys_req(WRITE, DEFAULT_DEVICE, errMsg, &errLen);
    }
    else
    {

        PCB *PCB_to_show = findPCB(processName);

        if (PCB_to_show == NULL)
        { // Check to see if the PCB exists.
            char errMsg[100];
            strcpy(errMsg, "The PCB could not be shown, as it does not exist!\n");
            int errLen = strlen(errMsg);
            sys_req(WRITE, DEFAULT_DEVICE, errMsg, &errLen);
        }
        else
        {
            // Print out the PCB name.
            char nameMsg[50];
            strcpy(nameMsg, "The process name is: ");
            int nameMsgLen = strlen(nameMsg);
            sys_req(WRITE, DEFAULT_DEVICE, nameMsg, &nameMsgLen);
            char name[20];
            strcpy(name, PCB_to_show->processName);
            int nameLen = strlen(name);
            sys_req(WRITE, DEFAULT_DEVICE, name, &nameLen);
            char newLine[1];
            strcpy(newLine, "\n");
            int newLineLen = 1;
            sys_req(WRITE, DEFAULT_DEVICE, newLine, &newLineLen);

            // Print out PCB class
            char classMsg[50];
            strcpy(classMsg, "The process class is: ");
            int classMsgLen = strlen(classMsg);
            sys_req(WRITE, DEFAULT_DEVICE, classMsg, &classMsgLen);

            if (PCB_to_show->processClass == 'a')
            {
                char appMsg[50];
                strcpy(appMsg, "application");
                int appMsgLen = strlen(appMsg);
                sys_req(WRITE, DEFAULT_DEVICE, appMsg, &appMsgLen);
            }
            else
            {
                char sysMsg[50];
                strcpy(sysMsg, "system");
                int sysMsgLen = strlen(sysMsg);
                sys_req(WRITE, DEFAULT_DEVICE, sysMsg, &sysMsgLen);
            }
            sys_req(WRITE, DEFAULT_DEVICE, newLine, &newLineLen);

            // Print out the PCB state

            if (PCB_to_show->runningStatus == 0)
            { // The process is ready.
                char stateMsg[50];
                strcpy(stateMsg, "The process is ready!\n");
                int stateMsgLen = strlen(stateMsg);
                sys_req(WRITE, DEFAULT_DEVICE, stateMsg, &stateMsgLen);
            }
            else if (PCB_to_show->runningStatus == -1)
            { // The process is blocked.
                char stateMsg[50];
                strcpy(stateMsg, "The process is blocked!\n");
                int stateMsgLen = strlen(stateMsg);
                sys_req(WRITE, DEFAULT_DEVICE, stateMsg, &stateMsgLen);
            }
            else if (PCB_to_show->runningStatus == 1)
            { // The process is running.
                char stateMsg[50];
                strcpy(stateMsg, "The process is running!\n");
                int stateMsgLen = strlen(stateMsg);
                sys_req(WRITE, DEFAULT_DEVICE, stateMsg, &stateMsgLen);
            }

            // Print out the PCB suspended status

            if (PCB_to_show->suspendedStatus == 0)
            { // The process is suspended
                char susMsg[50];
                strcpy(susMsg, "The process is suspended!\n");
                int susMsgLen = strlen(susMsg);
                sys_req(WRITE, DEFAULT_DEVICE, susMsg, &susMsgLen);
            }
            else if (PCB_to_show->suspendedStatus == 1)
            { // The process is not suspended
                char susMsg[50];
                strcpy(susMsg, "The process is not suspended!\n");
                int susMsgLen = strlen(susMsg);
                sys_req(WRITE, DEFAULT_DEVICE, susMsg, &susMsgLen);
            }

            // Print out the PCB priority
            char priorityMsg[50];
            int priorityMsgLen = 0;

            switch (PCB_to_show->priority)
            {
            case 0:
                strcpy(priorityMsg, "The process priority is 0!\n");
                priorityMsgLen = strlen(priorityMsg);
                sys_req(WRITE, DEFAULT_DEVICE, priorityMsg, &priorityMsgLen);
                break;

            case 1:
                strcpy(priorityMsg, "The process priority is 1!\n");
                priorityMsgLen = strlen(priorityMsg);
                sys_req(WRITE, DEFAULT_DEVICE, priorityMsg, &priorityMsgLen);
                break;

            case 2:
                strcpy(priorityMsg, "The process priority is 2!\n");
                priorityMsgLen = strlen(priorityMsg);
                sys_req(WRITE, DEFAULT_DEVICE, priorityMsg, &priorityMsgLen);
                break;

            case 3:
                strcpy(priorityMsg, "The process priority is 3!\n");
                priorityMsgLen = strlen(priorityMsg);
                sys_req(WRITE, DEFAULT_DEVICE, priorityMsg, &priorityMsgLen);
                break;

            case 4:
                strcpy(priorityMsg, "The process priority is 4!\n");
                priorityMsgLen = strlen(priorityMsg);
                sys_req(WRITE, DEFAULT_DEVICE, priorityMsg, &priorityMsgLen);
                break;

            case 5:
                strcpy(priorityMsg, "The process priority is 5!\n");
                priorityMsgLen = strlen(priorityMsg);
                sys_req(WRITE, DEFAULT_DEVICE, priorityMsg, &priorityMsgLen);
                break;

            case 6:
                strcpy(priorityMsg, "The process priority is 6!\n");
                priorityMsgLen = strlen(priorityMsg);
                sys_req(WRITE, DEFAULT_DEVICE, priorityMsg, &priorityMsgLen);
                break;

            case 7:
                strcpy(priorityMsg, "The process priority is 7!\n");
                priorityMsgLen = strlen(priorityMsg);
                sys_req(WRITE, DEFAULT_DEVICE, priorityMsg, &priorityMsgLen);
                break;

            case 8:
                strcpy(priorityMsg, "The process priority is 8!\n");
                priorityMsgLen = strlen(priorityMsg);
                sys_req(WRITE, DEFAULT_DEVICE, priorityMsg, &priorityMsgLen);
                break;

            case 9:
                strcpy(priorityMsg, "The process priority is 9!\n");
                priorityMsgLen = strlen(priorityMsg);
                sys_req(WRITE, DEFAULT_DEVICE, priorityMsg, &priorityMsgLen);
                break;

            default:
                break;
            }
        }
    }
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

    char message[] = "Printing the ready queue:\n";
    int messLength = strlen(message);
    sys_req(WRITE, DEFAULT_DEVICE, message, &messLength);

    queue *tempQueue = getReady();
    PCB *tempPCB = tempQueue->head;

    int loop = 0;
    int count = tempQueue->count;

    if (count == 0)
    {
        // the queue is empty
        char error_message[30] = "The queue is empty.\n";
        int error_size = strlen(error_message);
        sys_req(WRITE, DEFAULT_DEVICE, error_message, &error_size);
        return;
    }

    while (loop <= count && tempPCB->nextPCB != NULL && count > 0)
    {
        showPCB(tempPCB->processName);
        PCB *tempNext = tempPCB->nextPCB;
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

    char message[] = "Printing the suspended ready queue:\n";
    int messLength = strlen(message);
    sys_req(WRITE, DEFAULT_DEVICE, message, &messLength);

    queue *tempQueue = getSuspendedReady();
    PCB *tempPCB = tempQueue->head;

    int loop = 0;
    int count = tempQueue->count;

    if (count == 0)
    {
        // the queue is empty
        char error_message[30] = "The queue is empty.\n";
        int error_size = strlen(error_message);
        sys_req(WRITE, DEFAULT_DEVICE, error_message, &error_size);
        return;
    }

    while (loop < count && tempPCB->nextPCB != NULL && count > 0)
    {
        showPCB(tempPCB->processName);
        PCB *tempNext = tempPCB->nextPCB;
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

    char message[] = "Printing the suspended blocked queue:\n";
    int messLength = strlen(message);
    sys_req(WRITE, DEFAULT_DEVICE, message, &messLength);

    queue *tempQueue = getSuspendedBlocked();
    PCB *tempPCB = tempQueue->head;

    int loop = 0;
    int count = tempQueue->count;

    if (count == 0)
    {
        // the queue is empty
        char error_message[30] = "The queue is empty.\n";
        int error_size = strlen(error_message);
        sys_req(WRITE, DEFAULT_DEVICE, error_message, &error_size);
        return;
    }

    while (loop < count && tempPCB->nextPCB != NULL && count > 0)
    {
        showPCB(tempPCB->processName);
        PCB *tempNext = tempPCB->nextPCB;
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
        HEAD
    */
    /*
    Error Checking:
    None
    */

    // check

    char print_message[30] = "The blocked queue:\n";
    int message_size = strlen(print_message);
    sys_req(WRITE, DEFAULT_DEVICE, print_message, &message_size);

    // printPCBs(blocked);
    queue *tempQueue = getBlocked();
    PCB *tempPtr = tempQueue->head; //PCB_container->head;
    int count = tempQueue->count;

    if (count == 0)
    {
        // the queue is empty
        char error_message[30] = "The queue is empty.\n";
        int error_size = strlen(error_message);
        sys_req(WRITE, DEFAULT_DEVICE, error_message, &error_size);
        return;
    }
    // The queue is not empty

    int value = 0;
    // Testing purpose
    //char print_message[38]="The blocke queue testing:\n";
    //int message_size=strlen(print_message);
    //sys_req(WRITE, DEFAULT_DEVICE, print_message, &message_size);

    while (value <= count)
    { // testing for <== or <
        // Print out the process
        showPCB(tempPtr->processName);
        // increment pcb*tempPtr, the loop variable.
        tempPtr = tempPtr->nextPCB;
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

    showReady();
    showSuspendedReady();
    showBlocked();
    showSuspendedBlocked();
}
