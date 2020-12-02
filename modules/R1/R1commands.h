#ifndef R1COMMANDS_H
#define R1COMMANDS_H

void help();

void version();

void getTime();

void setTime();

void getDate();

void setDate();

unsigned int change_int_to_binary(int test);

int BCDtoChar(unsigned char test, char *buffer);

int quit();
#endif