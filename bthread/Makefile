# GNU toolchain parameters
AR		= arm-none-eabi-ar
AS		= arm-none-eabi-as
CC		= arm-none-eabi-gcc
CPP		= arm-none-eabi-g++
LD		= arm-none-eabi-gcc
OBJCOPY	= arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
RANLIB	= arm-none-eabi-ranlib
MARCH	= armv7-a
INCLUDE =
ARFLAG	=  crs
ASFLAG	= -march=$(MARCH) -g
CCFLAG	= -Wall -pipe -O0 -nostdlib -fno-builtin -march=$(MARCH) $(INCLUDE) -c
CPPFLAG	= $(CCFLAG)
LDFLAG	= -T link.lds -lm -lgcc
INC		= -I.
LIB		=
LINK	=
BOOTOBJ	= \
	boot.o \
	stdio.o \
	string.o \
	isr.o \
	vectors.o\
	bthread.o

DRIVERS	= \
	uart.o \
	vic.o
#	timer.o \
#	tube.o

TEST_TARGET = Test_prog
TEST_OBJ	= $(TEST_TARGET)/test.o

test : 	$(TEST_OBJ) $(BOOTOBJ) $(DRIVERS)
		$(LD) $(TEST_OBJ) $(BOOTOBJ) $(DRIVERS) $(LDFLAG) -o test.elf
		$(OBJCOPY) -O binary test.elf test.bin
		$(OBJDUMP) -xDS test.elf > test.txt

%.o: %.s
	$(AS) $< $(ASFLAG) $(INC) -o $@

%.o: %.c
	$(CC) $< $(CPPFLAG) $(INC) $(LIB) -c -o $@

%.o: %.cpp
	$(CPP) $< $(CPPFLAG) $(INC) $(LIB) -c -o $@

.PHONY:
clean:
	rm -rf *~ *.o *.a *.bin *.elf

all:
	@make clean
	@make test

