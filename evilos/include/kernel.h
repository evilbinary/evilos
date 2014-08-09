#ifndef KERNEL_H
#define KERNEL_H

/* BIOS could load boot sector to 07c0:0000 or 0000:07c0, 
   ds will be set to 0x0000, the LOAD_ADDR is the offset in 
   segment 0x0000 */
#define LOAD_ADDR 	0x7c00
/* in protected mode, set the stack just under the video
   ram section which locates at 0xa0000 */
#define STACK_BOT 	0xa0000

/* code selector in gdt */
#define CODE_SEL	0x08

/* data and stack selecter in gdt */
#define DATA_SEL	0x10

/* code selector in ldt */
#define USER_CODE_SEL	0x07

/* data and stack selector in ldt */
#define USER_DATA_SEL	0x0f

/* IDT_ADDR and GDT_ADDR must align to 16 */
#define IDT_ADDR	0x8000
#define IDT_SIZE	(256*8)
#define GDT_ADDR	((IDT_ADDR)+IDT_SIZE)
#define GDT_ENTRIES	5
#define GDT_SIZE	(8*GDT_ENTRIES)
/* 
   NULL				0	0
   CODE_SEL			8	1
   DATA_SEL			10	2
   CURR_TASK_TSS	18	3
   CURR_TASK_LDT	20	4
 */

#define CURR_TASK_TSS 	3
#define	CURR_TASK_LDT	4

#define TSS_SEL		0x18
#define	LDT_SEL		0x20

#define TSS_SEL_STR	"0x18"
#define LDT_SEL_STR	"0x20"

extern unsigned long long *gdt;
extern unsigned long long *ldt;


#include "type.h"


#define ATTRIB_DR 0x90
#define ATTRIB_DRW 0x92
#define ATTRIB_DRWA 0x93
#define ATTRIB_CE 0x98
#define ATTRIB_CRE 0x9A
#define ATTRIB_CE32 0x409A
#define ATTRIB_32 0x4000
#define ATTRIB_LIMIT4K 0x8000

#define ATTRIB_LDT 0x82
#define ATTRIB_TGATE 0x85
#define ATTRIB_TSS386 0x89
#define ATTRIB_CGATE386 0x8C
#define ATTRIB_IGATE386 0x8E
#define ATTRIB_TGATE386 0x8F
#define ATTRIB_SEL_GIG 0x00
#define ATTRIB_SEL_TIL 0x4

#define ATTRIB_DPL0 0x0
#define ATTRIB_DPL1 0x20
#define ATTRIB_DPL2 0x40
#define ATTRIB_DPL3 0x60

#define ATTRIB_RPL0 0x0
#define ATTRIB_RPL1 0x1
#define ATTRIB_RPL2 0x2
#define ATTRIB_RPL3 0x3

typedef struct GDT{
    u16 limit_low;
    u16 base_low;
    u8  base_middle;
    u16 attr_plus_limit;
    u8  base_high;
}__attribute__((packed))GDT;

typedef struct PDES{
    u16 limit;
    u64 base;
}__attribute__((packed))PDES;


#endif
