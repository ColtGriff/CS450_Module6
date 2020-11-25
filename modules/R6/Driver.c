// The file to house your r6 code.

#include <Driver.h>
#include <core/serial.h>
#include <string.h>
#include "../mpx_supt.h"
#include <core/io.h>
#include "../utilities.h"

u32int IVT // the interrupt vector table, using this to save and restore the IVT in com_open/com_close

dcb *DCB; // the device representing the terminal.

void disable_interrupts()
{
    outb(IRQ_COM1 + 1, 0x00); //disable interrupts
}

void enable_interrupts()
{
    outb(IRQ_COM1 + 4, 0x0B); //enable interrupts, rts/dsr set
}

void pic_mask(char enable)
{
    // If enable, do a logical NOT on the IRQ for COM1.
    // Obtain the mask by inb(the PIC_MASK register).
    // outb (PIC MASK register, (logical AND the mask with the irq from step 1))

    if (enable == '1')
    {
        inb(PIC_MASK);
        outb(PIC_MASK, (PIC_MASK && (!IRQ_COM1)));
    }
}

int com_open(int *e_flag, int baud_rate)  // ME
{
    // Check the event flag is not null, the baud rate valid,and port is open.
    // Set the status of the device to open and idle.
    // Set the event flag of the device to the one passed in
    // Save interrupt vector
    // Disable your interrupts.
    // Set registers. Take a look at init_serial() in serial.c
    // PIC mask enable
    // Enable your interrupts.
    // Read a single byte to reset the port.

	// -101 for invalid (null) event flag pointer
	// -102 invalid baud rate divisor
	// -103 port already open

	if(e_flag == NULL){
		return (-101)
	}
	else if(baud_rate <= 0){
		return (-102)
	}
	else if(dcb.port_open == OPEN){
		return (-103)
	}
	else{
		dcb.status = IDLE;
		dcb.port_open = OPEN;
		dcb.e_flag = e_flag;
		IVT = idt_get_gate(0x24);
		disable_interrupts(); 
		// These are from init_serial()
		outb(device + 3, 0x80);          //set line control register
	  	outb(device + 0, 115200 / 9600); //set bsd least sig bit
	  	outb(device + 1, 0x00);          //brd most significant bit
	  	outb(device + 3, 0x03);          //lock divisor; 8bits, no parity, one stop
	  	outb(device + 2, 0xC7);          //enable fifo, clear, 14byte threshold
	  	// The values may need changed
	  	enable_interrupts();
	  	(void)inb(device);               //read bit to reset port


		return 0;
	}
}

int com_close(void)  // ME
{
    // Set the status of the device to closed
    // Disable pic mask
    // Disable interrupts

    // if no error return 0, otherwise return -201 - serial port not open
    // Restore the original saved interrupt vector

	if(dcb.port_open == CLOSED){
		return (-201);
	}
	else{
		dcb.port_open == CLOSED;
		pic_mask(0);
		disable_interrupts();
		idt_set_gate(0x24, IVT, 0x08, 0x8e); 

		return 0;
	}

}

int com_read(char *buf_ptr, int *count_ptr)  // ME
{
    // check port open, check valid pointer, check port is idle, etc.
    // set dcb vars
    // disable interrupts
    // read from ring buffer into the dcb buffer if there is anything
    // enable interrupts
    // enable input ready interrupts

	// if no error return 0, otherwise:
	// return -301 (port not open)
	// return -302 (invalid buffer address)
	// -303 (invalid count address or count value)
	// -304 (device busy)


}

int com_write(char *buf_ptr, int *count_ptr)  // ME
{
    // check port open, check valid pointer, check port is idle, etc.
    // set dcb vars
    // disable interrupts
    // write a single byte to the device.
    // enable interrupts
    // enable write interrupts interrupts

    // if no error return 0, otherwise:
    // -401 serial port not open
    // -402 invalid buffer address
    // -403 invalid count address or count value
    // -404 device busy
}

void serial_io()
{
    // check port open.
    // obtain interrupt type. Call appropriate second level handler
    // Check if the event has completed. If so call io scheduler.
    // outb(PIC register, PIC end of interrupt)

    if (DCB->port_open == OPEN)
    {
    }
}

void serial_write()
{
    // Ensure the dcb status is writing
    // If there are any more characters left in the buffer, print them
    // Othewise we are done printing
    // Update the dcb status. Disable output interrupts
}

void serial_read()
{
    // Ensure the dcb status is reading. If not, push to the ring buffer.
    // Read a character from the COM port & add it to the buffer.
    // If we reached a new line or the buffer size, we are done reading
    // Update the dcb status. Disable intput interrupts
}