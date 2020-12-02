// The file to house your r6 code.

#include "Driver.h"
#include <core/serial.h>
#include <string.h>
#include "../mpx_supt.h"
#include <core/io.h>
#include "../utilities.h"

u32int IVT; // the interrupt vector table, using this to save and restore the IVT in com_open/com_close
int mask;
dcb *DCB; // the device representing the terminal. do sys_alloc_mem(sizeof(dcb));

iodQueue *active;  // IO queue for active requests
iodQueue *waiting; // queue for pending I/O requests

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

int com_open(int *e_flag, int baud_rate)
{
    // Check the event flag is not null, the baud rate valid,and port is not currently open. - DONE
    // Set the status of the device to open and idle. - DONE
    // Set the event flag of the device to the one passed in - DONE
    // Save interrupt vector - DONE
    // Disable your interrupts. - DONE
    // Set registers. Take a look at init_serial() in serial.c - DONE
    // PIC mask enable - DONE
    // Enable your interrupts. - DONE
    // Read a single byte to reset the port. - DONE
    if (e_flag == NULL)
    {
        return (-101); // invalid event flag pointer
    }
    else if (baud_rate <= 0)
    {
        return (-102); // invalid baud rate divisor
    }
    else if (DCB->port_open == 1)
    {
        return (-103); // port is already open
    }
    else
    {

        DCB->port_open = 1; // setting device open
        DCB->status = 1;    // setting status idle
        DCB->e_flag = (int)&(e_flag);

        // initialize ring buffer parameters here
        int read_count = 0;
        int write_count = 0;
        char ring[30];

        long baudR = 115200 / (long)baud_rate;

        // com1:
        // base +1 : interrupt enable (if bit 7 of line control register is 1 base and base+1 are LSB and MSB respectively of baud rate divisor)
        // base +2 : interrupt ID reg
        // base +3 : line control reg
        // base +4 : Modem control reg
        // base +5 : Line status reg
        // base +6 : modem status reg
        cli();
        outb(COM1 + 3, 0x80);  //set line control register
        outb(COM1 + 0, baudR); //set bsd least sig bit
        outb(COM1 + 1, 0x00);  //brd most significant bit ------------ Not too sure about how this works, from serial.c
        outb(COM1 + 3, 0x03);  //lock divisor; 8bits, no parity, one stop // 0000 0011

        // Enable the appropriate level in the PIC mask register
        mask = inb(PIC_MASK);
        mask = mask & ~0x10;
        outb(PIC_MASK, mask);
        sti();

        outb(COM1 + 4, 0x08); // Enable overall serial port interrupts

        // Enable input ready interupts only by storing the value 0x01 in the interrupt enable reg
        outb(COM1 + 1, 0x01); // storing the value 0x01 in the interrupt enable reg

        (void)inb(COM1); //read bit to reset port

        return 0; // no error
    }
}

int com_close(void)
{
    // Set the status of the device to closed - DONE(?)
    // Disable pic mask - DONE
    // Disable interrupts - DONE

    if (DCB->port_open != 1)
    {
        return (-201); // serial port not open
    }
    else
    {
        DCB->port_open = 0; // Clear open indicator in the DCB

        // Disable the appropriate level in the PIC mask reg
        cli();
        mask = inb(PIC_MASK); // 0x80 1000 0000
        mask = mask & ~0xEF;  // 0001 0000 -> ' -> & -> 1110 1111 = 0xEF
        outb(PIC_MASK, mask);
        sti();

        outb(COM1 + 3, 0x00); // Disable all interrupts in the ACC by loading zero values to the modem status reg

        outb(COM1 + 1, 0x00); // (prev comment continuation) and the interrupt enable reg

        outb(PIC_REG, 0x20); // passing the EOI code to the PIC_REG

        return 0; // no error
    }
}

int com_read(char *buf_ptr, int *count_ptr)
{
    // check port open, check valid pointer, check port is idle, etc. - DONE
    // set dcb vars - DONE
    // disable interrupts - DONE
    // read from ring buffer into the dcb buffer if there is anything - NOT DONE
    // enable interrupts - DONE
    // enable input ready interrupts - DONE

    int byte_count = 0;

    if (DCB->port_open != 1)
    {
        return (-301); // Port not open
    }
    if (buf_ptr == NULL)
    {
        return (-302); // invalid buffer address
    }
    if (count_ptr == NULL)
    {
        return (-303); // invalid count address(?) or value
    }
    if (DCB->status != 1)
    {
        return (-304); // device busy
    }
    else
    {

        // initialize the input buffer variables
        DCB->buffer_ptr = buf_ptr;
        DCB->count_ptr = count_ptr;

        DCB->status = 2; // set status to reading

        DCB->e_flag = 0; // Clear callers event flag

        cli();
        // Copy characters from ring buffer to requestor's bufer, until the ring buffer is emptied, the requested amount has been reached, or a CR (enter) code has been found
        // the copied characters should, of course be removed from the ring buffer.  Either input interrupts or all interrupts should be disabled during the copying

        // requestors buffer is buf_ptr
        while ((byte_count <= (int)&count_ptr && byte_count < 30) || ((inb(COM1) == '\n') || (inb(COM1) == '\r')))
        {
        }
        sti();

        // Enable input ready interupts only by storing the value 0x01 in the interrupt enable reg
        outb(COM1 + 1, 0x01); // storing the value 0x01 in the interrupt enable reg

        if (byte_count < (int)&count_ptr)
        {             // If more characters are needed, return. If the block is complete, continue with step 7
            return 0; // Gotta figure out what this return is
        }
        else
        {                    // step 7
            DCB->status = 1; // reset DCB status to idle
            DCB->e_flag = 1; // set event flag
            // return the actual count to the requestor's variable
            return byte_count;
        }
    }
    return 0;
}

int com_write(char *buf_ptr, int *count_ptr)
{
    // check port open, check valid pointer, check port is idle, etc. - DONE
    // set dcb vars - DONE
    // disable interrupts - DONE
    // write a single byte to the device. - DONE
    // enable interrupts - DONE
    // enable write interrupts - DONE

    int intReg;

    if (DCB->port_open != 1)
    {
        return (-401); // serial port not open
    }
    if (buf_ptr == NULL)
    {
        return (-402); // invalid buffer address
    }
    if (count_ptr == NULL)
    {
        return (-403); // invalid count address or count value
    }
    if (DCB->status != 1)
    {
        return (-404); // device busy
    }
    else
    {
        //set dcb vars
        DCB->buffer_ptr = buf_ptr;
        DCB->count_ptr = count_ptr;
        DCB->status = 3; // setting status to writing
        DCB->e_flag = 0;

        cli();
        outb(COM1, DCB->buffer_ptr); // get first character from requestors buffer and store it in the output reg

        intReg = inb(COM1 + 1); // enable write interrupts by setting bit 1 of the interrupt enable register.
        intReg = intReg | 0x02; // This must be done by setting the register to the logical or of its previous contents and 0x02
        outb(COM1 + 1, intReg); // THESE MAY NEED TO BE BEFORE THE OUTB
        sti();

        return 0; // no error
    }
}

void serial_io()
{
    // check port open.
    // obtain interrupt type. Call appropriate second level handler
    // Check if the event has completed. If so call io scheduler.
    // outb(PIC register, PIC end of interrupt)

    if (DCB->port_open == OPEN)
    {
        if (inb(COM1 + 2) & 0b0) // Interrupt was caused by the COM1 serial port
        {
            if (inb(COM1 + 2) & 0b000) // Modem Status Interrupt
            {
                serial_modem();
            }
            else if (inb(COM1 + 2) & 0b010) // Output Interrupt
            {
                serial_write();
            }
            else if (inb(COM1 + 2) & 0b100) // Input Interrupt
            {
                serial_read();
            }
            else if (inb(COM1 + 2) & 0b110) // Line Status Interrupt
            {
                serial_line();
            }

            if (DCB->e_flag == 1) // e_flag ==1 : IO is completed. Else, IO is not completed yet
            {
                io_scheduler();
            }
            outb(PIC_REG, PIC_EOI); // send EOI to the PIC command register.
        }
    }
}

int serial_write()
{
    // Ensure the dcb status is writing
    // If there are any more characters left in the buffer, print them
    // Othewise we are done printing
    // Update the dcb status. Disable output interrupts

    if (DCB->status == WRITE)
    {
        if ((int)&(DCB->count_ptr) > 0)
        {
            // if count has not been exhausted, get the next character from the requestor's output buffer and store it in the output register.
            // return without signaling completion.
            outb(COM1, (DCB->buffer_ptr + DCB->buffer_loc));
            DCB->buffer_loc++;
            DCB->count_ptr--;
            DCB->byte_count++;
            return 0;
        }
        else
        {
            DCB->status = IDLE;
            DCB->e_flag = 1;
            outb(COM1 + 1, (COM1 + 1) & 0b01);
            return DCB->byte_count;
        }
    }
    return 0;
}

int serial_read()
{
    // Ensure the dcb status is reading. If not, push to the ring buffer.
    // Read a character from the COM port & add it to the buffer.
    // If we reached a new line or the buffer size, we are done reading
    // Update the dcb status. Disable intput interrupts
    char input = inb(COM1);
    char *temp = &input;
    if (DCB->status == READ)
    {
        strcpy((DCB->buffer_ptr + DCB->buffer_loc), temp);
        if ((int)&(DCB->count_ptr) > 0 && input != '\n')
        {
            return 0;
        }
        else
        {
            DCB->status = IDLE;
            DCB->e_flag = 1;
            DCB->byte_count++;
            return DCB->byte_count;
        }
    }
    else
    {
        /*
        * push to the ring buffer
        * if buffer is full, discard the character.
        * return to first level anyway and do not signal completion.
        */
        if (push(input) == ERROR_FULL)
        {
            input = '\0';
            return ERROR_FULL;
        }

        return 0;
    }
}

void serial_modem()
{
    // read the modem status register and return to first level handler.
    inb(COM1 + 6);
}

void serial_line()
{
    // read a value from the Line Status Register and return to first level handler.
    inb(COM1 + 5);
}

int push(char input)
{
    if (DCB->ring[(DCB->write_count + 1) % 30] == DCB->ring[DCB->read_count])
    {
        return ERROR_FULL; // ring buffer is full.
    }
    else
    {
        DCB->ring[DCB->write_count] = input;
        DCB->write_count = (DCB->write_count + 1) % 30;
        return 0;
    }
}

char pop()
{
    char result = DCB->ring[DCB->read_count];
    DCB->read_count = (DCB->read_count + 1) % 30;
    return result;
}