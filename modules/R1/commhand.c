// Command Handler

#include <core/serial.h>
#include <string.h>
#include "../mpx_supt.h"
#include "R1commands.h"

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
			//serial_print("Help function\n");
			help();
		} else if(strcmp(cmdBuffer, "version") == 0){
<<<<<<< HEAD
			//serial_print("version function\n");
=======
			serial_print("version function\n");
>>>>>>> 59814169b90d1e8ae8a2a1ecb8c3ef576ba2f5f4
			version();
		} else if(strcmp(cmdBuffer, "getDate") == 0){
			//serial_print("getDate function\n");
			getDate();
		} else if(strcmp(cmdBuffer, "setDate") == 0){
			//serial_print("setDate function\n");
			setDate();
		} else if(strcmp(cmdBuffer, "getTime") == 0){
			//serial_print("getTime function\n");
			getTime();
		} else if(strcmp(cmdBuffer, "setTime") == 0){
			//serial_print("setTime function\n");
			setTime();
		} else if(strcmp(cmdBuffer, "quit") == 0){


			// Need a check here


			quit = 1;
		} else {
			char message[] = "Unrecognized Command\n";

<<<<<<< HEAD
			int tempBuffer = strlen(message);
=======
		//sys_req(WRITE, DEFAULT_DEVICE, cmdBuffer, &bufferSize); //Testing write function
>>>>>>> 59814169b90d1e8ae8a2a1ecb8c3ef576ba2f5f4

			sys_req(WRITE, DEFAULT_DEVICE, (char*)message, &tempBuffer);

		}

		// process the command: take array buffer chars and make a string. Decide what the cmd wants to do
		// see if quit was entered: if string == quit = 1

	}

	return 0;
}



