#ifndef	_PAGE_H
#define _PAGE_H

#include <type.h>
#include <kprintf.h>

#define PAGE_DIR_ADDR			0x400000L      //页目录地址
#define PAGE_TABLE_ADDR			0x401000L      //页表地址
#define PAGE_DIR_SIZE			0x1000L        //页目录大小   4KB
#define PAGE_OFFSET             0xC0000000
#define PAGE_SIZE				0x1000

#define __pa(x)		((x) - PAGE_OFFSET)
#define __va(x)		((x) + PAGE_OFFSET)    

#define PAGE_P   1    
#define PAGE_RWR   0
#define PAGE_RWW   2
#define PAGE_USS   0
#define PAGE_USU   4

#define init_page() page_init()

void page_init();

void do_page_fault(enum KP_LEVEL kl,
              u32 ret_ip, u32 ss, u32 gs,
              u32 fs, u32 es, u32 ds, 
              u32 edi, u32 esi, u32 ebp,
              u32 esp, u32 ebx, u32 edx, 
              u32 ecx, u32 eax, u32 isr_nr, 
              u32 err, u32 eip, u32 cs, 
              u32 eflags,u32 old_esp, u32 old_ss);

#endif
