// Command Handler

#include <core/serial.h>
#include <string.h>
#include "../mpx_supt.h"

int commhand(){

	char cmdBuffer[100];
	int bufferSize; 
	//int quit = 0;

/*
	char help[100] = "1:Help";
	char setDate[100] = "2:Set Date";
	char getDate[100] = "3:Get Date";
	char setTime[100] = "4:Set Time";
	char getTime[100] = "5:Get Time";
	char version[100] = "6:Version";

	printf("%s", help);
	printf("%s", setDate);
	printf("%s", getDate);
	printf("%s", version);

	puts("Please select an option.")
*/



	//while(!quit){
		//get a command: cal polling fx

		memset(cmdBuffer,'\0',100);

		bufferSize = 99; // reset size before each call to read

		sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);

		//sys_req(WRITE, DEFAULT_DEVICE, cmdBuffer, &bufferSize); //Testing write function

		// process the command: take array buffer chars and make a string. Decide what the cmd wants to do
		// see if quit was entered: if string == quit = 1

		return 0;

	//}

}