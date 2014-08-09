#include <malloc.h>
#include <kmalloc.h>
//implements 
void *malloc(unsigned long size){
	return kmalloc(size);
}

void free(void *addr){
	kfree(addr);
}
void *realloc (void *block,unsigned long size){
	void *p;
	//vesa_printf(0,300,"realloc %x",block);
	//for(;;);	
	size+=mem_get_size(block);
	p=malloc(size);
	memcpy(p,block);
	free(block);
	return p;
}
