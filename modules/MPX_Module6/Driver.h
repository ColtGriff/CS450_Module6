// The file to house your r6 header.
#ifndef DRIVER_H
#define DRIVER_H

#define PIC_REG 0x20
#define PIC_EOI 0x20
#define PIC_MASK 0x21
#define IRQ_COM1 0x10

#define OPEN 1
#define CLOSE 0

#define ERROR_FULL -1
#define ERROR_EMPTY_QUEUE -2

#include "../mpx_supt.h"
#include "../R2/R2_Internal_Functions_And_Structures.h"

/*
* enum for the possible dcb states.
*/
typedef enum status_t
{
    STATUS_IDLE,    /* Port is idle */
    STATUS_READING, /* Port is reading */
    STATUS_WRITING, /* Port is writing */
} status_t;

/*!
+*  struct dcb represents a Device Control Block.
+*             A dcb should exist for each COM port, but you can just use COM1
+*  @param com_port the COM port. (You can omit this and just always use COM1)
+*  @param port_open whether the COM is open.
+*  @param e_flag whether the operation has completed (0 or 1).
+*  @param status the different operations (IDLE, READ, WRITE).
+*  @param buffer_ptr the buffer array to read into/write from.
+*  @param count_ptr how many characters to read/write.
+*  @param buffer_loc the current location we are reading/writing at.
+*  @param byte_count the number of bytes that have been read/written so far.
*/
typedef struct dcb
{
    // params here

    int com_port;
    int port_open; // 0 closed, 1 is open.
    int e_flag;    // 0 IO not finished, 1 IO finished
    int status;    // EXIT, IDLE, READ, WRITE, INVALID_OPERATION: 0, 1, 2, 3, 4 respectively
    char *buffer_ptr;
    int *count_ptr;
    int buffer_loc;
    int byte_count;

    // ring buffer parameters
    char ring[30];
    int read_count;
    int write_count;
} dcb;

/*!
+*  struct iod represents an I/O Desriptor.
+*  @param pcb_id the process that this iod is representing.
+*  @param op_code the operation that the process requested.
+*  @param com_port the COM port. (You can omit this and just always use COM1)
+*  @param buffer_ptr the buffer pointer to read to/write from.
+*  @param count_ptr the amount of characters to be read/written.
+*  @param next the next IOD in the IO queue after this one.
*/
typedef struct iod
{
    // params here
    PCB *pcb_id;
    int op_code;
    int com_port;
    char *buffer_ptr;
    int *count_ptr;
    struct iod *next;
} iod;

typedef struct iodQueue // simply an IO queue
{
    iod *head;
    iod *tail;
    int count_iods; // number of iods in a IO queue

} iodQueue;

// typedef struct ring_buffer
// {
//     char buffer[30];
//     char *read_ptr;
//     char *write_ptr;
// } ring_buffer;

/*!
+*  pic_mask() masks so only the desired PIC interrupt is enabled or disabled.
+*  @param enable 1 to enable or 0 to disable.
*/
void pic_mask(char enable);

/*!
+*  disable_interrupts() disables all interrupts to device.
*/
void disable_interrupts();

/*!
+*  enable_interrupts() enables interrupts to device.
*/
void enable_interrupts();

/*!
+*  com_open() Opens the communication port.
+*  @param e_flag event flag will be set to 1 if read/write
+*  @param baud_rate the desired baud rate
+*  @return Returns three possible error codes, or a 0 for successful operation.
*/
int com_open(int *e_flag, int baud_rate);

/*!
+*  com_close() Closes the communication port.
+*  @return error code if port was not open, or a 0 for successful operation
*/

int com_close(void);

/*!
+*  com_read() Reads the buffer from the port. Non-blocking.
+*  @param buf_ptr buffer in which the read characters will be stored.
+*  @param count_ptr the maximum number of bytes to read. After completion,
+*                       this will contain the number of characters read.
+*  @return Returns four possible error codes, or a 0 for successful operation.
*/
int com_read(char *buf_ptr, int *count_ptr);

/*!
+*  com_write() Writes the buffer to the port. Non-blocking.
+*  @param buf_ptr buffer in which the characters to write are stored.
+*  @param count_ptr the number of characters from the buffer to write.
+*  @return Returns four possible error codes, or a 0 for successful operation.
*/
int com_write(char *buf_ptr, int *count_ptr);

/*!
+*  serial_io() is the interrupt C routine for serial IO.
*/
void serial_io();

/*!
+*  serial_write() provides interrupt routine for writing IO.
*/
int serial_write();

/*!
+*  serial_read() provides interrupt routine for reading IO.
*/

int serial_read();

void serial_modem();

void serial_line();

int push(char input);

char pop();

void insert_IO_request(iod *iocb); // called in mpx_support.c, sys_call()

void remove_IO_request(PCB *pcb_id); // not yet called anywhere.

void allocateIOQueues();

dcb* DCB;

#endif