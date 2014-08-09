#include <multiboot.h>
#include <screen.h>

multiboot_info_t *multiboot_info=0;


/* Check if the bit BIT in FLAGS is set.  */
#define CHECK_FLAG(flags,bit)	((flags) & (1 << (bit)))

 
/* Forward declarations.  */
//void cmain (unsigned long magic, unsigned long addr);
 

/* Check if MAGIC is valid and print the Multiboot information structure
   pointed by ADDR.  */
void cmain (unsigned long magic, unsigned long addr)
{
  multiboot_info_t *mbi;
  
  /* Clear the screen.  */
  cls ();

  /* Am I booted by a Multiboot-compliant boot loader?  */
  if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
      printf ("Invalid magic number: 0x%x\n", (unsigned) magic);
      return;
    }

  /* Set MBI to the address of the Multiboot information structure.  */
  mbi = (multiboot_info_t *) addr;
	multiboot_info=mbi;
  /* Print out the flags.  */
  printf ("addr:0x%x flags = 0x%x\n",addr,addr,(unsigned) mbi->flags);

  /* Are mem_* valid?  */
  if (CHECK_FLAG (mbi->flags, 0))
    printf ("mem_lower = %uKB, mem_upper = %uKB\n",
	    (unsigned) mbi->mem_lower, (unsigned) mbi->mem_upper);

  /* Is boot_device valid?  */
  if (CHECK_FLAG (mbi->flags, 1))
    printf ("boot_device = 0x%x\n", (unsigned) mbi->boot_device);
  
  /* Is the command line passed?  */
  if (CHECK_FLAG (mbi->flags, 2))
    printf ("cmdline = %s\n", (char *) mbi->cmdline);

  /* Are mods_* valid?  */
  if (CHECK_FLAG (mbi->flags, 3))
    {
      multiboot_module_t *mod;
      int i;
      
      printf ("mods_count = %d, mods_addr = 0x%x\n",
	      (int) mbi->mods_count, (int) mbi->mods_addr);
      for (i = 0, mod = (multiboot_module_t *) mbi->mods_addr;
	   i < mbi->mods_count;
	   i++, mod++)
	printf (" mod_start = 0x%x, mod_end = 0x%x, cmdline = %s\n",
		(unsigned) mod->mod_start,
		(unsigned) mod->mod_end,
		(char *) mod->cmdline);
    }

  /* Bits 4 and 5 are mutually exclusive!  */
  if (CHECK_FLAG (mbi->flags, 4) && CHECK_FLAG (mbi->flags, 5))
    {
      printf ("Both bits 4 and 5 are set.\n");
      return;
    }

  /* Is the symbol table of a.out valid?  */
  if (CHECK_FLAG (mbi->flags, 4))
    {
      multiboot_aout_symbol_table_t *multiboot_aout_sym = &(mbi->u.aout_sym);
      
      printf ("multiboot_aout_symbol_table: tabsize = 0x%0x, "
	      "strsize = 0x%x, addr = 0x%x\n",
	      (unsigned) multiboot_aout_sym->tabsize,
	      (unsigned) multiboot_aout_sym->strsize,
	      (unsigned) multiboot_aout_sym->addr);
    }

  /* Is the section header table of ELF valid?  */
  if (CHECK_FLAG (mbi->flags, 5))
    {
      multiboot_elf_section_header_table_t *multiboot_elf_sec = &(mbi->u.elf_sec);

      printf ("multiboot_elf_sec: num = %u, size = 0x%x,"
	      " addr = 0x%x, shndx = 0x%x\n",
	      (unsigned) multiboot_elf_sec->num, (unsigned) multiboot_elf_sec->size,
	      (unsigned) multiboot_elf_sec->addr, (unsigned) multiboot_elf_sec->shndx);
    }

  /* Are mmap_* valid?  */
  if (CHECK_FLAG (mbi->flags, 6))
    {
      multiboot_memory_map_t *mmap;
      
      printf ("mmap_addr = 0x%x, mmap_length = 0x%x\n",
	      (unsigned) mbi->mmap_addr, (unsigned) mbi->mmap_length);
 
   }
    if (CHECK_FLAG (mbi->flags, 7))
    {
      printf("drives set\n");
    }
    if (CHECK_FLAG (mbi->flags, 8))
    {
      printf("config_table set\n");
    }
    if (CHECK_FLAG (mbi->flags, 9))
    {
      printf("boot_loader_name set\n");
    }
    if (CHECK_FLAG (mbi->flags, 10))
    {
      printf("apm_table set\n");
    }
    if (CHECK_FLAG (mbi->flags, 11))
    {
      printf("vbeinfo set\n");
    }
  	init();
  	
}    


