// Command Handler

#include <core/serial.h>
#include <string.h>
#include "../mpx_supt.h"
#include "R1commands.h"
#include "../R2/R2commands.h"
#include "../R2/R2_Internal_Functions_And_Structures.h"

int commhand()
{

	char welcomeMSG[] = "Welcome to our CS 450 Project! Type 'help' to see what you can do!\n";
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
		// Testing my functions
		else if (strcmp(cmdBuffer, "showBlocked") == 0)
		{
			showBlocked();
		}

		//
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
