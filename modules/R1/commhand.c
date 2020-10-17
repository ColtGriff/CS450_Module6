// Command Handler

#include <core/serial.h>
#include <string.h>
#include "../mpx_supt.h"
#include "R1commands.h"
#include "../R2/R2commands.h"
#include "../R2/R2_Internal_Functions_And_Structures.h"
#include "../R3/R3commands.h"

int commhand()
{

	char welcomeMSG[] = "\nWelcome to our CS 450 Project!\nType help to see what you can do!\n\n";
	int welcomeLength = strlen(welcomeMSG);
	sys_req(WRITE, DEFAULT_DEVICE, welcomeMSG, &welcomeLength);

	char cmdBuffer[100];
	int bufferSize;
	allocateQueues();

	int quitFlag = 0;

	while (!quitFlag)
	{
		//get a command: cal polling fx

		memset(cmdBuffer, '\0', 100);

		bufferSize = 99; // reset size before each call to read

		sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);

		char newLine[] = "\n";
		int newLineCount = 1;
		sys_req(WRITE, DEFAULT_DEVICE, newLine, &newLineCount);

		if (strcmp(cmdBuffer, "help") == 0)
		{
			help();
		}
		else if (strcmp(cmdBuffer, "version") == 0)
		{
			version();
		}
		else if (strcmp(cmdBuffer, "getDate") == 0)
		{
			getDate();
		}
		else if (strcmp(cmdBuffer, "setDate") == 0)
		{
			setDate();
		}
		else if (strcmp(cmdBuffer, "getTime") == 0)
		{
			getTime();
		}
		else if (strcmp(cmdBuffer, "setTime") == 0)
		{
			setTime();
		}
		else if (strcmp(cmdBuffer, "createPCB") == 0)
		{
			char processName[20];
			char processClass;
			int processPriority;

			char nameMsg[] = "Please enter a name for the PCB you wish to create. (The name can be no more than 20 characters)\n";
			int nameMsgLen = strlen(nameMsg);
			sys_req(WRITE, DEFAULT_DEVICE, nameMsg, &nameMsgLen);
			sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);
			sys_req(WRITE, DEFAULT_DEVICE, newLine, &newLineCount);
			strcpy(processName, cmdBuffer);
			memset(cmdBuffer, '\0', 100);

			char classMsg[] = "Please enter a class for the PCB you wish to create. ('a' for application or 's' for system)\n";
			int classMsgLen = strlen(classMsg);
			sys_req(WRITE, DEFAULT_DEVICE, classMsg, &classMsgLen);
			sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);
			sys_req(WRITE, DEFAULT_DEVICE, newLine, &newLineCount);
			if (strcmp(cmdBuffer, "a") == 0)
			{
				processClass = 'a';
			}
			else if (strcmp(cmdBuffer, "s") == 0)
			{
				processClass = 's';
			}
			else
			{
				processClass = '\0';
			}
			memset(cmdBuffer, '\0', 100);

			char priorityMsg[] = "Please enter a priority for the PCB you wish to create. (The priorities range from 0 to 9)\n";
			int priorityMsgLen = strlen(priorityMsg);
			sys_req(WRITE, DEFAULT_DEVICE, priorityMsg, &priorityMsgLen);
			sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);
			sys_req(WRITE, DEFAULT_DEVICE, newLine, &newLineCount);
			processPriority = atoi(cmdBuffer);

			createPCB(processName, processClass, processPriority);
		}
		else if (strcmp(cmdBuffer, "deletePCB") == 0)
		{
			char processName[20];

			char nameMsg[] = "Please enter the name for the PCB you wish to delete. (The name can be no more than 20 characters)\n";
			int nameMsgLen = strlen(nameMsg);
			sys_req(WRITE, DEFAULT_DEVICE, nameMsg, &nameMsgLen);
			sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);
			sys_req(WRITE, DEFAULT_DEVICE, newLine, &newLineCount);
			strcpy(processName, cmdBuffer);

			deletePCB(processName);
		}
		else if (strcmp(cmdBuffer, "blockPCB") == 0)
		{
			char processName[20];

			char nameMsg[] = "Please enter the name for the PCB you wish to block. (The name can be no more than 20 characters)\n";
			int nameMsgLen = strlen(nameMsg);
			sys_req(WRITE, DEFAULT_DEVICE, nameMsg, &nameMsgLen);
			sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);
			sys_req(WRITE, DEFAULT_DEVICE, newLine, &newLineCount);
			strcpy(processName, cmdBuffer);

			blockPCB(processName);
		}
		else if (strcmp(cmdBuffer, "unblockPCB") == 0)
		{
			char processName[20];

			char nameMsg[] = "Please enter the name for the PCB you wish to unblock. (The name can be no more than 20 characters)\n";
			int nameMsgLen = strlen(nameMsg);
			sys_req(WRITE, DEFAULT_DEVICE, nameMsg, &nameMsgLen);
			sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);
			sys_req(WRITE, DEFAULT_DEVICE, newLine, &newLineCount);
			strcpy(processName, cmdBuffer);

			unblockPCB(processName);
		}
		else if (strcmp(cmdBuffer, "suspendPCB") == 0)
		{
			char processName[20];

			char nameMsg[] = "Please enter the name for the PCB you wish to suspend. (The name can be no more than 20 characters)\n";
			int nameMsgLen = strlen(nameMsg);
			sys_req(WRITE, DEFAULT_DEVICE, nameMsg, &nameMsgLen);
			sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);
			sys_req(WRITE, DEFAULT_DEVICE, newLine, &newLineCount);
			strcpy(processName, cmdBuffer);

			suspendPCB(processName);
		}
		else if (strcmp(cmdBuffer, "resumePCB") == 0)
		{
			char processName[20];

			char nameMsg[] = "Please enter the name for the PCB you wish to resume. (The name can be no more than 20 characters)\n";
			int nameMsgLen = strlen(nameMsg);
			sys_req(WRITE, DEFAULT_DEVICE, nameMsg, &nameMsgLen);
			sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);
			sys_req(WRITE, DEFAULT_DEVICE, newLine, &newLineCount);
			strcpy(processName, cmdBuffer);

			resumePCB(processName);
		}
		else if (strcmp(cmdBuffer, "setPCBPriority") == 0)
		{
			char processName[20];
			int newProcessPriority;

			char nameMsg[] = "Please enter the name for the PCB you wish to change priorities for. (The name can be no more than 20 characters)\n";
			int nameMsgLen = strlen(nameMsg);
			sys_req(WRITE, DEFAULT_DEVICE, nameMsg, &nameMsgLen);
			sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);
			sys_req(WRITE, DEFAULT_DEVICE, newLine, &newLineCount);
			strcpy(processName, cmdBuffer);

			char priorityMsg[] = "Please enter a priority for the PCB you wish to change priorities for. (The priorities range from 0 to 9)\n";
			int priorityMsgLen = strlen(priorityMsg);
			sys_req(WRITE, DEFAULT_DEVICE, priorityMsg, &priorityMsgLen);
			sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);
			sys_req(WRITE, DEFAULT_DEVICE, newLine, &newLineCount);
			newProcessPriority = atoi(cmdBuffer);

			setPCBPriority(processName, newProcessPriority);
		}
		else if (strcmp(cmdBuffer, "showPCB") == 0)
		{
			char processName[20];

			char nameMsg[] = "Please enter the name for the PCB you wish to see. (The name can be no more than 20 characters)\n";
			int nameMsgLen = strlen(nameMsg);
			sys_req(WRITE, DEFAULT_DEVICE, nameMsg, &nameMsgLen);
			sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);
			sys_req(WRITE, DEFAULT_DEVICE, newLine, &newLineCount);
			strcpy(processName, cmdBuffer);

			showPCB(processName);
		}
		else if (strcmp(cmdBuffer, "showReady") == 0)
		{
			showReady();
		}
		else if (strcmp(cmdBuffer, "showSuspendedReady") == 0)
		{
			showSuspendedReady();
		}
		else if (strcmp(cmdBuffer, "showSuspendedBlocked") == 0)
		{
			showSuspendedBlocked();
		}
		else if (strcmp(cmdBuffer, "showBlocked") == 0)
		{
			showBlocked();
		}
		else if (strcmp(cmdBuffer, "showAll") == 0)
		{
			showAll();
		}
		else if(strcmp(cmdBuffer, "yield") == 0){
			yield();
		}
		else if(strcmp(cmdBuffer, "loadr3") == 0){
			loadr3();
		}
		else if (strcmp(cmdBuffer, "quit") == 0)
		{
			quitFlag = quit();

			sys_req(WRITE, DEFAULT_DEVICE, newLine, &newLineCount);
		}
		else
		{
			char message[] = "Unrecognized Command\n";

			int tempBuffer = strlen(message);

			sys_req(WRITE, DEFAULT_DEVICE, (char *)message, &tempBuffer);
		}

		// process the command: take array buffer chars and make a string. Decide what the cmd wants to do
		// see if quit was entered: if string == quit = 1
	}

	return 0;
}
