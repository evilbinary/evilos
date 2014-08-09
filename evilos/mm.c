#include <mm.h>
#include <multiboot.h>
//#include "utility.h"
#include "kmalloc.h"

static struct mem_block_t blocks[NR_BLOCKS];
static struct mem_block_t* block_head;
static struct mem_block_t* block_tail;
static unsigned long mem_nr;
static unsigned long mem_size;

extern multiboot_info_t *multiboot_info;

void mem_block_print(struct mem_block_t* head){
	struct mem_block_t* p=head;
	for(;p!=NULL;p=p->next){
		printf("type:%x base:%x size:%x\n",p->type,p->addr,p->size);	
	}
}
void mem_block_print_free(struct mem_block_t* head){
	struct mem_block_t* p=head;
	for(;p!=NULL;p=p->next){
		if(p->type==MEM_FREE)
			printf("type:%x base:%x size:%x\n",p->type,p->addr,p->size);	
	}
}
void mem_test(){
	char * p,*p1;
	cls();
	printf("all blocks\n");
	mem_block_print(block_head);
	printf("after malloc\n");
	p=(char *)mem_malloc(1024*1024);
	p1=(char *)mem_malloc(1024*1024);
	printf("malloc addr p:0x%x p1:0x%x\n",p,p1);
	//printf("free\n");
	//mem_block_print_free(block_head);
	printf("all blocks\n");
	mem_block_print(block_head);
	printf("free addr:0x%x\n",p);
	mem_free(p1);
	printf("all blocks\n");
	mem_block_print(block_head);
}

void mem_init(){
	multiboot_info_t *mbi;
	multiboot_memory_map_t *mmap;
	struct mem_block_t* p;
	block_head=NULL;
	mem_nr=0;
	mbi = multiboot_info;
    printf ("mmap_addr = 0x%x, mmap_length = 0x%x\n",(unsigned) mbi->mmap_addr, (unsigned) mbi->mmap_length);
    for (mmap = (multiboot_memory_map_t *) mbi->mmap_addr;
    (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
    mmap = (multiboot_memory_map_t *) ((unsigned long) mmap+ mmap->size + sizeof (mmap->size))){
    	
		blocks[mem_nr].addr=mmap->addr;
		blocks[mem_nr].size=mmap->len;
		blocks[mem_nr].type=mmap->type;
		blocks[mem_nr].next=NULL;
		if(block_head==NULL){
			block_head=&blocks[mem_nr];
			block_tail=block_head;
		}else{
			blocks[mem_nr].next=block_head;
			block_head=&blocks[mem_nr];
		}
		printf("addr:%x size:%d type:%d\n",blocks[mem_nr].addr,blocks[mem_nr].size=mmap->len,blocks[mem_nr].type=mmap->type);
		mem_nr++;
	}
	//mem_test();
	//for(;;);
}
void* mem_malloc(unsigned long size){
	struct mem_block_t* p=block_head;
	for(;p!=NULL;p=p->next){
		if((p->type==MEM_FREE)){
			if(p->size==size){
				p->type=MEM_USED;
				return p->addr;		
			}else if(p->size>size){
				//printf("cmp= base:%x size:%x cmpsize:%x\n",p->base,p->size,size);
				if(mem_nr<(NR_BLOCKS-1)){
					blocks[mem_nr].addr=(p->addr+p->size-size);
					//printf("p.a:%x t:%d b.a%x",p->addr+size,p->type,blocks[mem_nr].addr);
					blocks[mem_nr].size=size;
					blocks[mem_nr].type=MEM_USED;
					blocks[mem_nr].next=p->next;
					p->size-=size;
					p->next=&blocks[mem_nr];
					return blocks[mem_nr++].addr;
				}else{//fix me
					struct mem_block_t* t=(struct mem_block_t*)mem_malloc(sizeof(struct mem_block_t));
					t->addr=p->addr+p->size-size;
					t->size=size;
					t->type=MEM_USED;
					t->next=p->next;
					p->next=&t;
					mem_nr++;
					vesa_printf(0,0,"erro malloc memory.");
					return t->addr;
						
				}
			}
		}
	}
	return NULL;
}
void mem_free(void* addr){
	struct mem_block_t* p=block_head;
	for(;p!=NULL;p=p->next){
		if(p->addr==addr){
			p->type=MEM_FREE;
			//todo merging mem
			break;		
		}	
	}
}

unsigned long mem_get_size(void* addr){
	struct mem_block_t* p=block_head;
	for(;p!=NULL;p=p->next){
		if(p->addr==addr){
			return p->size;		
		}	
	}
}
