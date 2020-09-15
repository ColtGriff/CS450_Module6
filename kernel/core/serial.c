/*
  ----- serial.c -----

  Description..: Contains methods and variables used for
    serial input and output.
*/

#include <stdint.h>
#include <string.h>

#include <core/io.h>
#include <core/serial.h>

#define NO_ERROR 0

// Active devices used for serial I/O
int serial_port_out = 0;
int serial_port_in = 0;

/*
  Procedure..: init_serial
  Description..: Initializes devices for user interaction, logging, ...
*/
int init_serial(int device)
{
  outb(device + 1, 0x00);          //disable interrupts
  outb(device + 3, 0x80);          //set line control register
  outb(device + 0, 115200 / 9600); //set bsd least sig bit
  outb(device + 1, 0x00);          //brd most significant bit
  outb(device + 3, 0x03);          //lock divisor; 8bits, no parity, one stop
  outb(device + 2, 0xC7);          //enable fifo, clear, 14byte threshold
  outb(device + 4, 0x0B);          //enable interrupts, rts/dsr set
  (void)inb(device);               //read bit to reset port
  return NO_ERROR;
}

/*
  Procedure..: serial_println
  Description..: Writes a message to the active serial output device.
    Appends a newline character.
*/
int serial_println(const char *msg)
{
  int i;
  for (i = 0; *(i + msg) != '\0'; i++)
  {
    outb(serial_port_out, *(i + msg));
  }
  outb(serial_port_out, '\r');
  outb(serial_port_out, '\n');
  return NO_ERROR;
}

/*
  Procedure..: serial_print
  Description..: Writes a message to the active serial output device.
*/
int serial_print(const char *msg)
{
  int i;
  for (i = 0; *(i + msg) != '\0'; i++)
  {
    outb(serial_port_out, *(i + msg));
  }
  if (*msg == '\r')
    outb(serial_port_out, '\n');
  return NO_ERROR;
}

/*int (*student_read)(char *buffer, int *count);
  Procedure..: set_serial_out
  Description..: Sets serial_port_out to the given device address.
    All serial output, such as that from serial_println, will be
    directed to this device.
*/
int set_serial_out(int device)
{
  serial_port_out = device;
  return NO_ERROR;
}

/*
  Procedure..: set_serial_in
  Description..: Sets serial_port_in to the given device address.
    All serial input, such as console input via a virtual machine,
    QEMU/Bochs/etc, will be directed to this device.
*/
int set_serial_in(int device)
{
  serial_port_in = device;
  return NO_ERROR;
}

int *polling(char *buffer, int *count)
{
  // insert your code to gather keyboard input via the technique of polling.

  char keyboard_character;

  int cursor = 0;

  char log[] = {'\0', '\0', '\0', '\0'};

  while (1)
  {

    if (inb(COM1 + 5) & 1)
    {                                 // is there input char?
      keyboard_character = inb(COM1); //read the char from COM1

      if (keyboard_character == '\n' || keyboard_character == '\r')
      { // HANDLEING THE CARRIAGE RETURN AND NEW LINE CHARACTERS

        buffer[cursor] = '\0';
        break;
      }
      else if ((keyboard_character == 127 || keyboard_character == 8) && cursor > 0)
      { // HANDELING THE BACKSPACE CHARACTER

        //serial_println("Handleing backspace character.");

        buffer[cursor - 1] = ' ';
        serial_print("\b \b");
        cursor--;

        int temp_cursor = cursor;

        while (buffer[temp_cursor + 1] != '\0')
        {

          //serial_println("Entered backspace while loop.");

          buffer[temp_cursor] = buffer[temp_cursor + 1];
          buffer[temp_cursor + 1] = ' ';
          temp_cursor++;
        }
      }
      else if (keyboard_character == '~' && cursor < 99)
      { //HANDLING THE DELETE KEY

        //serial_println("Handling the delete key");

        buffer[cursor + 1] = ' ';

        int temp_cursor = cursor + 1;

        while (buffer[temp_cursor + 1] != '\0')
        {

          //serial_println("Entered delete while loop.");

          buffer[temp_cursor] = buffer[temp_cursor + 1];
          buffer[temp_cursor + 1] = ' ';
          temp_cursor++;
        }
      }
      else if (keyboard_character == '\033')
      { // HANDLEING FIRST CHARACTER FOR ARROW KEYS

        log[0] = keyboard_character;
      }
      else if (keyboard_character == '[' && log[0] == '\033')
      { // HANDLEING SECOND CHARACTER FOR ARROW KEYS

        log[1] = keyboard_character;
      }
      else if (log[0] == '\033' && log[1] == '[' && log[2] == '\0')
      { // HANDLEING LAST CHARACTER FOR ARROW KEYS

        log[2] = keyboard_character;

        //serial_println("^");

        if (log[2] == 'A')
        { //Up arrow
          //Call a history function from the commhand or do nothing
        }
        else if (log[2] == 'B')
        { //Down arrow
          //Call a history command from the commhand or do nothing
        }
        else if (log[2] == 'C' && cursor != 99)
        { //Right arrow

          serial_print("\033[C");
          cursor++;
        }
        else if (log[2] == 'D' && cursor != 0)
        { //Left arrow

          serial_print("\033[D");
          cursor--;
        }

        memset(log, '\0', 4);
      }
      else
      {

        //serial_println("Adding character to buffer");

        buffer[cursor] = keyboard_character;
        outb(serial_port_out, keyboard_character);
        cursor++;
      }
    }
  }

  *count = cursor; // buffer count

  // You must validat each key and handle special keys such as delete, back space, and
  // arrow keys

  // remove the following line after implementing your module, this is present
  // just to allow the program to compile before R1 is complete
  //strlen(buffer);

  return count;
}
