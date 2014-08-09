##########################################################################################
# Project: EvilBinary OS
# Makefile Evilbinary 小E 
# Mail:rootntsd@gmail.com
##########################################################################################

CC = gcc -m32
AS =as --32
LD =ld -m elf_i386
INCLUDES = -I./include -I../include -I.
LIBS = -L./lib -L../lib
CFLAGS =-std=gnu99 -Wall -w -O -fstrength-reduce -fomit-frame-pointer\
	-finline-functions -fno-builtin  -ffreestanding -nostdinc #-Wextra -fno-stack-protector #
ASFLAGS =-I./include -I../include -I.
LDFLAGS =-s -Ttext 100000 #-T kernel.ld
AR =ar
ARFLAGS= rcs
RM =rm -f
SRCS = $(wildcard *.c)
OBJS = signal.o  sscanf.o  stat.o  stdio.o  stdlib.o  timeb.o  utsname.o \
	gettimeofday.o time.o ctype.o errno.o string.o vsprintf.o getopt.o strings.o
TARGET=libc.a 

.PHONY : all clean

all:$(TARGET)
$(TARGET):$(OBJS)
	$(AR) $(ARFLAGS) $@ $?
.s.o:
	$(AS) $(ASFLAGS) -o $@ $<
.c.o:
	$(CC) $(CFLAGS) -c -o $@ $< $(INCLUDES) $(LIBS)
.S.o:
	$(CC) $(CFLAGS) -c -o $@ $< $(INCLUDES) $(LIBS)
clean:
	$(RM) $(OBJS) $(TARGET)

