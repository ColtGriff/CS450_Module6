typedef struct CMCB
{
    char type;         // indicates if the CMBC is free or allocated ('a' for allocated, 'f' for free).
    int beginningAddr; // the beginning address of the CMCB.
    int size;          //
    char name[];
    CMCB *nextCMCB;
    CMCB *prevCMCB;
} CMCB;

typedef struct LMCB
{
    char type;
    int size;
} LMCB;