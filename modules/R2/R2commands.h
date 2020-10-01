void createPCB(char *processName, unsigned char processClass, int processPriority);

void deletePCB(char *processName);

void blockPCB(char *processName);

void unblockPCB(char *processName);

void suspendPCB(char *processName);

void resumePCB(char *processName);

void setPCBPriority(char *processName, int newProcessPriority);

void showPCB(char *processName);

void showReady();

void showBlocked();

void showAll();
//char* int_to_char(int value);
//void printPCBs(queue*PCB_container);