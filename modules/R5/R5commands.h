typedef struct CMCB
{
    char type;         // indicates if the CMBC is free or allocated ('a' for allocated, 'f' for free).
    int beginningAddr; // the beginning address of the CMCB.
    int size;          // the size of the CMCB in bytes.
    char name[20];     // name of the process(PCB) that is housed in the CMCB.
    CMCB *nextCMCB;    // pointer to the next CMCB of the same type.
    CMCB *prevCMCB;    // pointer to the previous CMCB of the same type.
} CMCB;

typedef struct LMCB
{
    char type; // indicates if the LMBC is free or allocated ('a' for allocated, 'f' for free).
    int size;  // the size of the LMCB in bytes.
} LMCB;

void initializeHeap();

void allocateMemory();

void freeMemory();

void isEmpty();

void showFreeMemory();

void showAllocatedMemory();