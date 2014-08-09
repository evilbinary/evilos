#ifndef KPRINTF_H
#define KPRINTF_H


enum KP_LEVEL {KPL_DUMP, KPL_PANIC};

void kprintf(enum KP_LEVEL, const char *fmt, ...);

#endif
