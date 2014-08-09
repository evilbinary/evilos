#include <task.h>
#include <kernel.h>
#include <asm.h>
#include "page.h"
#include "type.h"

void page_init(){
	u32 mem_size;
    u32 page_table_num=mem_size/(4096*1024);
    u32 page_residue_num=(mem_size%(4096*1024))/4096;
    u32 index;
    unsigned long cr0;

	if(page_residue_num) page_table_num+=1;
	page_map_all();

    __asm__("movl %%cr0, %0" : "=r" (cr0));  
    __asm__("movl %%eax, %%cr3": :"a"(PAGE_DIR_ADDR));
    cr0 |= 0x80000000;
    __asm__("movl %%eax, %%cr0": :"a"(cr0));
  	
  
}

void page_map_all() {
	u32 i,k;
	u32 addr=0;
	unsigned long *pdir = (unsigned long *) PAGE_DIR_ADDR;
	for(k=0;k<1024;k++) {
		unsigned long *page_table = (unsigned long *) (PAGE_TABLE_ADDR + PAGE_SIZE + (k*PAGE_SIZE));
		pdir[k] = (unsigned long*)page_table;
		pdir[k] = pdir[k] | (PAGE_P|PAGE_USU|PAGE_RWW);
		for(i=0; i<1024; i++){
			page_table[i] = addr | (PAGE_P|PAGE_USU|PAGE_RWW);
			addr = addr + PAGE_SIZE; // 4096 = 4kb
		};
	
	}
}
void * get_physaddr(void * virtualaddr){
    unsigned long pdindex = (unsigned long)virtualaddr >> 22;
    unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;
 
    unsigned long * pd = (unsigned long *)PAGE_DIR_ADDR;
    // Here you need to check whether the PD entry is present.
 
    unsigned long * pt = ((unsigned long *)PAGE_TABLE_ADDR) + (0x400 * pdindex);
    // Here you need to check whether the PT entry is present.
 
    return (void *)((pt[ptindex] & ~0xFFF) + ((unsigned long)virtualaddr & 0xFFF));
}
void map_page(void * physaddr, void * virtualaddr, unsigned int flags){
    // Make sure that both addresses are page-aligned.
 	unsigned long pdindex = (unsigned long)virtualaddr >> 22;
    unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;
    
    unsigned long * pd = (unsigned long *)PAGE_DIR_ADDR;
    // Here you need to check whether the PD entry is present.
    // When it is not present, you need to create a new empty PT and
    // adjust the PDE accordingly.
 
    unsigned long * pt = ((unsigned long *)PAGE_TABLE_ADDR) + (0x400 * pdindex);
    // Here you need to check whether the PT entry is present.
    // When it is, then there is already a mapping present. What do you do now?
 
    pt[ptindex] = ((unsigned long)physaddr) | (flags & 0xFFF) | 0x01; // Present
 
    // Now you need to flush the entry in the TLB
    // or you might not notice the change.
 
   __asm__ volatile("invlpg (%0)" ::"r" (pd) : "memory");
 
}

void test_map_page(){
	u32* p=0xfd0000;
   	vesa_printf(200,60,"virtl:%x phy:%x",p,get_physaddr(p));
	vesa_printf(200,80,"value:%x",*p);
   	map_page(p,0xee0000,PAGE_USU|PAGE_RWW);
	
	vesa_printf(200,100,"virtl:%x phy:%x",p,get_physaddr(p));
	vesa_printf(200,120,"value:%x",*p);	
}

void do_page_fault(enum KP_LEVEL kl,
              u32 ret_ip, u32 ss, u32 gs,
              u32 fs, u32 es, u32 ds, 
              u32 edi, u32 esi, u32 ebp,
              u32 esp, u32 ebx, u32 edx, 
              u32 ecx, u32 eax, u32 isr_nr, 
              u32 err, u32 eip, u32 cs, 
              u32 eflags,u32 old_esp, u32 old_ss) {
    u32 cr2, cr3;
    (void)ret_ip; (void)ss; (void)gs; (void)fs; (void)es; 
    (void)ds; (void)edi; (void)esi; (void)ebp; (void)esp; 
    (void) ebx; (void)edx; (void)ecx; (void)eax; 
    (void)isr_nr; (void)eip; (void)cs; (void)eflags; 
    (void)old_esp; (void)old_ss; (void)kl;
    __asm__ ("movl %%cr2, %%eax":"=a"(cr2));
    __asm__ ("movl %%cr3, %%eax":"=a"(cr3));
    kprintf(KPL_PANIC, "\n  The fault at %x cr3:%x was caused by a %s. "
            "The accessing cause of the fault was a %s, when the "
            "processor was executing in %s mode, page %x is free\n", 
            cr2, cr3,
            (err&0x1)?"page-level protection voilation":"not-present page", 
            (err&0x2)?"write":"read", 
            (err&0x4)?"user":"supervisor",
            0);
}
