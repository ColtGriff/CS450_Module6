#ifndef R2COMMANDS_H
#define R2COMMANDS_H

void createPCB(char *processName, char processClass, int processPriority);

void deletePCB(char *processName);

void blockPCB(char *processName);

void unblockPCB(char *processName);

void suspendPCB(char *processName);

void resumePCB(char *processName);

void setPCBPriority(char *processName, int newProcessPriority);

void showPCB(char *processName);

void showReady();

void showSuspendedBlocked();

void showSuspendedReady();

void showBlocked();

void showAll();
#endif