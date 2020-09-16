
#include "../mpx_supt.h"
#include "R1commands.h"
// 0x00 - Seconds
// 0x02 - Minutes
// 0x04 - Hours
// 0x06 - Day of Week
// 0x07 - Day of Month
// 0x08 - Month
// 0x09 - Year


// Im assuming that we will have to set the get and set functions for date and time
// to take inputs from the commHand function

// GetYear
// Year register: 0x09 and 0x32

void help(){
	char mesg[2]="t";
	int mesg_size;
	
	mesg_size=2;
       
	sys_req( WRITE, DEFAULT_DEVICE, mesg, &mesg_size);
       
}
void version(){
       char module_version[10]="MPX R1 \n";
       char completion_date[30]=" completion date: 9/17/2020";
	int mesg_size =10;
	int date_size=30;
	sys_req( WRITE, DEFAULT_DEVICE, module_version, &mesg_size);
	sys_req( WRITE, DEFAULT_DEVICE, completion_date, &date_size);

	
}
/**

void getTime(){

	outb(0x70, 0x04);  // getting Hour value
	unsigned char tempHour = inb(0x71);

	outb(0x70, 0x02);  // getting Minute value
	unsigned char tempMin = inb(0x71);

	outb(0x70, 0x00);  // getting Second value
	unsigned char tempSec = inb(0x71);

	//serial_print("The current Time is \n"); // Gotta figure out BCD to Decimal
	sys_req(WRITE, DEFAULT_DEVICE,tempHour,tempMin,tempSec);
	

	return 0;

}

void setTime(int hh, int min, int sec){

	cli();

	outb(0x70, 0x04); // Hour
	//outb(0x71, byte); 

	outb(0x70, 0x02); // Minute
	//outb(0x71, byte);

	outb(0x70, 0x00); // Second
	//outb(0x71, byte);

	sti();

	return 0;
}

void getDate(){



	outb(0x70, 0x07);  // getting Day of month value
	unsigned char tempDay = inb(0x71);

	outb(0x70, 0x08);  // getting Month value
	unsigned char tempMonth = inb(0x71);

	outb(0x70, 0x09);  // getting second byte Year value
	unsigned char tempYear = inb(0x71);

	outb(0x70, 0x32);  // getting first byte Year value
	unsigned char tempYear = inb(0x71);

	return 0;
}

void setDate(int month, int day, int year){
	
	cli();

	outb(0x70, 0x07); // Setting day of month value
	//outb(0x71, byte); // need to figure out what the bytes are that are sent to change date

	outb(0x70, 0x08); // Setting month value
	//outb(0x71, byte);

	outb(0x70, 0x09); // Setting second byte year value
	//outb(0x71, byte);

	outb(0x70, 0x32); // Setting first byte year value
	//outb(0x71, byte);

	sti();

	return 0;
}

// Trying to figure out how to convert
//

unsigned char change_int_to_binary(int test){

	unsigned char val = (test/16*10)+(test%16)

	return val;
}

int change_binary_to_int(unsigned char test){

	int val = (test/10*16)+(test%10);
	
	return val;

}
**/
