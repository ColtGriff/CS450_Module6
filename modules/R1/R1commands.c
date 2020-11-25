//R1 Commands

#include <core/serial.h>
#include <string.h>
#include "../mpx_supt.h"
#include "../R2/R2_Internal_Functions_And_Structures.h"
#include "../R2/R2commands.h"
#include <core/io.h>
#include "../utilities.h"

int BCDtoChar(unsigned char test, char *buffer);
unsigned char intToBCD(int test);

void help()
{
	printMessage("help: Returns basic command information.\n");
	printMessage("version: Returns the current version of the software.\n");
	printMessage("getTime: Returns the current set time.\n");
	printMessage("setTime: Allows the user to change the set time.\n");
	printMessage("getDate: Returns the current set date.\n");
	printMessage("setDate: Allows the user to change the set date.\n");
	// printMessage("createPCB: Will create a PCB and put it into the ready queue by default.\n");
	printMessage("deletePCB: Will delete a specific PCB from what ever queue it is in.\n");
	// printMessage("blockPCB: Will change a specific PCB's state to blocked.\n");
	// printMessage("unblockPCB: Will change a specific PCB's state to ready.\n");
	printMessage("suspendPCB: Will suspend a specific PCB.\n");
	printMessage("resumePCB: Will unsuspend a specific PCB.\n");
	printMessage("setPCBPriority: Will change the priority of a specific PCB.\n");
	printMessage("showPCB: Will display the name, class, state, suspended status, and priority of a specific PCB.\n");
	printMessage("showReady: Will display the name, class, state, suspended status, and priority of every PCB in the ready queue.\n");
	printMessage("showSuspendedReady: Will display the name, class, state, suspended status, and priority of every PCB in the suspended ready queue.\n");
	printMessage("showSuspendedBlocked: Will display the name, class, state, suspended status, and priority of every PCB in the suspended blocked queue.\n");
	printMessage("showBlocked: Will display the name, class, state, suspended status, and priority of every PCB in the blocked queue.\n");
	printMessage("showReady: Will display the name, class, state, suspended status, and priority of every PCB in all 4 queues.\n");
	// printMessage("yield: Will cause commhand to voluntarily allow other processes to use the CPU.(removed for R4)\n");
	printMessage("loadr3: Will load all processes for R3. \n");
	printMessage("infinitePCB: Will load a process that executes infinitely until suspended.\n");
	//printMessage("addAlarm: Allows the user to make an alarm. The system is also able to keep track of multiple alarms.\n");
	printMessage("showFreeMemory: Shows all of the free memory in the system.\n");
	printMessage("showAllocatedMemory: Shows all of the allocated memory in the system.\n");
	printMessage("quit: Allows the user to shut the system down.\n");
}

int version()
{
	printMessage("Version 6\n");

	return 0;
}

void getTime()
{

	char buffer[4] = "\0\0\0";
	int count = 4;
	char divider = ':';
	char newLine[1] = "\n";
	int newLineCount = 1;

	outb(0x70, 0x04); // getting Hour value
	BCDtoChar(inb(0x71), buffer);
	buffer[2] = divider;
	sys_req(WRITE, DEFAULT_DEVICE, buffer, &count);
	memset(buffer, '\0', count);

	outb(0x70, 0x02); // getting Minute value
	BCDtoChar(inb(0x71), buffer);
	buffer[2] = divider;
	sys_req(WRITE, DEFAULT_DEVICE, buffer, &count);
	memset(buffer, '\0', count);

	outb(0x70, 0x00); // getting Second value
	BCDtoChar(inb(0x71), buffer);
	buffer[2] = '\0';
	sys_req(WRITE, DEFAULT_DEVICE, buffer, &count);
	memset(buffer, '\0', count);

	sys_req(WRITE, DEFAULT_DEVICE, newLine, &newLineCount);
	memset(newLine, '\0', newLineCount);
}

int setTime()
{

	int count = 4; // counter for printing

	///////// Taking hours input
	printMessage("Please type the desired hours. I.E.: hh.\n");

	char hour[4] = "\0\0\n\0";

	int flag = 0;

	do
	{
		sys_req(READ, DEFAULT_DEVICE, hour, &count);
		if (atoi(hour) < 24 && atoi(hour) >= 0)
		{

			printMessage("\n");
			flag = 0;
		}
		else
		{
			printMessage("\nInvalid hours.\n");
			flag = 1;
		}
	} while (flag == 1);

	///////// Taking minutes input
	printMessage("Please type the desired minutes. I.E.: mm.\n");

	char minute[4] = "\0\0\n\0";

	do
	{
		sys_req(READ, DEFAULT_DEVICE, minute, &count);
		if (atoi(minute) < 60 && atoi(minute) >= 0)
		{

			printMessage("\n");
			flag = 0;
		}
		else
		{
			printMessage("\nInvalid minutes.\n");
			flag = 1;
		}
	} while (flag == 1);

	/////////// Taking seconds input
	printMessage("Please type the desired seconds. I.E.: ss.\n");
	char second[4] = "\0\0\n\0";

	do
	{
		sys_req(READ, DEFAULT_DEVICE, second, &count);
		if (atoi(second) < 60 && atoi(second) >= 0)
		{

			printMessage("\n");
			flag = 0;
		}
		else
		{
			printMessage("Invalid seconds.\n");
			flag = 1;
		}
	} while (flag == 1);

	cli();

	outb(0x70, 0x04); // Hour
	outb(0x71, intToBCD(atoi(hour)));

	outb(0x70, 0x02); // Minute
	outb(0x71, intToBCD(atoi(minute)));

	outb(0x70, 0x00); // Second
	outb(0x71, intToBCD(atoi(second)));

	sti();

	printMessage("The time has been set.\n");

	return 0;
}

void getDate()
{

	char buffer[4] = "\0\0\0\0";
	int count = 4;
	char divider = '/';
	char newLine[1] = "\n";
	int newLineCount = 1;

	outb(0x70, 0x07); // getting Day of month value
	BCDtoChar(inb(0x71), buffer);
	buffer[2] = divider;
	sys_req(WRITE, DEFAULT_DEVICE, buffer, &count);
	memset(buffer, '\0', count);

	outb(0x70, 0x08); // getting Month value
	BCDtoChar(inb(0x71), buffer);
	buffer[2] = divider;
	sys_req(WRITE, DEFAULT_DEVICE, buffer, &count);
	memset(buffer, '\0', count);

	outb(0x70, 0x32); // getting Year value second byte
	BCDtoChar(inb(0x71), buffer);
	buffer[2] = '\0';
	sys_req(WRITE, DEFAULT_DEVICE, buffer, &count);
	memset(buffer, '\0', count);

	outb(0x70, 0x09); // getting Year value first byte
	BCDtoChar(inb(0x71), buffer);
	sys_req(WRITE, DEFAULT_DEVICE, buffer, &count);
	memset(buffer, '\0', count);

	sys_req(WRITE, DEFAULT_DEVICE, newLine, &newLineCount);
	memset(newLine, '\0', newLineCount);
}

int setDate()
{

	int count = 4; // used to print year

	/////////// Taking year input
	printMessage("Please type the desired year. I.E.: yyyy.\n");

	char year[5] = "\0\0\0\0\0"; // year buffer

	int flag = 0; // thrown if input is invalid

	do
	{
		sys_req(READ, DEFAULT_DEVICE, year, &count);
		if (atoi(year) > 0)
		{

			printMessage("\n");
			flag = 0;

			char yearUpper[3] = "\0\0\0";
			char yearLower[3] = "\0\0\0";

			yearUpper[0] = year[0];
			yearUpper[1] = year[1];
			yearLower[0] = year[2];
			yearLower[1] = year[3];

			cli();

			outb(0x70, 0x32); // Setting first byte year value
			outb(0x71, intToBCD(atoi(yearUpper)));

			outb(0x70, 0x09); // Setting second byte year value
			outb(0x71, intToBCD(atoi(yearLower)));

			sti();
		}
		else
		{
			printMessage("\nInvalid year.\n");
			flag = 1;
		}
	} while (flag == 1);

	/////////// Taking month input
	printMessage("Please type the desired month. I.E.: mm.\n");

	char month[4] = "\0\0\n\0";
	count = 4; // used to print month

	do
	{
		sys_req(READ, DEFAULT_DEVICE, month, &count);
		if (atoi(month) < 13 && atoi(month) > 0)
		{

			printMessage("\n");
			flag = 0;

			cli();

			outb(0x70, 0x08); // Setting month value
			outb(0x71, intToBCD(atoi(month)));

			sti();
		}
		else
		{
			printMessage("\nInvalid month.\n");
			flag = 1;
		}
	} while (flag == 1);

	/////////// Taking day input
	printMessage("Please type the desired day of month. I.E.: dd.\n");

	char day[4] = "\0\0\n\0";
	count = 4; // used to print day

	do
	{
		sys_req(READ, DEFAULT_DEVICE, day, &count);
		printMessage("\n");
		if ((atoi(year) % 4 == 0 && atoi(year) % 100 != 0) || atoi(year) % 400 == 0)
		{ // checking for leap year

			printMessage("This is a leap year. February has 29 days.\n");

			if ((atoi(month) == 1 || atoi(month) == 3 || atoi(month) == 5 || atoi(month) == 7 || atoi(month) == 8 || atoi(month) == 10 || atoi(month) == 12) && atoi(day) > 31)
			{
				flag = 1;
				printMessage("Invalid day.\n");
			}
			else if ((atoi(month) == 4 || atoi(month) == 6 || atoi(month) == 9 || atoi(month) == 11) && atoi(day) > 30)
			{
				flag = 1;
				printMessage("Invalid day.\n");
			}
			else if ((atoi(month) == 2) && atoi(day) > 29)
			{
				flag = 1;
				printMessage("Invalid day.\n");
			}
			else
			{

				flag = 0;
				cli();

				outb(0x70, 0x07); // Setting day of month value
				outb(0x71, intToBCD(atoi(day)));

				sti();
			}
		}
		else if (atoi(year) % 4 != 0 || atoi(year) % 400 != 0)
		{ // checking for leap year

			printMessage("This is not a leap year.\n");

			if ((atoi(month) == 1 || atoi(month) == 3 || atoi(month) == 5 || atoi(month) == 7 || atoi(month) == 8 || atoi(month) == 10 || atoi(month) == 12) && atoi(day) > 31)
			{
				flag = 1;
				printMessage("Invalid day.\n");
			}
			else if ((atoi(month) == 4 || atoi(month) == 6 || atoi(month) == 9 || atoi(month) == 11) && atoi(day) > 30)
			{
				flag = 1;
				printMessage("Invalid day.\n");
			}
			else if ((atoi(month) == 2) && atoi(day) > 28)
			{
				flag = 1;
				printMessage("Invalid day.\n");
			}
			else
			{

				cli();

				outb(0x70, 0x07); // Setting day of month value
				outb(0x71, intToBCD(atoi(day)));

				sti();
			}
		}

	} while (flag == 1);

	printMessage("The date has been set.\n");
	return 0;
}

unsigned char intToBCD(int test)
{

	return (((test / 10) << 4) | (test % 10));
}

int BCDtoChar(unsigned char test, char *buffer)
{

	int val1 = (test / 16);
	int val2 = (test % 16);

	buffer[0] = val1 + '0';
	buffer[1] = val2 + '0';

	return 0;
}

void deleteQueue(queue *queue)
{
	PCB *tempPtr;
	int loop;
	for (loop = 0; loop < queue->count; loop++)
	{
		tempPtr = queue->head;
		removePCB(tempPtr);
	}
}

void removeAll()
{
	if (getReady()->head != NULL)
	{
		deleteQueue(getReady());
	}

	if (getBlocked()->head != NULL)
	{
		deleteQueue(getBlocked());
	}

	if (getSuspendedBlocked()->head != NULL)
	{
		deleteQueue(getSuspendedBlocked());
	}

	if (getSuspendedReady()->head != NULL)
	{
		deleteQueue(getSuspendedReady());
	}
}

int quit()
{
	int flag = 0;

	printMessage("Are you sure you want to shutdown? y/n\n");

	char quitAns[] = "\0\0";
	int quitAnsLength = 1;
	sys_req(READ, DEFAULT_DEVICE, quitAns, &quitAnsLength);
	char answer = quitAns[0];

	if (answer == 'y' || answer == 'Y')
	{
		flag = 1;
		//removeAll processes.
		removeAll();
		printMessage("\n");
	}
	else if (answer == 'n' || answer == 'N')
	{
		flag = 0;
		printMessage("\n");
	}
	else
	{
		printMessage("Invalid input!\n");
	}

	return flag;
}