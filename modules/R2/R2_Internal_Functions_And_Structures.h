typedef struct
{
    char processName[20];       // Name of the Process Control Block(PCB).
    unsigned char processClass; //'a'==application process, 's'==system process
    int priority;               // Range from 0-9. 0 is the lowest priority, 9 is the highest priority.
    int runningStatus;          // 0==ready, 1==running, -1==blocked.
    int suspendedStatus;        // 0==suspended, 1==not suspended.
    unsigned char *stack[1024]; // Stack array.
    unsigned char *stackTop;    // Pointer to the top of the stack area for the PCB. (stackTop = PCB->stack + 1024)
    unsigned char *stackBase;   // Pointer to the base of the stack area for the PCB. (stackBase = PCB->stack)
    struct PCB *nextPCB;        // Pointer to the next PCB in the queue.
    struct PCB *PrevPCB;        // Pointer to the previous PCB in the queue.
} PCB;

typedef struct
{
    int count; //Keeps track of how many PCBs are in the queue.
    PCB *head; //Points to the PCB at the head(beginning/top) of the queue.
    PCB *tail; //Points to the PCB at the tail(end/bottom) of the queue.

} queue;

PCB *allocatePCB();

int freePCB(PCB *PCB_to_free);

PCB *setupPCB(char *processName, unsigned char processClass, int processPriority);

PCB *findPCB(char *processName);

void insertPcb(PCB *PCB_to_insert);

int removePCB(PCB *PCB_to_remove);

queue *getReady();

queue *getBlocked();

queue *getSuspendedReady();

queue *getSuspendedBlocked();
PCB* searchPCB(queue*PCB_container, char*processName);