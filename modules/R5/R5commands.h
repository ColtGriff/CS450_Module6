typedef struct CMCB
{
    char type;            // indicates if the CMBC is free or allocated ('a' for allocated, 'f' for free).
    u32int beginningAddr; // the beginning address of the CMCB.
    u32int size;          // the size of the memory in bytes.
    //char name[20];        // name of the process (PCB) that is housed in the CMCB.
    struct CMCB *nextCMCB; // pointer to the next CMCB of the same type.
    struct CMCB *prevCMCB; // pointer to the previous CMCB of the same type.
} CMCB;

// typedef struct LMCB
// {
//     char type; // indicates if the LMBC is free or allocated ('a' for allocated, 'f' for free).
//     int size;  // the size of the LMCB in bytes.
// } LMCB;

typedef struct memList
{
    int count;
    CMCB *head;
    CMCB *tail;
} memList;

void allocateMemLists();

u32int initializeHeap(u32int heapSize);

u32int *allocateMemory(u32int size);

int freeMemory(u32int *memToFree);

int isEmpty();

void showFreeMemory();

void showAllocatedMemory();