#include "display.h"
#include "vesa.h"

static void gdi_flip(pixel *ptr,int pitch,void *object,int x,int y)
{
	struct gdi_object *obj=(struct gdi_object *)object;
	/*SetDIBitsToDevice(obj->dc,
		x,y,CANVAS_BLOCK_WIDTH,CANVAS_BLOCK_HEIGHT,
		x,0,
		0,CANVAS_BLOCK_HEIGHT,
		ptr-x,&(obj->bi),DIB_RGB_COLORS
	);*/
	//vesa_printf(50,140,"gdi_flip");
	//vesa_fill_rect(x,y,CANVAS_BLOCK_WIDTH,CANVAS_BLOCK_HEIGHT,GREEN);
	int i,j;
	for(i=0;i<CANVAS_BLOCK_HEIGHT;i++){
		for(j=0;j<CANVAS_BLOCK_WIDTH;j++){
			vesa_put_pixel(x+j,y+i,ptr[pitch*(i*CANVAS_BLOCK_WIDTH+j)]);
			 
		}	
	}
	
}

void gdi_create(struct gdi *g,int w,int h)
{
	struct gdi_object *obj;
	g->pitch=(w*sizeof(pixel)+3)&(~3);
	g->flip=gdi_flip;
	g->buffer=(pixel*)malloc(h*g->pitch);
	g->object=obj=(struct gdi_object *)malloc(sizeof(struct gdi_object));
	g->width=w;
	g->height=h;
}

void gdi_release(struct gdi *g)
{
	struct gdi_object *obj=(struct gdi_object *)g->object;
	//ReleaseDC(obj->wnd,obj->dc);
	free(g->buffer);
	free(g->object);	
}
