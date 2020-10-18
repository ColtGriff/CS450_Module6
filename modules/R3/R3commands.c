//R3 commands, functions, and structures

#include <string.h>
#include "../mpx_supt.h"
#include <string.h>
#include <core/serial.h>
#include "../R2/R2_Internal_Functions_And_Structures.h"
#include "../R2/R2commands.h"
#include "R3commands.h"
#include "procsr3.h"


void yield()
{ // temporary command - only in R3
    asm volatile("int $60");
}

void loadr3()
{
    //loadr3 will load all r3 "processes" (proc3.c file eCampus) into memory in a suspended ready state at any priority of your choosing.
    // We may want to change these to use setupPCB instead of createPCB and suspendPCB
    char message[] = "Loading R3 Processes.\n";
    int tempBuffer = strlen(message);
    sys_req(WRITE, DEFAULT_DEVICE, (char *)message, &tempBuffer);

    char newLine[] = "\n";
    tempBuffer = strlen(message);
    sys_req(WRITE, DEFAULT_DEVICE, (char *)newLine, &tempBuffer);

    createPCB("Process1", 'a', 1);
    suspendPCB("Process1");
    PCB *new_pcb1 = findPCB("Process1");
    context *cp1 = (context *)(new_pcb1->stackTop);
    memset(cp1, 0, sizeof(context));
    cp1->fs = 0x10;
    cp1->gs = 0x10;
    cp1->ds = 0x10;
    cp1->es = 0x10;
    cp1->cs = 0x8;
    cp1->ebp = (u32int)(new_pcb1->stack);
    cp1->esp = (u32int)(new_pcb1->stackTop);
    cp1->eip = (u32int)proc1; // The function correlating to the process, ie. Proc1
    cp1->eflags = 0x202;

    createPCB("Process2", 'a', 1);
    suspendPCB("Process2");
    PCB *new_pcb2 = findPCB("Process2");
    context *cp2 = (context *)(new_pcb2->stackTop);
    memset(cp2, 0, sizeof(context));
    cp2->fs = 0x10;
    cp2->gs = 0x10;
    cp2->ds = 0x10;
    cp2->es = 0x10;
    cp2->cs = 0x8;
    cp2->ebp = (u32int)(new_pcb2->stack);
    cp2->esp = (u32int)(new_pcb2->stackTop);
    cp2->eip = (u32int)proc2; // The function correlating to the process, ie. Proc1
    cp2->eflags = 0x202;

    createPCB("Process3", 'a', 1);
    suspendPCB("Process3");
    PCB *new_pcb3 = findPCB("Process3");
    context *cp3 = (context *)(new_pcb3->stackTop);
    memset(cp3, 0, sizeof(context));
    cp3->fs = 0x10;
    cp3->gs = 0x10;
    cp3->ds = 0x10;
    cp3->es = 0x10;
    cp3->cs = 0x8;
    cp3->ebp = (u32int)(new_pcb3->stack);
    cp3->esp = (u32int)(new_pcb3->stackTop);
    cp3->eip = (u32int)proc3; // The function correlating to the process, ie. Proc1
    cp3->eflags = 0x202;

    createPCB("Process4", 'a', 1);
    suspendPCB("Process4");
    PCB *new_pcb4 = findPCB("Process4");
    context *cp4 = (context *)(new_pcb4->stackTop);
    memset(cp4, 0, sizeof(context));
    cp4->fs = 0x10;
    cp4->gs = 0x10;
    cp4->ds = 0x10;
    cp4->es = 0x10;
    cp4->cs = 0x8;
    cp4->ebp = (u32int)(new_pcb4->stack);
    cp4->esp = (u32int)(new_pcb4->stackTop);
    cp4->eip = (u32int)proc4; // The function correlating to the process, ie. Proc1
    cp4->eflags = 0x202;

    createPCB("Process5", 'a', 1);
    suspendPCB("Process5");
    PCB *new_pcb5 = findPCB("Process5");
    context *cp5 = (context *)(new_pcb5->stackTop);
    memset(cp5, 0, sizeof(context));
    cp5->fs = 0x10;
    cp5->gs = 0x10;
    cp5->ds = 0x10;
    cp5->es = 0x10;
    cp5->cs = 0x8;
    cp5->ebp = (u32int)(new_pcb5->stack);
    cp5->esp = (u32int)(new_pcb5->stackTop);
    cp5->eip = (u32int)proc5; // The function correlating to the process, ie. Proc1
    cp5->eflags = 0x202;

}