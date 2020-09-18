#
# Makefile for MPX Modules

AS      = nasm
CC      = i386-elf-gcc
CFLAGS  = -Wall -Wextra -Werror -nostartfiles -nostdlib -nodefaultlibs -ffreestanding -g -c
LD      = i386-elf-ld
LDFLAGS = 
ASFLAGS = -f elf -g

# add new files as you create them.
# I'll start you off with just mpx_supt.o
OBJFILES =\
mpx_supt.o \
<<<<<<< HEAD
R1/commhand.o \
R1/R1commands.o

=======
R1/commhand.o\
R1/R1commands.o
>>>>>>> 59814169b90d1e8ae8a2a1ecb8c3ef576ba2f5f4
.c.s:
	$(CC) $(CFLAGS) -S -o $@ $<
.s.o:
	$(AS) $(ASFLAGS) -o $@ $<
.c.o:
	$(CC) $(CFLAGS) -I../include -o $@ $<

all: modules.o

modules.o: $(OBJFILES)
	$(LD) -r -o modules.o $(OBJFILES)

clean:
	rm modules.o $(OBJFILES)
