//R2 User Commands

#include <string.h>
#include "../mpx_supt.h"
#include "../utilities.h"
#include "R2_Internal_Functions_And_Structures.h"
#include "R2commands.h"
#include <string.h>
#include <core/serial.h>

//TEMPORARY USER COMMANDS - WILL BE REMOVED FOR R3/R4
void createPCB(char *processName, char processClass, int processPriority)
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
        printMessage("The PCB could not be created as it either does not have a unique name or the name is longer than 20 characters!\n");
    }
    else if (processClass != 'a' && processClass != 's')
    { // Check if the process has a valid class.
        printMessage("The PCB could not be created as it does not have a valid class!\n");
    }
    else if (processPriority < 0 || processPriority > 9)
    { // Check if the process has a valid priority.
        printMessage("The PCB could not be created as it does not have a valid priority!\n");
    }
    else
    { // Make the PCB
        PCB *createdPCB = setupPCB(processName, processClass, processPriority);

        printMessage("The PCB was created!\n");

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
        printMessage("The PCB could not be deleted as the name is longer than 20 characters!\n");
    }

    PCB *PCB_to_delete = findPCB(processName);

    if (PCB_to_delete == NULL)
    {
        printMessage("The PCB you want to remove does not exist\n");
    }
    else if (strcmp(processName, "infinite") == 0 && PCB_to_delete->suspendedStatus != 0)
    {
        printMessage("In order to delete the infinite process it must be suspended first.\n");
    }
    else if (PCB_to_delete->processClass == 's')
    {
        printMessage("You do not have permission to delete system processes!\n");
    }
    else
    {
        int removed = removePCB(PCB_to_delete);
        if (removed == 1)
        {
            printMessage("The PCB could not be unlinked.\n");
        }
        else
        {
            int result = sys_free_mem(PCB_to_delete);
            if (result == -1)
            {
                // printMessage("The PCB could not be successfully deleted\n");
            }
            else
            {
                printMessage("The desired PCB was deleted\n");
            }
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

        printMessage("The PCB was successfully blocked!\n");
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

        //printMessage("The PCB was successfully unblocked!\n");
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

    if (PCBtoSuspend == NULL || strlen(processName) > 20)
    {
        printMessage("This is not a valid name.\n");
    }
    else if (PCBtoSuspend->processClass == 's')
    {
        printMessage("You do not have permission to suspend system processes!\n");
    }
    else
    {
        removePCB(PCBtoSuspend);
        PCBtoSuspend->suspendedStatus = 0;
        insertPCB(PCBtoSuspend);

        printMessage("The PCB was successfully suspended!\n");
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
        printMessage("This is not a valid name.\n");
    }
    else
    {
        removePCB(PCBtoResume);
        PCBtoResume->suspendedStatus = 1;
        insertPCB(PCBtoResume);

        printMessage("The PCB was successfully resumed!\n");
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

        printMessage("The PCB's priority was successfully changed!\n");
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
        printMessage("The PCB could not be shown as the name is longer than 20 characters!\n");
    }
    else
    {
        PCB *PCB_to_show = findPCB(processName);

        if (PCB_to_show == NULL)
        { // Check to see if the PCB exists.
            printMessage("The PCB could not be shown, as it does not exist!\n");
        }
        else
        {
            // Print out the PCB name.
            printMessage("The process name is: ");
            int length = strlen(PCB_to_show->processName);
            sys_req(WRITE, DEFAULT_DEVICE, PCB_to_show->processName, &length);
            printMessage("\n");

            // Print out PCB class
            printMessage("The process class is: ");

            if (PCB_to_show->processClass == 'a')
            {
                printMessage("application.\n");
            }
            else
            {
                printMessage("system.\n");
            }

            // Print out the PCB state

            if (PCB_to_show->runningStatus == 0)
            { // The process is ready.
                printMessage("The process is ready!\n");
            }
            else if (PCB_to_show->runningStatus == -1)
            { // The process is blocked.
                printMessage("The process is blocked!\n");
            }
            else if (PCB_to_show->runningStatus == 1)
            { // The process is running.
                printMessage("The process is running!\n");
            }

            // Print out the PCB suspended status

            if (PCB_to_show->suspendedStatus == 0)
            { // The process is suspended
                printMessage("The process is suspended!\n");
            }
            else if (PCB_to_show->suspendedStatus == 1)
            { // The process is not suspended
                printMessage("The process is not suspended!\n");
            }

            // Print out the PCB priority
            switch (PCB_to_show->priority)
            {
            case 0:
                printMessage("The process priority is 0!\n");
                break;

            case 1:
                printMessage("The process priority is 1!\n");
                break;

            case 2:
                printMessage("The process priority is 2!\n");
                break;

            case 3:
                printMessage("The process priority is 3!\n");
                break;

            case 4:
                printMessage("The process priority is 4!\n");
                break;

            case 5:
                printMessage("The process priority is 5!\n");
                break;

            case 6:
                printMessage("The process priority is 6!\n");
                break;

            case 7:
                printMessage("The process priority is 7!\n");
                break;

            case 8:
                printMessage("The process priority is 8!\n");
                break;

            case 9:
                printMessage("The process priority is 9!\n");
                break;

            default:
                break;
            }
        }
    }
}

void showQueue(PCB *pcb, int count)
{
    if (count == 0)
    {
        // the queue is empty
        printMessage("The queue is empty.\n");
        return;
    }
    // The queue is not empty

    int value;
    for (value = 0; value < count; value++)
    {
        // Print out the process
        showPCB(pcb->processName);
        pcb = pcb->nextPCB;
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

    printMessage("The ready queue:\n");
    showQueue(getReady()->head, getReady()->count);
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

    printMessage("The suspended ready queue:\n");
    showQueue(getSuspendedReady()->head, getSuspendedReady()->count);
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

    printMessage("The suspended blocked queue:\n");
    showQueue(getSuspendedBlocked()->head, getSuspendedBlocked()->count);
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

    printMessage("The blocked queue:\n");
    showQueue(getBlocked()->head, getBlocked()->count);
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
    printMessage("\n");

    showSuspendedReady();
    printMessage("\n");

    showBlocked();
    printMessage("\n");

    showSuspendedBlocked();
    printMessage("\n");
}
