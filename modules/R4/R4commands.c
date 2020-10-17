//R4commands.c

#include <string.h>
#include "../mpx_supt.h"
#include <string.h>
#include <core/serial.h>
#include "../R2/R2_Internal_Functions_And_Structures.h"
#include "../R2/R2commands.h"
#include "../R3/R3commands.h"
#include "R4commands.h"

void alarmPCB()
{

}

void infinitePCB()
{
	createPCB("infinite", 'a', 1);
    PCB *new_pcb = findPCB("infinite");
    context *cp = (context *)(new_pcb->stackTop);
    memset(cp, 0, sizeof(context));
    cp->fs = 0x10;
    cp->gs = 0x10;
    cp->ds = 0x10;
    cp->es = 0x10;
    cp->cs = 0x8;
    cp->ebp = (u32int)(new_pcb->stack);
    cp->esp = (u32int)(new_pcb->stackTop);
    cp->eip = (u32int)infiniteFunc; // The function correlating to the process, ie. Proc1
    cp->eflags = 0x202;

}

void infiniteFunc()
{
	while(1){

		char idleMSG[] = "Infinite Process Executing.\n";
		int idleLength = strlen(idleMSG);

		sys_req(IDLE, DEFAULT_DEVICE, idleMSG, &idleLength);

		sys_req(WRITE, DEFAULT_DEVICE, idleMSG, &idleLength);

	}
}