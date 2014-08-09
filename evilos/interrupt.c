#include "interrupt.h"

unsigned long long *idt = ((unsigned long long *)IDT_ADDR);
unsigned long long *gdt = ((unsigned long long *)GDT_ADDR);

void isr_entry(int index, unsigned long long offset) {
	unsigned long long idt_entry = 0x00008e0000000000ULL |
			((unsigned long long)CODE_SEL<<16);
	idt_entry |= (offset<<32) & 0xffff000000000000ULL;
	idt_entry |= (offset) & 0xffff;
	idt[index] = idt_entry;
}

void idt_install(void) {
	unsigned int i = 0;
	struct DESCR {
		unsigned short length;
		unsigned long address;
	} __attribute__((packed)) idt_descr = {256*8-1, IDT_ADDR};
	for (i=0; i<VALID_ISR; ++i)
		isr_entry(i, (unsigned int)(isr[(i<<1)+1]));
	for (++i; i<256; ++i)
		isr_entry(i, (unsigned int)default_isr);
	__asm__ ("lidt	%0\n\t"::"m"(idt_descr));
}

void pic_install(void) {
	outb(0x11, 0x20);
	outb(0x11, 0xa0);
	outb(0x20, 0x21);
	outb(0x28, 0xa1);
	outb(0x04, 0x21);
	outb(0x02, 0xa1);
	outb(0x01, 0x21);
	outb(0x01, 0xa1);
	outb(0xff, 0x21);
	outb(0xff, 0xa1);
}
void gdt_install(){
    PDES pgdt;
	pgdt.limit=GDT_SIZE;
	pgdt.base=gdt;
    set_gdt_des(0,0,0,0x68);    //null
    set_gdt_des(1,ATTRIB_CE+ATTRIB_32+ATTRIB_LIMIT4K,0,0xffff);//code
    set_gdt_des(2,ATTRIB_DRW+ATTRIB_32+ATTRIB_LIMIT4K,0,0xffff);//data
    __asm__ ("lgdt %0\n\t"::"m"(pgdt));
}
