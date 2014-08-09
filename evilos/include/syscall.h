#ifndef SYSCALL_H
#define SYSCALL_H

#define SYS_CALL	0x80
extern void sys_call(void);
extern void sys_print(void);

void syscall_init(void);
void syscall_isr(void);
void syscall_exit(void);


#endif
