// Command Handler

#include <core/serial.h>
#include <string.h>
#include "../mpx_supt.h"

int commhand(){

	char cmdBuffer[100];
	int bufferSize; 

	int quit = 0;


	while(!quit){
		//get a command: cal polling fx

		memset(cmdBuffer,'\0',100);

		bufferSize = 99; // reset size before each call to read

		sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);

		serial_print("\n");

		if(strcmp(cmdBuffer, "help") == 0){
			serial_print("Help function\n");
		} else if(strcmp(cmdBuffer, "version") == 0){
			serial_print("version function\n");
		} else if(strcmp(cmdBuffer, "getDate") == 0){
			serial_print("getDate function\n");
		} else if(strcmp(cmdBuffer, "setDate") == 0){
			serial_print("setDate function\n");
		} else if(strcmp(cmdBuffer, "getTime") == 0){
			serial_print("getTime function\n");
		} else if(strcmp(cmdBuffer, "setTime") == 0){
			serial_print("setTime function\n");
		} else if(strcmp(cmdBuffer, "quit") == 0){
			quit = 1;
		} else {
			serial_print("Unrecognized command!\n");
		}

		sys_req(WRITE, DEFAULT_DEVICE, cmdBuffer, &bufferSize); //Testing write function

		serial_print("\n");

		// process the command: take array buffer chars and make a string. Decide what the cmd wants to do
		// see if quit was entered: if string == quit = 1

	}


	return 0;

}

