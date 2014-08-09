#ifndef	_MUTEX_H_
#define _MUTEX_H_

typedef unsigned long mutex_t;

static inline unsigned long xchgl(unsigned long x, unsigned long *ptr){
    __asm__ __volatile__("xchgl %0,%1\n\t"
        :"=r"(x)
        :"m"(*ptr),"0"(x)
        :"memory"
        );
    return x;
}

/*mptr是mutex_t的指针*/
#define mutex_lock(mptr) ({\
    while(xchgl(1,(mptr)));\
})

/*mptr是mutex_t的指针*/
#define mutex_unlock(mptr) xchgl(0,(mptr))


/*mptr是mutex_t的指针*/
#define init_mutex(mptr) ({*mptr = 0;})
#define mutex_init(mptr) ({*mptr = 0;})


#endif
