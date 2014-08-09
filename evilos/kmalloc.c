#include <mm.h>
#include <kmalloc.h>


void *kmalloc(unsigned long size){
	void *addr=NULL;
	size=((size+4096)/4096)*4096;
	addr= mem_malloc(size);
	/*printf("size:%d addr:%x\n",size,addr);
	addr= mem_malloc(size);
	printf("size:%d addr:%x\n",size,addr);
	addr= mem_malloc(size);
	printf("size:%d addr:%x\n",size,addr);*/
	memset(addr,0,size);
	return addr;
}
void kfree(void *addr){
	mem_free(addr);
	//vesa_printf(0,300,"kfree %x",addr);
}
