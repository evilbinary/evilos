#ifndef _MM_H
#define _MM_H

#define NR_BLOCKS             10240
#define NULL 0
#define MEM_FREE	1
#define MEM_USED	0x10
#define MEM_LOCK	3
#define MEM_SHARE	4

struct mem_block_t{
    unsigned long addr;
	unsigned long size;
	int type;
	struct mem_block_t * next;
};

void mem_init();
void*  mem_malloc(unsigned long size);
void mem_free(void* size);

#endif
