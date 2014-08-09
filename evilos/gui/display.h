#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "canvas.h"

struct gdi {
	draw_func flip;
	void *object;
	pixel *buffer;
	int width;
	int height;
	int pitch;
};

struct gdi_object {
	//HWND wnd;
	//HDC dc;
	//BITMAPINFO bi;
	//RGBQUAD pal[2];
};

void gdi_create(struct gdi *g,int w,int h);
void gdi_release(struct gdi *g);


#endif