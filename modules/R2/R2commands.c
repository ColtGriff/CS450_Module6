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
}