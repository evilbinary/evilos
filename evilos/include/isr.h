#ifndef ISR_H
#define ISR_H

void default_isr(void);

#define VALID_ISR	(46+2)


extern void (*isr[(VALID_ISR)<<1])(void);


#endif
