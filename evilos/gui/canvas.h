#ifndef _DIRTYRECT_CANVAS_H
#define _DIRTYRECT_CANVAS_H

#define CANVAS_BLOCK_WIDTH 64
#define CANVAS_BLOCK_HEIGHT 32
#define DIRTY (~0)

typedef unsigned short pixel;
typedef void (*draw_func)(pixel *ptr,int pitch,void *object,int x,int y);
struct canvas;

#define COMMON_HEADER	\
	unsigned dirty;	\
	int width;	\
	int height;	\
	int kx;	\
	int ky;

struct canvas* canvas_create(int w,int h,pixel *ptr,int pitch,draw_func flip,void *flip_object);
void canvas_release(struct canvas *c);
void canvas_draw(struct canvas *c,draw_func func,void *object,int x,int y);
void canvas_update(struct canvas *c);
void canvas_redraw(struct canvas *c);

#endif