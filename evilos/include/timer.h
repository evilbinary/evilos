#ifndef TIMER_H
#define TIMER_H

void timer_install(int);
void dotimer(void);

extern volatile unsigned long timer_ticks;

#endif
