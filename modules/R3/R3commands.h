//R3commands.h

typedef struct context
{
    u32int gs, fs, es, ds;
    u32int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    u32int eip, cs, eflags;
} context;

void sys_call_isr();

u32int *sys_call(context *registers);

void yield();

void loadr3();