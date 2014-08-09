#include <malloc.h>
//#include <assert.h>
#include <string.h>
#include "canvas.h"

#define CANVAS_PIPELINE_EXPAND 32

struct pipe_node {
	draw_func func;
	void *object;
	int x;
	int y;
};

struct canvas_block {
	pixel *ptr;
	struct pipe_node *pipeline;
	int total_object;
	int total_last;
	int capability_pipeline;
};

struct canvas {
	int width;
	int height;
	int pitch;
	int block_w;
	int block_h;
	struct canvas_block block[1];
};

static void drawfunc_clear(pixel *ptr,int pitch,void *object,int x,int y)
{
	int i;
	for (i=0;i<CANVAS_BLOCK_HEIGHT;i++) {
		memset(ptr,0,CANVAS_BLOCK_WIDTH*sizeof(pixel));
		ptr=(pixel*)((char*)ptr+pitch);
	}
}

static void canvas_block_resize(struct canvas_block *blk)
{
	const int size=blk->capability_pipeline+CANVAS_PIPELINE_EXPAND;
	const int newsize=size*sizeof(struct pipe_node);
	blk->capability_pipeline=size;
	blk->pipeline=(struct pipe_node*)realloc(blk->pipeline,newsize);
}

static void canvas_init(struct canvas *c,pixel *ptr,draw_func flip,void *flip_object)
{
	int pitch=c->pitch * CANVAS_BLOCK_HEIGHT;
	int i,j;
	const int w=c->width;
	const int h=c->height;
	pixel *line=ptr;
	struct canvas_block *block=c->block;
	for (i=0;i<h;i+=CANVAS_BLOCK_HEIGHT) {
		for (j=0;j<w;j+=CANVAS_BLOCK_WIDTH) {
			block->ptr=line+j;
			block->pipeline=0;
			block->capability_pipeline=0;
			canvas_block_resize(block);
			block->total_object=1;
			block->total_last=0;
			block->pipeline[0].func=flip;
			block->pipeline[0].object=flip_object;
			block->pipeline[0].x=j;
			block->pipeline[0].y=i;
			++block;
		}
		line=(pixel*)((char*)line+pitch);
	}
}

struct canvas* canvas_create(int w,int h,pixel *ptr,int pitch,draw_func flip,void *flip_object)
{
	const int nw=(w+CANVAS_BLOCK_WIDTH-1)/CANVAS_BLOCK_WIDTH;
	const int nh=(h+CANVAS_BLOCK_HEIGHT-1)/CANVAS_BLOCK_HEIGHT;
	struct canvas *c=(struct canvas *)malloc(
		sizeof(struct canvas)-sizeof(struct canvas_block)+
		sizeof(struct canvas_block)*(nw*nh)
	);

	c->width=w;
	c->height=h;
	c->block_w=nw;
	c->block_h=nh;
	c->pitch=pitch;
	canvas_init(c,ptr,flip,flip_object);
	return c;
}

void canvas_release(struct canvas *c)
{
	int i,j;
	const int w=c->block_w;
	const int h=c->block_h;
	struct canvas_block *block=c->block;
	for (i=0;i<h;i++) {
		for (j=0;j<w;j++) {
			free(block->pipeline);
			++block;
		}
	}
	free(c);
}

struct object_2d { COMMON_HEADER } ;

void canvas_draw(struct canvas *c,draw_func func,void *object,int x,int y)
{
	struct object_2d *obj=(struct object_2d *)object;
	int left,right,top,bottom;
	unsigned dirty=obj->dirty;
	if (obj->width<=0 || obj->height<=0)
		return;
	obj->dirty=0;
	left=(x-=obj->kx);
	top=(y-=obj->ky);
	right=left+obj->width-1;
	bottom=top+obj->height-1;

	// clip to canvas
	if (left<0) {
		left=0;
	}
	else if (left>=c->width) {
		return;
	}
	if (top<0) {
		top=0;
	}
	else if (top>=c->height) {
		return;
	}
	if (right>c->width-1) {
		right=c->width-1;
	}
	else if (right<0) {
		return;
	}
	if (bottom>c->height-1) {
		bottom=c->height-1;
	}
	else if (bottom<0) {
		return;
	}

	// splite to blocks
	do {
		int i,j;
		int w=right/CANVAS_BLOCK_WIDTH;
		int h=bottom/CANVAS_BLOCK_HEIGHT;
		int from_x=left/CANVAS_BLOCK_WIDTH;
		int from_y=top/CANVAS_BLOCK_HEIGHT;
		struct canvas_block *block=&c->block[from_y*c->block_w];
		for (i=from_y;i<=h;i++) {
			for (j=from_x;j<=w;j++) {
				struct canvas_block *blk=&block[j];
				struct pipe_node *node;
				if (blk->total_object==blk->capability_pipeline) {
					canvas_block_resize(blk);
				}
				node=&blk->pipeline[blk->total_object++];
				node->func=func;
				node->object=object;
				node->x=x-j*CANVAS_BLOCK_WIDTH;
				node->y=y-i*CANVAS_BLOCK_HEIGHT;
				blk->total_last|=dirty;
			}
			block+=c->block_w;
		}
	} while(0);
}

void canvas_update(struct canvas *c)
{
	int i,j;
	struct canvas_block *blk=c->block;
	int pitch=c->pitch;
	for (i=c->block_w*c->block_h;i>0;--i,++blk) {
		if (blk->total_object!=blk->total_last) {
			pixel *ptr=blk->ptr;
			drawfunc_clear(ptr,pitch,0,0,0);
			for (j=blk->total_object-1;j>=0;j--) {
				struct pipe_node *node=&blk->pipeline[j];
				node->func(ptr,pitch,node->object,node->x,node->y);
			}
			blk->total_last=blk->total_object;
		}
		blk->total_object=1;
	}
}

void canvas_redraw(struct canvas *c)
{
	int i;
	struct canvas_block *blk=c->block;
	int pitch=c->pitch;
	for (i=c->block_w*c->block_h;i>0;--i,++blk) {
		blk->total_last=0;
	}
}
