#include "box.h"

static void draw_box(pixel *ptr,int pitch,void *object,int x,int y)
{
	struct box *obj=(struct box*)object;
	int w=obj->width;
	int h=obj->height;
	int i,j;
	pixel *buffer=ptr;
	pixel color=obj->color;
	if (y<0) {
		h+=y;
		y=0;
	}
	else {
		buffer=(pixel*)((char*)buffer+y*pitch);
	}
	if (x<0) {
		w+=x;
		x=0;
	}
	else {
		buffer+=x;
	}
	if (w+x>CANVAS_BLOCK_WIDTH) {
		w=CANVAS_BLOCK_WIDTH-x;
	}
	if (h+y>CANVAS_BLOCK_HEIGHT) {
		h=CANVAS_BLOCK_HEIGHT-y;
	}
	for (i=0;i<h;i++) {
		for (j=0;j<w;j++) {
			buffer[j]=color;
		}
		buffer=(pixel*)((char*)buffer+pitch);
	}
}

void box_draw(struct canvas *c,struct box *b,int x,int y)
{
	canvas_draw(c,draw_box,b,x,y);
}