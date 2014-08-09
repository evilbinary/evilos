#include "syscall.h"

void syscall_init(){
	int i;
	for (i=46+2; i<256; ++i)
	isr_entry(i, (unsigned int)syscall_isr);

}
void syscall_isr(void){
	kprintf(0,"syscall isr\n");
	printf("syscall isr\n");
}
void syscall_exit(void){

}
