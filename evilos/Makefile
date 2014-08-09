##########################################################################################
# Project: EvilBinary OS
# Makefile Evilbinary 小E
# Mail:rootntsd@gmail.com
##########################################################################################

CC = gcc -m32
AS =as --32
LD =ld -m elf_i386
INCLUDES = -I./include -I../include -I. -I./gui -I./app -I./lib -I./lib/include
LIBS = -L./lib -L../lib -lc
CFLAGS =-std=gnu99 -Wall -w -O -fstrength-reduce -fomit-frame-pointer\
	-finline-functions -fno-builtin  -ffreestanding -nostdinc #-Wextra -fno-stack-protector #
ASFLAGS =-I./include -I../include -I.
LDFLAGS =$(LIBS) -s -Ttext 100000 -M -nostdlib> kernel.map  #-T eeos.ld
AR =ar
ARFLAGS= -rcs
RM =rm -f
GUI-P=gui/gamemachine.o\
	gui/gmi.o\
	gui/lowmath.o\
	gui/quickheap.o\
	gui/quicktable.o\
	gui/sortable.o\
	gui/v4p.o\
	gui/v4pi.o\
	gui/v4pserial.o\
	gui/editor.o
GUI=gui/canvas.o\
	gui/box.o\
	gui/display.o

ARS	=libc.a
APP =
SRCS = $(wildcard *.c gui/*.c)
OBJS = boot.o kernel.o init.o isr.o exceptions.o kb.o kprintf.o   \
	screen.o task.o timer.o libcc.o page.o mm.o vesa.o vbe.o \
	mouse.o sem.o vmalloc.o kmalloc.o malloc.o eeos_pipe.o	\
	vfs.o	syscall.o interrupt.o\
	$(APP) $(GUI)
TARGET=kernel

.PHONY : all clean grub

all:$(TARGET) copy2image
$(TARGET):$(OBJS)
	$(LD) $(OBJS) -o $@  $(LDFLAGS)
.s.o:
	$(AS) $(ASFLAGS) -o $@ $<
.c.o:
	$(CC) $(CFLAGS) -c -o $@ $< $(INCLUDES) $(LIBS)
.S.o:
	$(CC) $(CFLAGS) -c -o $@ $< $(INCLUDES) $(LIBS)
clean:
	$(RM) $(OBJS) $(TARGET)
copy2image:kernel
	sudo mount `pwd`/boot.img /mnt/floppy -o loop
	sudo cp kernel /mnt/floppy/boot/
	sudo cp menu.lst /mnt/floppy/boot/grub/
	sudo umount /mnt/floppy
sp:
	sudo mount `pwd`/boot.img /mnt/floppy -o loop
	sudo cp grub-0.97/stage1/stage1 /mnt/floppy/boot/grub/
	sudo cp grub-0.97/stage2/stage2 /mnt/floppy/boot/grub/
	sudo umount /mnt/floppy
grub:
	dd if=/dev/zero bs=512 count=2880 of=floppy.img
	losetup /dev/loop7 floppy.img
	mkfs.ext2 /dev/loop7
	sudo mount -o loop /dev/loop7 /mnt/floppy
	mkdir -p /mnt/floppy/boot/grub
	sudo cp stage1 /mnt/floppy/boot/grub
	sudo cp stage2 /mnt/floppy/boot/grub
	sudo cp menu.lst /mnt/floppy/boot/grub
	grub --device-map=/dev/null
	device (fd0) /dev/loop7
	root (fd0)
	setup (fd0)
	quit
	sudo umount /mnt/floppy
	losetup -d /dev/loop7
	file floppy.img
g:
	cd grub-0.97;make;
au:
	cd grub-0.97/util/;
	losetup /dev/loop7 boot.img;
	mount /dev/loop7 /mnt/floppy/;
	grub-install  --force-lba --root-directory=/mnt/floppy/boot/grub/ /dev/loop7
	umount /mnt/floppy;
	losetup -d /dev/loop7;
run:
	bochs -q -f bochsrc
