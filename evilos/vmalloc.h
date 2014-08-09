#ifndef _VMALLOC_H
#define _VMALLOC_H

#define VMALLOC_END 	0xfa000000UL
void *vmalloc(unsigned long size);
void vfree(void *addr);



#endif