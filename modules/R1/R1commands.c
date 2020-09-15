

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

int help(){

}

int version(){
	
}

int getTime(){

	outb(0x70, 0x04);  // getting Hour value
	unsigned char tempHour = inb(0x71);

	outb(0x70, 0x02);  // getting Minute value
	unsigned char tempMin = inb(0x71);

	outb(0x70, 0x00);  // getting Second value
	unsigned char tempSec = inb(0x71);

	serial_print("The current Time is \n"); // Gotta figure out BCD to Decimal

	return 0;

}

int setTime(int time){

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

int getDate(){



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

int setDate(){
	
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

unsigned char intToBCD(int test){

	unsigned char val = (test/16*10)+(test%16)

	return val;
}

int BCDtoInt(unsigned char test){

	int val = (test/10*16)+(test%10);
	
	return val;
}