/*
  ----- kmain.c -----

  Description..: Kernel main. The first function called after
      the bootloader. Initialization of hardware, system
      structures, devices, and initial processes happens here.
      
      Initial Kernel -- by Forrest Desjardin, 2013, 
      Modifications by:    Andrew Duncan 2014,  John Jacko 2017
      				Ben Smith 2018, and Alex Wilson 2019
*/

#include <stdint.h>
#include <string.h>
#include <system.h>

#include <core/io.h>
#include <core/serial.h>
#include <core/tables.h>
#include <core/interrupts.h>
#include <mem/heap.h>
#include <mem/paging.h>

#include "modules/mpx_supt.h"
#include "modules/R1/commhand.h"
#include "modules/R2/R2commands.h"
#include "modules/R2/R2_Internal_Functions_And_Structures.h"
#include "modules/R3/R3commands.h"
#include "modules/R4/R4commands.h"

void kmain(void)
{
   // extern uint32_t magic;
   // Uncomment if you want to access the multiboot header
   // extern void *mbd;
   // char *boot_loader_name = (char*)((long*)mbd)[16];

   // 0) Initialize Serial I/O
   // functions to initialize serial I/O can be found in serial.c
   // there are 3 functions to call

   init_serial(COM1);
   set_serial_in(COM1);
   set_serial_out(COM1);

   klogv("Starting MPX boot sequence...");
   klogv("Initialized serial I/O on COM1 device...");

   // 1) Initialize the support software by identifying the current
   //     MPX Module.  This will change with each module.
   // you will need to call mpx_init from the mpx_supt.c

   mpx_init(MODULE_R2);

   // 2) Check that the boot was successful and correct when using grub
   // Comment this when booting the kernel directly using QEMU, etc.
   //if ( magic != 0x2BADB002 ){
   //  kpanic("Boot was not error free. Halting.");
   //}

   // 3) Descriptor Tables -- tables.c
   //  you will need to initialize the global
   // this keeps track of allocated segments and pages
   klogv("Initializing descriptor tables...");

   init_gdt();
   init_idt();

   init_pic();
   sti();

   // 4)  Interrupt vector table --  tables.c
   // this creates and initializes a default interrupt vector table
   // this function is in tables.c

   init_irq();

   klogv("Interrupt vector table initialized!");

   // 5) Virtual Memory -- paging.c  -- init_paging
   //  this function creates the kernel's heap
   //  from which memory will be allocated when the program calls
   // sys_alloc_mem UNTIL the memory management module  is completed
   // this allocates memory using discrete "pages" of physical memory
   // NOTE:  You will only have about 70000 bytes of dynamic memory
   //
   klogv("Initializing virtual memory...");

   init_paging();

   // 6) Call YOUR command handler -  interface method
   klogv("Transferring control to commhand...");
   //commhand(); //Removed for R4

   allocateQueues();
   //allocateAlarms();

   createPCB("Commhand", 's', 9);
   PCB *new_pcb = findPCB("Commhand");
   context *cp = (context *)(new_pcb->stackTop);
   memset(cp, 0, sizeof(context));
   cp->fs = 0x10;
   cp->gs = 0x10;
   cp->ds = 0x10;
   cp->es = 0x10;
   cp->cs = 0x8;
   cp->ebp = (u32int)(new_pcb->stack);
   cp->esp = (u32int)(new_pcb->stackTop);
   cp->eip = (u32int)commhand; // The function correlating to the process, ie. Proc1
   cp->eflags = 0x202;

   createPCB("Alarm", 'a', 1);
   PCB *AlarmPCB = findPCB("Alarm");
   context *cpAlarm = (context *)(AlarmPCB->stackTop);
   memset(cpAlarm, 0, sizeof(context));
   cpAlarm->fs = 0x10;
   cpAlarm->gs = 0x10;
   cpAlarm->ds = 0x10;
   cpAlarm->es = 0x10;
   cpAlarm->cs = 0x8;
   cpAlarm->ebp = (u32int)(AlarmPCB->stack);
   cpAlarm->esp = (u32int)(AlarmPCB->stackTop);
   cpAlarm->eip = (u32int)alarmPCB; // The function correlating to the process, ie. Proc1
   cpAlarm->eflags = 0x202;

   createPCB("Idle", 's', 0);
   PCB *idlePCB = findPCB("Idle");
   context *cpIDLE = (context *)(idlePCB->stackTop);
   memset(cpIDLE, 0, sizeof(context));
   cpIDLE->fs = 0x10;
   cpIDLE->gs = 0x10;
   cpIDLE->ds = 0x10;
   cpIDLE->es = 0x10;
   cpIDLE->cs = 0x8;
   cpIDLE->ebp = (u32int)(idlePCB->stack);
   cpIDLE->esp = (u32int)(idlePCB->stackTop);
   cpIDLE->eip = (u32int)idle; // The function correlating to the process, ie. Proc1
   cpIDLE->eflags = 0x202;

   asm volatile("int $60");

   // 7) System Shutdown on return from your command handler

   klogv("Starting system shutdown procedure...");

   /* Shutdown Procedure */
   klogv("Shutdown complete. You may now turn off the machine. (QEMU: C-a x)");
   hlt();
}
