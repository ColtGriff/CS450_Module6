// Command Handler

int comhand(){

	char cmdBuffer[100];
	int bufferSize; 
	int quit = 0;

	while(!quit){
		//get a command: cal polling fx

		memset(buffer,'\0',100);

		bufferSize = 99; // reset size before each call to read

		sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);

		// process the command: take array buffer chars and make a string. Decide what the cmd wants to do
		// see if quit was entered: if string == quit = 1

	}

}