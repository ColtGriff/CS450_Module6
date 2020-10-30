// Command Handler

#include <core/serial.h>
#include <string.h>
#include "../mpx_supt.h"
#include "../utilities.h"
#include "R1commands.h"
#include "../R2/R2commands.h"
#include "../R2/R2_Internal_Functions_And_Structures.h"
#include "../R3/R3commands.h"
#include "../R4/R4commands.h"

void commhand()
{

	printMessage("\nWelcome to our CS 450 Project!\nType help to see what you can do!\n\n");

	char cmdBuffer[100];
	int bufferSize;
	char processName[20];
	int processPriority;

	int quitFlag = 0;

	while (!quitFlag)
	{
		//get a command: cal polling fx

		memset(cmdBuffer, '\0', 100);

		bufferSize = 99; // reset size before each call to read

		sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);

		printMessage("\n");

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
		// else if (strcmp(cmdBuffer, "createPCB") == 0)
		// {
		// 	printMessage("Please enter a name for the PCB you wish to create. (The name can be no more than 20 characters)\n");
		// 	sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);
		// 	printMessage("\n");
		// 	strcpy(processName, cmdBuffer);
		// 	memset(cmdBuffer, '\0', 100);

		// 	printMessage("Please enter a class for the PCB you wish to create. ('a' for application or 's' for system)\n");
		// 	sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);
		// 	printMessage("\n");
		// 	if (strcmp(cmdBuffer, "a") == 0)
		// 	{
		// 		processClass = 'a';
		// 	}
		// 	else if (strcmp(cmdBuffer, "s") == 0)
		// 	{
		// 		processClass = 's';
		// 	}
		// 	else
		// 	{
		// 		processClass = '\0';
		// 	}
		// 	memset(cmdBuffer, '\0', 100);

		// 	printMessage("Please enter a priority for the PCB you wish to create. (The priorities range from 0 to 9)\n");
		// 	sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);
		// 	printMessage("\n");
		// 	processPriority = atoi(cmdBuffer);

		// 	createPCB(processName, processClass, processPriority);
		// }
		else if (strcmp(cmdBuffer, "deletePCB") == 0)
		{
			printMessage("Please enter the name for the PCB you wish to delete. (The name can be no more than 20 characters)\n");
			sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);
			printMessage("\n");
			strcpy(processName, cmdBuffer);

			deletePCB(processName);
		}
		// else if (strcmp(cmdBuffer, "blockPCB") == 0)
		// {
		// 	printMessage("Please enter the name for the PCB you wish to block. (The name can be no more than 20 characters)\n");
		// 	sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);
		// 	printMessage("\n");
		// 	strcpy(processName, cmdBuffer);

		// 	blockPCB(processName);
		// }
		// else if (strcmp(cmdBuffer, "unblockPCB") == 0)
		// {
		// 	printMessage("Please enter the name for the PCB you wish to unblock. (The name can be no more than 20 characters)\n");
		// 	sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);
		// 	printMessage("\n");
		// 	strcpy(processName, cmdBuffer);

		// 	unblockPCB(processName);
		// }
		else if (strcmp(cmdBuffer, "suspendPCB") == 0)
		{
			printMessage("Please enter the name for the PCB you wish to suspend. (The name can be no more than 20 characters)\n");
			sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);
			printMessage("\n");
			strcpy(processName, cmdBuffer);

			suspendPCB(processName);
		}
		else if (strcmp(cmdBuffer, "resumePCB") == 0)
		{
			printMessage("Please enter the name for the PCB you wish to resume. (The name can be no more than 20 characters)\n");
			sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);
			printMessage("\n");
			strcpy(processName, cmdBuffer);

			resumePCB(processName);
		}
		else if (strcmp(cmdBuffer, "setPCBPriority") == 0)
		{
			printMessage("Please enter the name for the PCB you wish to change priorities for. (The name can be no more than 20 characters)\n");
			sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);
			printMessage("\n");
			strcpy(processName, cmdBuffer);

			printMessage("Please enter a priority for the PCB you wish to change priorities for. (The priorities range from 0 to 9)\n");
			sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);
			printMessage("\n");
			processPriority = atoi(cmdBuffer);

			setPCBPriority(processName, processPriority);
		}
		else if (strcmp(cmdBuffer, "showPCB") == 0)
		{
			printMessage("Please enter the name for the PCB you wish to see. (The name can be no more than 20 characters)\n");
			sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);
			printMessage("\n");
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
		// else if (strcmp(cmdBuffer, "yield") == 0)
		// {
		// 	yield();
		// }
		else if (strcmp(cmdBuffer, "loadr3") == 0)
		{
			loadr3();
		}
		else if (strcmp(cmdBuffer, "infinitePCB") == 0)
		{
			infinitePCB();
		}
		// else if (strcmp(cmdBuffer, "addAlarm") == 0)
		// {
		// 	addAlarm();
		// }
		else if (strcmp(cmdBuffer, "quit") == 0)
		{
			quitFlag = quit();

			if (quitFlag == 1)
			{
				sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);
			}

			printMessage("\n");
		}
		else
		{
			printMessage("Unrecognized Command\n");
		}

		sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);

		// process the command: take array buffer chars and make a string. Decide what the cmd wants to do
		// see if quit was entered: if string == quit = 1
	}
}
