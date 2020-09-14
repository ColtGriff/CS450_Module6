


// 0x00 - Seconds
// 0x02 - Minutes
// 0x04 - Hours
// 0x06 - Day of Week
// 0x07 - Day of Month
// 0x08 - Month
// 0x09 - Year


// Im assuming that we will have to set the get and set functions for date and time
// to take inputs from the commHand function
//

int help(){

}

int version(){
	
}

int getTime(){

	outb(0x70, 0x04);  // getting Hour value
	int tempHour = inb(0x71);

	outb(0x70, 0x02);  // getting Minute value
	int tempMin = inb(0x71);

	outb(0x70, 0x00);  // getting Second value
	int tempSec = inb(0x71);

	

	serial_print("The current Time is \n"); // Gotta figure out BCD to Decimal

}

int setTime(){

	cli();

	outb(0x70, 0x04); // Hour
	//outb(0x71, byte); // Need to figure out what the bytes are that are sent to change time

	outb(0x70, 0x02); // Minute
	//outb(0x71, byte);

	outb(0x70, 0x00); // Second
	//outb(0x71, byte);

	sti();


}

int getDate(){

	outb(0x70, 0x07);  // getting Day value
	int tempDay = inb(0x71);

	outb(0x70, 0x08);  // getting Month value
	int tempMonth = inb(0x71);

	outb(0x70, 0x09);  // getting Year value
	int tempYear = inb(0x71);

	outb

}

int setDate(){
	
	cli();

	outb(0x70, 0x07); // Setting day value
	//outb(0x71, byte); // need to figure out what the bytes are that are sent to change date

	outb(0x70, 0x08); // Setting month value
	//outb(0x71, byte);

	outb(0x70, 0x09); // Setting year value
	//outb(0x71, byte);

	sti();
}