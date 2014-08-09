#include <asm.h>
#include <screen.h>
#include <libcc.h>
#include <string.h>

/* Variables.  */
/* Save the X position.  */
static int xpos;
/* Save the Y position.  */
static int ypos;
/* Point to the video memory.  */
static volatile unsigned char *video;


/* Clear the screen and initialize VIDEO, XPOS and YPOS.  */
 void
cls (void)
{
  int i;

  video = (unsigned char *) VIDEO;
  
  for (i = 0; i < COLUMNS * LINES * 2; i++)
    *(video + i) = 0;

  xpos = 0;
  ypos = 0;
}

/* Convert the integer D to a string and save the string in BUF. If
   BASE is equal to 'd', interpret that D is decimal, and if BASE is
   equal to 'x', interpret that D is hexadecimal.  */
void
itoa (char *buf, int base, int d)
{
  char *p = buf;
  char *p1, *p2;
  unsigned long ud = d;
  int divisor = 10;
  
  /* If %d is specified and D is minus, put `-' in the head.  */
  if (base == 'd' && d < 0)
    {
      *p++ = '-';
      buf++;
      ud = -d;
    }
  else if (base == 'x')
    divisor = 16;

  /* Divide UD by DIVISOR until UD == 0.  */
  do
    {
      int remainder = ud % divisor;
      
      *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    }
  while (ud /= divisor);

  /* Terminate BUF.  */
  *p = 0;
  
  /* Reverse BUF.  */
  p1 = buf;
  p2 = p - 1;
  while (p1 < p2)
    {
      char tmp = *p1;
      *p1 = *p2;
      *p2 = tmp;
      p1++;
      p2--;
    }
}

/* Put the character C on the screen.  */
void
putchar (int c)
{
  if (c == '\n' || c == '\r')
    {
    newline:
      xpos = 0;
      ypos++;
      if (ypos >= LINES)
	ypos = 0;
      return;
    }

  *(video + (xpos + ypos * COLUMNS) * 2) = c & 0xFF;
  *(video + (xpos + ypos * COLUMNS) * 2 + 1) = ATTRIBUTE;

  xpos++;
  if (xpos >= COLUMNS)
    goto newline;
}

/* Format a string and print it on the screen, just like the libc
   function printf.  */
void
screen_printf (const char *format, ...)
{
  char **arg = (char **) &format;
  int c;
  char buf[20];

  arg++;
  
  while ((c = *format++) != 0)
    {
      if (c != '%')
	putchar (c);
      else
	{
	  char *p;
	  
	  c = *format++;
	  switch (c)
	    {
	    case 'd':
	    case 'u':
	    case 'x':
	      itoa (buf, c, *((int *) arg++));
	      p = buf;
	      goto string;
	      break;

	    case 's':
	      p = *arg++;
	      if (! p)
		p = "(null)";

	    string:
	      while (*p)
		putchar (*p++);
	      break;

	    default:
	      putchar (*((int *) arg++));
	      break;
	    }
	}
    }
}

static int csr_x = 0;
static int csr_y = 0;


static void 
scroll(int lines) {
	int x = MAX_COLUMNS-1, y = MAX_LINES*(lines-1)+MAX_LINES-1;
	short *p = (short *)(VIDEO_RAM+CHAR_OFF(x, y));
	int i = MAX_COLUMNS*(lines-1) + MAX_COLUMNS;
	memcpy((void *)VIDEO_RAM, (void *)(VIDEO_RAM+LINE_RAM*lines),
		   LINE_RAM*(MAX_LINES-lines));
	for (; i>0; --i)
		*p-- = (short)((BLANK_ATTR<<8)|BLANK_CHAR);
}

void 
set_cursor(int x, int y) {
	csr_x = x;
	csr_y = y;
	outb(0x0e, 0x3d4);
	outb(((csr_x+csr_y*MAX_COLUMNS)>>8)&0xff, 0x3d5);
	outb(0x0f, 0x3d4);
	outb(((csr_x+csr_y*MAX_COLUMNS))&0xff, 0x3d5);
}

void
get_cursor(int *x, int *y) {
	*x = csr_x;
	*y = csr_y;
}

void 
print_c(char c, COLOUR fg, COLOUR bg) {
	char *p; 
	char attr;
	p = (char *)VIDEO_RAM+CHAR_OFF(csr_x, csr_y);
	attr = (char)(bg<<4|fg);
	switch (c) {
	case '\r':
		csr_x = 0;
		break;
	case '\n':
		for (; csr_x<MAX_COLUMNS; ++csr_x) {
			*p++ = BLANK_CHAR;
			*p++ = attr;
		}
		break;
	case '\t':
		c = csr_x+TAB_WIDTH-(csr_x&(TAB_WIDTH-1));
		c = c<MAX_COLUMNS?c:MAX_COLUMNS;
		for (; csr_x<c; ++csr_x) {
			*p++ = BLANK_CHAR;
			*p++ = attr;
		}
		break;
	case '\b':
		if ((! csr_x) && (! csr_y))
			return;
		if (! csr_x) {
			csr_x = MAX_COLUMNS - 1;
			--csr_y;
		} else
			--csr_x;
		((short *)p)[-1] = (short)((BLANK_ATTR<<8)|BLANK_CHAR);
		break;
	default:
		*p++ = c; 
		*p++ = attr;
		++csr_x;
		break;
	}
	if (csr_x >= MAX_COLUMNS) {
		csr_x = 0;
		if (csr_y < MAX_LINES-1)
			++csr_y;
		else 
			scroll(1);
	}
	set_cursor(csr_x, csr_y);
}




