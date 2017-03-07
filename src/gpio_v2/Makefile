ELF=gpio.elf

SRC=gpio.c
MCU=msp430g2553

CC		=	msp430-elf-gcc
LD		=	msp430-elf-gcc
AS		=	msp430-elf-gcc
SIZE	=	msp430-elf-size
RUN		=	msp430-elf-run
STRIP	=	msp430-elf-strip

RUNOPTS	=	--memory-region 0x200,0x200 --trace=on

include ../Makefile.in
