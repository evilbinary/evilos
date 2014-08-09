#ifndef SCREEN_H
#define SCREEN_H

#define MAX_LINES	25
#define MAX_COLUMNS	80
#define TAB_WIDTH	8			/* must be power of 2 */

#define VIDEO_RAM	0xb8000

#define LINE_RAM	(MAX_COLUMNS*2)

#define PAGE_RAM	(MAX_LINE*MAX_COLUMNS)

#define BLANK_CHAR	(' ')
#define BLANK_ATTR	(0x07)		/* white fg, black bg */

#define CHAR_OFF(x,y)	(LINE_RAM*(y)+2*(x))


/* Some screen stuff.  */
/* The number of columns.  */
#define COLUMNS			80
/* The number of lines.  */
#define LINES			24
/* The attribute of an character.  */
#define ATTRIBUTE		7
/* The video memory address.  */
#define VIDEO			0xB8000

typedef enum COLOUR_TAG {
	BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, WHITE,
	GRAY, LIGHT_BLUE, LIGHT_GREEN, LIGHT_CYAN, 
	LIGHT_RED, LIGHT_MAGENTA, YELLOW, BRIGHT_WHITE
} COLOUR;

void set_cursor(int, int);
void get_cursor(int *, int *);
void print_c(char c, COLOUR fg, COLOUR bg);

void cls (void);
void itoa (char *buf, int base, int d);
void putchar (int c);
void printf (const char *format, ...);

#endif
