#ifndef LIBCC_H
#define LIBCC_H

#define NULL	((void *)0)

void bcopy(const void *src, void *dest, unsigned int n);
void bzero(void *dest, unsigned int n);


#define assert(s) do { if (! (s)) {	\
kprintf(KPL_PANIC, "ASSERT: %s, %d", __FILE__, __LINE__);	\
halt();	\
}} while (0)

#endif
