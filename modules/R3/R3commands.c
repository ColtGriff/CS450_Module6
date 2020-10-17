//R3 commands, functions, and structures

#include <string.h>
#include "../mpx_supt.h"
#include <string.h>
#include <core/serial.h>
#include "../R2/R2_Internal_Functions_And_Structures.h"
#include "../R2/R2commands.h"
#include "R3commands.h"
#include "procsr3.h"

param params;

PCB *COP;
context *callerContext;

u32int *sys_call(context *registers)
{ // Benjamin and Anastase programmed this function
    if (COP == NULL)
    { // sys_call has not been called yet.
        callerContext = registers;
    }
    else
    {
        if (params.op_code == IDLE)
        { // Save the context (reassign COP's stack top).
            COP->stackTop = (unsigned char *)registers;
        }
        else if (params.op_code == EXIT)
        { // free COP.
            sys_free_mem(COP);
        }
    }

    queue *ready = getReady();
    PCB *tempPtr = ready->head;

    if (tempPtr != NULL)
    {
        int removed = removePCB(tempPtr);
        if (removed == 0)
        {
            tempPtr->runningStatus = 1;
            COP = tempPtr;
            return (u32int *)COP->stackTop;
        }
    }
    return (u32int *)registers;
}

void yield()
{ // temporary command - only in R3
    asm volatile("int $60");
}

PCB *loadr3()
{
    //loadr3 will load all r3 "processes" (proc3.c file eCampus) into memory in a suspended ready state at any priority of your choosing.

    createPCB("Process1", 'a', 1);
    suspendPCB("Process1");
    PCB *new_pcb = findPCB("Process1");
    context *cp = (context *)(new_pcb->stackTop);
    memset(cp, 0, sizeof(context));
    cp->fs = 0x10;
    cp->gs = 0x10;
    cp->ds = 0x10;
    cp->es = 0x10;
    cp->cs = 0x8;
    cp->ebp = (u32int)(new_pcb->stack);
    cp->esp = (u32int)(new_pcb->stackTop);
    cp->eip = (u32int)proc1; // The function correlating to the process, ie. Proc1
    cp->eflags = 0x202;

    createPCB("Process2", 'a', 1);
    suspendPCB("Process2");
    PCB *new_pcb = findPCB("Process2");
    context *cp = (context *)(new_pcb->stackTop);
    memset(cp, 0, sizeof(context));
    cp->fs = 0x10;
    cp->gs = 0x10;
    cp->ds = 0x10;
    cp->es = 0x10;
    cp->cs = 0x8;
    cp->ebp = (u32int)(new_pcb->stack);
    cp->esp = (u32int)(new_pcb->stackTop);
    cp->eip = (u32int)proc2; // The function correlating to the process, ie. Proc1
    cp->eflags = 0x202;

    createPCB("Process3", 'a', 1);
    suspendPCB("Process3");
    PCB *new_pcb = findPCB("Process3");
    context *cp = (context *)(new_pcb->stackTop);
    memset(cp, 0, sizeof(context));
    cp->fs = 0x10;
    cp->gs = 0x10;
    cp->ds = 0x10;
    cp->es = 0x10;
    cp->cs = 0x8;
    cp->ebp = (u32int)(new_pcb->stack);
    cp->esp = (u32int)(new_pcb->stackTop);
    cp->eip = (u32int)proc3; // The function correlating to the process, ie. Proc1
    cp->eflags = 0x202;

    createPCB("Process4", 'a', 1);
    suspendPCB("Process4");
    PCB *new_pcb = findPCB("Process4");
    context *cp = (context *)(new_pcb->stackTop);
    memset(cp, 0, sizeof(context));
    cp->fs = 0x10;
    cp->gs = 0x10;
    cp->ds = 0x10;
    cp->es = 0x10;
    cp->cs = 0x8;
    cp->ebp = (u32int)(new_pcb->stack);
    cp->esp = (u32int)(new_pcb->stackTop);
    cp->eip = (u32int)proc4; // The function correlating to the process, ie. Proc1
    cp->eflags = 0x202;

    createPCB("Process5", 'a', 1);
    suspendPCB("Process5");
    PCB *new_pcb = findPCB("Process5");
    context *cp = (context *)(new_pcb->stackTop);
    memset(cp, 0, sizeof(context));
    cp->fs = 0x10;
    cp->gs = 0x10;
    cp->ds = 0x10;
    cp->es = 0x10;
    cp->cs = 0x8;
    cp->ebp = (u32int)(new_pcb->stack);
    cp->esp = (u32int)(new_pcb->stackTop);
    cp->eip = (u32int)proc5; // The function correlating to the process, ie. Proc1
    cp->eflags = 0x202;
}