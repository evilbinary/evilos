#ifndef _PYOS_VESA_H_
#define _PYOS_VESA_H_

#include <type.h>
#include <stdarg.h>

#define RED 0xff0000
#define GREEN  0x00ff00
#define BLUE 0x0000ff
#define WHITE 0xffffff
#define BLACK 0x000000
#define GRAY 0x808080
#define YELLOW 0xFFFF00
#define DIMGRAY 0x696969

#define CONRNISLK 0xFFF8DC
#define PALEGODENROD 0xEEE8AA
#define CRIMSON	0xDC143C

#define DARKRED 0x8b0000
#define FIREBRICK 0xb22222
#define CYAN	0xFF00FF
#define MAGENTA	0xFF00FF
#define BROWN	0xA52A2A
#define WHITESMOKE 0xF5F5F5
#define DARKGRAY 0xa9a9a9	
#define LIGHTGRAY	0xd3d3d3
#define LIGHTBLUE	0xadd8e6
#define LIGHTGREEN	0x90ee90
#define LIGHTCYAN	0xE1FFFF
#define BEIGE	0x6B8E23


e32 vesa_init(void);
void vesa_put_pixel(e32 x,e32 y,e32 color);
void vesa_draw_vline(e32 x1,e32 x2,e32 y, e32 color);
void vesa_draw_hline(e32 y1,e32 y2,e32 x, e32 color);
void vesa_clear_screen(void);
void vesa_vram_to_buf( e32 x1, e32 y1, e32 x2, e32 y2,e32* buf);
void vesa_buf_to_vram( e32 x1, e32 y1, e32 x2, e32 y2,e32* buf);
e32 vesa_get_screen_width(void);
e32 vesa_get_screen_height(void);
void vesa_screen_scroll_down(e32 n);
void vesa_screen_scroll_up(e32 n);
void vesa_draw_line(u32 x1,u32 y1,u32 x2,u32 y2,u32 color);
void vesa_put_ascii(e32 x, e32 y, u8 ch, e32 color);

void vesa_printf(e32 x,e32 y,char *format,...);

void vesa_draw_string(e32 x,e32 y,e8 *str);
void vesa_draw_char_witdh_color(e32 x,e32 y,u16 ch,u32 frcolor,u32 bgcolor);
void vesa_draw_string_with_color(e32 x,e32 y,e8 *str,u32 frcolor,u32 bgcolor);
 
 
// 色调合成函数
e32 vesa_compond_rgb( u8 r , u8 g , u8 b ) ;

// 色调转换函数，把 5:5:5 格式转换成标准的 5:6:5 格式
e32 vesa_change_color_form_555_to_565( e32 color_form_555 ) ;

// 画点函数
void vesa_draw_point( e32 x , e32 y , e32 color ) ;

// 矩形填充函数
void vesa_fill_rect(e32 x,e32 y,e32 w,e32 h,u32 color) ;

// 清屏函数
void vesa_clean_screen( u32 color ) ;

// 画横线函数
void vesa_draw_x_line( e32 y , e32 x1 , e32 x2 , e32 color ) ;
// 画竖线函数
void vesa_draw_y_line( e32 x , e32 y1 , e32 y2 , e32 color ) ;
// 画矩形函数
void vesa_draw_rect( e32 x1 , e32 y1 , e32 x2 , e32 y2 , e32 color , e32 dose_fill_it ) ; 

// 显示英文
void vesa_print_english( e32 x , e32 y , e32 pos_in_font , e32 color ) ;

// 显示汉字
void vesa_print_chinese( e32 x , e32 y , e32 pos_in_font , e32 color ) ;

// 显示 bmp 格式的图片
void vesa_show_bmp_picture( e32 x , e32 y , void *bmp_addr , e32 mask_color , e32 dose_use_mask_color ) ;

// 显示字符
void vesa_print_char_under_text_model( e8 x ) ;

// 图片拷贝函数
void vesa_copy_picture_from_screen( e32 x , e32 y , u32 *object_picture_addr , e32 picture_width , e32 picture_height ) ;

// 图片拷贝函数
void vesa_copy_picture_to_screen( e32 x , e32 y , u32 *source_picture_addr , e32 picture_width , e32 pictrue_height ) ;
// 取得指定点色彩
e32 vesa_get_point_color( e32 x , e32 y ) ;

#endif
