//R1 Commands

#include <core/serial.h>
#include <string.h>
#include "../mpx_supt.h"
#include <core/io.h>

int BCDtoChar(unsigned char test, char *buffer);
unsigned char intToBCD(int test);

int help()
{

	// Help Description section
	char helpDesc[] = "help: Returns basic command information.\n";

	int tempBuffer = strlen(helpDesc);

	sys_req(WRITE, DEFAULT_DEVICE, (char *)helpDesc, &tempBuffer);
	memset(helpDesc, '\0', tempBuffer);

	// Version Description section
	char versionDesc[] = "version: Returns the current version of the software.\n";

	tempBuffer = strlen(versionDesc);

	sys_req(WRITE, DEFAULT_DEVICE, (char *)versionDesc, &tempBuffer);
	memset(versionDesc, '\0', tempBuffer);

	// getTime Description section
	char getTimeDesc[] = "getTime: Returns the current set time.\n";

	tempBuffer = strlen(getTimeDesc);

	sys_req(WRITE, DEFAULT_DEVICE, (char *)getTimeDesc, &tempBuffer);
	memset(getTimeDesc, '\0', tempBuffer);

	// setTime Description section
	char setTimeDesc[] = "setTime: Allows the user to change the set time.\n";

	tempBuffer = strlen(setTimeDesc);

	sys_req(WRITE, DEFAULT_DEVICE, (char *)setTimeDesc, &tempBuffer);
	memset(setTimeDesc, '\0', tempBuffer);

	// getDate Description section
	char getDateDesc[] = "getDate: Returns the current set date.\n";

	tempBuffer = strlen(getDateDesc);

	sys_req(WRITE, DEFAULT_DEVICE, (char *)getDateDesc, &tempBuffer);
	memset(getDateDesc, '\0', tempBuffer);

	// setDate Description section
	char setDateDesc[] = "setDate: Allows the user to change the set date.\n";

	tempBuffer = strlen(setDateDesc);

	sys_req(WRITE, DEFAULT_DEVICE, (char *)setDateDesc, &tempBuffer);
	memset(setDateDesc, '\0', tempBuffer);

	// quit Description section
	char quitDesc[] = "quit: Allows the user to shut the system down.\n";

	tempBuffer = strlen(quitDesc);

	sys_req(WRITE, DEFAULT_DEVICE, (char *)quitDesc, &tempBuffer);
	memset(quitDesc, '\0', tempBuffer);

	return 0;
}

int version()
{

	char version[] = "Version 1.0\n";

	int tempBuffer = strlen(version);

	sys_req(WRITE, DEFAULT_DEVICE, (char *)version, &tempBuffer);
	memset(version, '\0', tempBuffer);

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

	char spacer[1] = "\n"; // used to space out terminal outputs
	int spaceCount = 1;

	///////// Taking hours input
	char instruction1[] = "Please type the desired hours. I.E.: hh.\n";

	int length = strlen(instruction1);

	sys_req(WRITE, DEFAULT_DEVICE, instruction1, &length);
	memset(instruction1, '\0', length);

	char hour[4] = "\0\0\n\0";

	int flag = 0;

	do
	{
		sys_req(READ, DEFAULT_DEVICE, hour, &count);
		if (atoi(hour) < 24 && atoi(hour) >= 0)
		{

			sys_req(WRITE, DEFAULT_DEVICE, spacer, &spaceCount);
			flag = 0;
		}
		else
		{
			char invalid[] = "Invalid hours.\n";
			int lengthInval = strlen(invalid);
			sys_req(WRITE, DEFAULT_DEVICE, spacer, &spaceCount);
			sys_req(WRITE, DEFAULT_DEVICE, invalid, &lengthInval);
			memset(invalid, '\0', lengthInval);
			flag = 1;
		}
	} while (flag == 1);

	///////// Taking minutes input
	char instruction2[] = "Please type the desired minutes. I.E.: mm.\n";

	length = strlen(instruction2);

	sys_req(WRITE, DEFAULT_DEVICE, instruction2, &length);
	memset(instruction2, '\0', length);

	char minute[4] = "\0\0\n\0";

	do
	{
		sys_req(READ, DEFAULT_DEVICE, minute, &count);
		if (atoi(minute) < 60 && atoi(minute) >= 0)
		{

			sys_req(WRITE, DEFAULT_DEVICE, spacer, &spaceCount);
			flag = 0;
		}
		else
		{
			char invalid[] = "Invalid minutes.\n";
			int lengthInval = strlen(invalid);
			sys_req(WRITE, DEFAULT_DEVICE, spacer, &spaceCount);
			sys_req(WRITE, DEFAULT_DEVICE, invalid, &lengthInval);
			memset(invalid, '\0', lengthInval);
			flag = 1;
		}
	} while (flag == 1);

	/////////// Taking seconds input
	char instruction3[] = "Please type the desired seconds. I.E.: ss.\n";

	length = strlen(instruction3);

	sys_req(WRITE, DEFAULT_DEVICE, instruction3, &length);
	memset(instruction3, '\0', length);

	char second[4] = "\0\0\n\0";

	do
	{
		sys_req(READ, DEFAULT_DEVICE, second, &count);
		if (atoi(second) < 60 && atoi(second) >= 0)
		{

			sys_req(WRITE, DEFAULT_DEVICE, spacer, &spaceCount);
			flag = 0;
		}
		else
		{
			char invalid[] = "Invalid seconds.\n";
			int lengthInval = strlen(invalid);
			sys_req(WRITE, DEFAULT_DEVICE, spacer, &spaceCount);
			sys_req(WRITE, DEFAULT_DEVICE, invalid, &lengthInval);
			memset(invalid, '\0', lengthInval);
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

	char exitMessage[] = "The time has been set.\n";
	int exitLength = strlen(exitMessage);
	sys_req(WRITE, DEFAULT_DEVICE, exitMessage, &exitLength);
	memset(exitMessage, '\0', exitLength);
	memset(spacer, '\0', spaceCount);

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

	char spacer[1] = "\n"; // used to space out terminal outputs
	int spaceCount = 1;

	/////////// Taking year input
	char instruction1[] = "Please type the desired year. I.E.: yyyy.\n";
	int length = strlen(instruction1);

	sys_req(WRITE, DEFAULT_DEVICE, instruction1, &length);
	memset(instruction1, '\0', length);

	char year[5] = "\0\0\0\0\0"; // year buffer

	int flag = 0; // thrown if input is invalid

	do
	{
		sys_req(READ, DEFAULT_DEVICE, year, &count);
		if (atoi(year) > 0)
		{

			sys_req(WRITE, DEFAULT_DEVICE, spacer, &spaceCount);
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
			char invalid[] = "Invalid year.\n";
			int lengthInval = strlen(invalid);
			sys_req(WRITE, DEFAULT_DEVICE, spacer, &spaceCount);
			sys_req(WRITE, DEFAULT_DEVICE, invalid, &lengthInval);
			memset(invalid, '\0', lengthInval);
			flag = 1;
		}
	} while (flag == 1);

	/////////// Taking month input
	char instruction2[] = "Please type the desired month. I.E.: mm.\n";
	length = strlen(instruction2);

	sys_req(WRITE, DEFAULT_DEVICE, instruction2, &length);
	memset(instruction2, '\0', length);

	char month[4] = "\0\0\n\0";
	count = 4; // used to print month

	do
	{
		sys_req(READ, DEFAULT_DEVICE, month, &count);
		if (atoi(month) < 13 && atoi(month) > 0)
		{

			sys_req(WRITE, DEFAULT_DEVICE, spacer, &spaceCount);
			flag = 0;

			cli();

			outb(0x70, 0x08); // Setting month value
			outb(0x71, intToBCD(atoi(month)));

			sti();
		}
		else
		{
			char invalid[] = "Invalid month.\n";
			int lengthInval = strlen(invalid);
			sys_req(WRITE, DEFAULT_DEVICE, spacer, &spaceCount);
			sys_req(WRITE, DEFAULT_DEVICE, invalid, &lengthInval);
			memset(invalid, '\0', lengthInval);
			flag = 1;
		}
	} while (flag == 1);

	/////////// Taking day input
	char instruction3[] = "Please type the desired day of month. I.E.: dd.\n";

	length = strlen(instruction3);
	sys_req(WRITE, DEFAULT_DEVICE, instruction3, &length);
	memset(instruction3, '\0', length);

	char day[4] = "\0\0\n\0";
	count = 4; // used to print day

	do
	{
		sys_req(READ, DEFAULT_DEVICE, day, &count);
		sys_req(WRITE, DEFAULT_DEVICE, spacer, &spaceCount);
		if ((atoi(year) % 4 == 0 && atoi(year) % 100 != 0) || atoi(year) % 400 == 0)
		{ // checking for leap year

			char leapYear[] = "This is a leap year. February has 29 days.\n";
			length = strlen(leapYear);

			sys_req(WRITE, DEFAULT_DEVICE, leapYear, &length);
			memset(leapYear, '\0', length);

			if ((atoi(month) == 1 || atoi(month) == 3 || atoi(month) == 5 || atoi(month) == 7 || atoi(month) == 8 || atoi(month) == 10 || atoi(month) == 12) && atoi(day) > 31)
			{
				flag = 1;
				char invalid[] = "Invalid day.\n";
				length = strlen(invalid);
				sys_req(WRITE, DEFAULT_DEVICE, invalid, &length);
				memset(invalid, '\0', length);
			}
			else if ((atoi(month) == 4 || atoi(month) == 6 || atoi(month) == 9 || atoi(month) == 11) && atoi(day) > 30)
			{
				flag = 1;
				char invalid[] = "Invalid day.\n";
				length = strlen(invalid);
				sys_req(WRITE, DEFAULT_DEVICE, invalid, &length);
				memset(invalid, '\0', length);
			}
			else if ((atoi(month) == 2) && atoi(day) > 29)
			{
				flag = 1;
				char invalid[] = "Invalid day.\n";
				length = strlen(invalid);
				sys_req(WRITE, DEFAULT_DEVICE, invalid, &length);
				memset(invalid, '\0', length);
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

			char noLeap[] = "This is not a leap year.\n";
			length = strlen(noLeap);
			sys_req(WRITE, DEFAULT_DEVICE, noLeap, &length);
			memset(noLeap, '\0', length);

			if ((atoi(month) == 1 || atoi(month) == 3 || atoi(month) == 5 || atoi(month) == 7 || atoi(month) == 8 || atoi(month) == 10 || atoi(month) == 12) && atoi(day) > 31)
			{
				flag = 1;
				char invalid[] = "Invalid day.\n";
				length = strlen(invalid);
				sys_req(WRITE, DEFAULT_DEVICE, invalid, &length);
				memset(invalid, '\0', length);
			}
			else if ((atoi(month) == 4 || atoi(month) == 6 || atoi(month) == 9 || atoi(month) == 11) && atoi(day) > 30)
			{
				flag = 1;
				char invalid[] = "Invalid day.\n";
				length = strlen(invalid);
				sys_req(WRITE, DEFAULT_DEVICE, invalid, &length);
				memset(invalid, '\0', length);
			}
			else if ((atoi(month) == 2) && atoi(day) > 28)
			{
				flag = 1;
				char invalid[] = "Invalid day.\n";
				length = strlen(invalid);
				sys_req(WRITE, DEFAULT_DEVICE, invalid, &length);
				memset(invalid, '\0', length);
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

	char exitMessage[] = "The date has been set.\n";
	int exitLength = strlen(exitMessage);
	sys_req(WRITE, DEFAULT_DEVICE, exitMessage, &exitLength);
	memset(exitMessage, '\0', exitLength);
	memset(spacer, '\0', spaceCount);

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

int quit()
{
	int flag = 0;

	char quitMsg[] = "Are you sure you want to shutdown? y/n\n";
	int quitMsgLength = strlen(quitMsg);
	sys_req(WRITE, DEFAULT_DEVICE, quitMsg, &quitMsgLength);

	char quitAns[] = "\0\0";
	int quitAnsLength = 1;
	sys_req(READ, DEFAULT_DEVICE, quitAns, &quitAnsLength);
	char answer = quitAns[0];

	if (answer == 'y' || answer == 'Y')
	{
		flag = 1;
	}
	else if (answer == 'n' || answer == 'N')
	{
		flag = 0;
	}
	else
	{
		char error[] = "Invalid input!\n";
		int errorLength = strlen(error);
		sys_req(WRITE, DEFAULT_DEVICE, error, &errorLength);
	}

	return flag;
}