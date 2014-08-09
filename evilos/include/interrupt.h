#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <isr.h>
#include <asm.h>
#include <kernel.h>

void isr_entry(int index, unsigned long long offset);
void idt_install(void);
void pic_install(void);
void gdt_install(void);

#endif // INTERRUPT_H
