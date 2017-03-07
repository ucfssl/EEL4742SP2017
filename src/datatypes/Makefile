ELF=datatypes.elf

SRC=datatypes.c

ifeq ($(ARCH),msp430)
CC		=	msp430-elf-gcc
LD		=	msp430-elf-gcc
RUN		=	msp430-elf-run
SIZE	=	msp430-elf-size
STRIP	=	msp430-elf-strip
else ifeq ($(ARCH),sparc)
CC		=	sparc-leon3-linux-uclibc-gcc
LD		=	sparc-leon3-linux-uclibc-gcc
RUN		=	qemu-sparc
SIZE	=	sparc-leon3-linux-uclibc-size
STRIP	=	sparc-leon3-linux-uclibc-strip
LDFLAGS	+=	-static
else
CC		=	gcc
LD		=	gcc
RUN		=	/bin/sh -c
SIZE	=	size
STRIP	=	strip
endif

include ../Makefile.in

# vim:set ts=4 tw=79 syntax=make et:
