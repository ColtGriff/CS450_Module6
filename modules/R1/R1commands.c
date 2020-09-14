


// 0x00 - Seconds
// 0x02 - Minutes
// 0x04 - Hours
// 0x06 - Day of Week
// 0x07 - Day of Month
// 0x08 - Month
// 0x09 - Year


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

	outb(0x70, 0x08);  // getting Month value
	int tempMonth = inb(0x71);

	outb(0x70, 0x09);  // getting Year value
	int tempYear = inb(0x71);

	serial_print("The current Time is \n");

}

int setTime(){

	cli();

	



	sti();


}

int getDate(){

}

int setDate(){

}