//R4commands.c

#include <string.h>
#include "../mpx_supt.h"
#include <string.h>
#include <core/serial.h>
#include <core/io.h>
#include "../R2/R2_Internal_Functions_And_Structures.h"
#include "../R2/R2commands.h"
#include "../R3/R3commands.h"
#include "R4commands.h"
#include "../utilities.h"
#include "../R1/R1commands.h"

alarmList *alarms;

void alarmPCB()
{
	if (alarms->head == NULL && findPCB("Alarm")->runningStatus != -1)
	{
		blockPCB("Alarm");
	}
	else
	{
		iterateAlarms();
	}
}

void infinitePCB()
{
	createPCB("infinite", 'a', 1);
	PCB *new_pcb = findPCB("infinite");
	context *cp = (context *)(new_pcb->stackTop);
	memset(cp, 0, sizeof(context));
	cp->fs = 0x10;
	cp->gs = 0x10;
	cp->ds = 0x10;
	cp->es = 0x10;
	cp->cs = 0x8;
	cp->ebp = (u32int)(new_pcb->stack);
	cp->esp = (u32int)(new_pcb->stackTop);
	cp->eip = (u32int)infiniteFunc; // The function correlating to the process, ie. Proc1
	cp->eflags = 0x202;
}

void infiniteFunc()
{
	while (1)
	{

		printMessage("Infinite Process Executing.\n");

		sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
	}
}

void allocateAlarmQueue()
{
	alarms = sys_alloc_mem(sizeof(alarmList));
	alarms->count = NULL;
	alarms->head = NULL;
	alarms->tail = NULL;
}

alarm *allocateAlarms()
{
	alarm *newAlarm = (alarm *)sys_alloc_mem(sizeof(alarm));

	char name[20] = "newAlarm";
	strcpy(newAlarm->alarmName, name);

	newAlarm->alarmTime = 0;

	// Setting the alarms prev and next PCB
	newAlarm->nextAlarm = NULL;
	newAlarm->prevAlarm = NULL;

	return newAlarm;
}

alarmList *getAlarms()
{
	return alarms;
}

void addAlarm()
{

	unblockPCB("Alarm");

	printMessage("Please enter a name for the alarm you want to create.\n\n");

	alarm *Alarm_to_insert = allocateAlarms();

	int nameLength = strlen(Alarm_to_insert->alarmName);
	sys_req(READ, DEFAULT_DEVICE, Alarm_to_insert->alarmName, &nameLength);

	printMessage("Please type the desired hours. I.E.: hh.\n");

	char hour[4] = "\0\0\n\0";

	int flag = 0;

	do
	{
		int hourLength = strlen(hour);
		sys_req(READ, DEFAULT_DEVICE, hour, &hourLength);
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
		int minuteLength = strlen(minute);
		sys_req(READ, DEFAULT_DEVICE, minute, &minuteLength);
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
		int secondLength = strlen(second);
		sys_req(READ, DEFAULT_DEVICE, second, &secondLength);
		if (atoi(second) < 60 && atoi(second) >= 0)
		{

			printMessage("\n");
			flag = 0;
		}
		else
		{
			printMessage("\nInvalid seconds.\n");
			flag = 1;
		}
	} while (flag == 1);

	// Storing time in the alarm to insert
	Alarm_to_insert->alarmTime = convertTime(hour, minute, second);

	// Inserting the alarm
	if (getAlarms()->head != NULL)
	{
		getAlarms()->tail->nextAlarm = Alarm_to_insert;
		Alarm_to_insert->prevAlarm = getAlarms()->tail;
		getAlarms()->tail = Alarm_to_insert;
		getAlarms()->count++;
	}
	else
	{
		getAlarms()->head = Alarm_to_insert;
		getAlarms()->tail = Alarm_to_insert;
		getAlarms()->count++;
	}
}

int convertTime(char *hours, char *minutes, char *seconds)
{
	int result = (atoi(hours) * 3600);
	result += (atoi(minutes) * 60);
	result += (atoi(seconds));

	return result;
}

void iterateAlarms()
{
	char hours[4] = "\0\0\0\0";
	outb(0x70, 0x04); // getting current Hour value
	BCDtoChar(inb(0x71), hours);

	char minutes[4] = "\0\0\0\0";
	outb(0x70, 0x02); // getting current Minute value
	BCDtoChar(inb(0x71), minutes);

	char seconds[4] = "\0\0\0\0";
	outb(0x70, 0x00); // getting current Minute value
	BCDtoChar(inb(0x71), seconds);

	int currentTime = convertTime(hours, minutes, seconds);

	alarm *tempAlarm = getAlarms()->head;

	while (tempAlarm != NULL)
	{
		if (currentTime >= getAlarms()->head->alarmTime)
		{
			// do something for alarm.
			printMessage(getAlarms()->head->alarmName);
			getAlarms()->head = getAlarms()->head->nextAlarm;
		}
		else if (currentTime >= getAlarms()->tail->alarmTime)
		{
			printMessage(getAlarms()->tail->alarmName);
			getAlarms()->tail = getAlarms()->tail->prevAlarm;
		}
		else if (currentTime >= tempAlarm->alarmTime)
		{
			printMessage(tempAlarm->alarmName);
			tempAlarm->prevAlarm->nextAlarm = tempAlarm->nextAlarm;
			tempAlarm->nextAlarm->prevAlarm = tempAlarm->prevAlarm;
			tempAlarm->nextAlarm = NULL;
			tempAlarm->prevAlarm = NULL;
		}
		else
		{
			// iterates if not time
			tempAlarm = tempAlarm->nextAlarm;
		}
	}
}