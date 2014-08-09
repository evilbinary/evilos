#ifndef _DIRTYRECT_BOX_H
#define _DIRTYRECT_BOX_H

#include "canvas.h"

struct box {
	COMMON_HEADER
	pixel color;
};

void box_draw(struct canvas *c,struct box *b,int x,int y);

#endif