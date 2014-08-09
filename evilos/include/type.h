#ifndef	_TYPE_H
#define _TYPE_H

typedef char   e8;
typedef short  e16;
typedef int    e32;
typedef double e64;
typedef int  ebool;
typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned int    u32;
typedef unsigned long long u64;

typedef void (*function_handl)();

#define NULL 0

struct point_t{
	e32 x;
	e32 y;
};

#endif
