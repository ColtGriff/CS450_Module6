//R3 commands, functions, and structures

#include <string.h>
#include "../mpx_supt.h"
#include <string.h>
#include <core/serial.h>
#include "../R2/R2_Internal_Functions_And_Structures.h"
#include "../R2/R2commands.h"
#include "R3commands.h"

param params;

PCB *COP;
context *callerContext;

void sys_call_isr()
{
    // Push all general purpose register values to the stack.
    callerContext->edi = COP->stack[0];
    callerContext->esi = COP->stack[1];
    callerContext->ebp = COP->stack[2];
    callerContext->esp = COP->stack[3];
    callerContext->ebx = COP->stack[4];
    callerContext->edx = COP->stack[5];
    callerContext->ecx = COP->stack[6];
    callerContext->eax = COP->stack[7];
    callerContext->eip = COP->stack[8];
    callerContext->cs = COP->stack[9];
    callerContext->eflags = COP->stack[10];

    // Push the segment register values to the stack.
    callerContext->ds = COP->stack[11];
    callerContext->es = COP->stack[12];
    callerContext->fs = COP->stack[13];
    callerContext->gs = COP->stack[14];

    // Call sys_call and set a new stack pointer.
    callerContext->eax = sys_call(callerContext->esp);

    COP->stack[14] = callerContext->gs;
}

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
            COP = COP->stackTop;
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
            return COP->stackTop;
        }
    }
    else
    {
        return registers;
    }
}

void yield()
{ // temporary command - only in R3
    asm volatile("int $60");
}

PCB *loadr3()
{
    //loadr3 will load all r3 "processes" (proc3.c file eCampus) into memory in a suspended ready state at any priority of your choosing.
    return NULL;
}