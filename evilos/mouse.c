#include <mouse.h>
#include <asm.h>
#include <vesa.h>

char mousemoves=0;
char mouse_byte[4];    			//signed char
char mouse_haswheel=0;		// zero indicates mouse has no wheel
char wasdrag=0;

void do_mouse(void){
	static unsigned char mousecount=0;  
  	mousemoves=0;
	if ((inb(0x64) & 0x01) != 0) {
    	mouse_byte[mousecount++]=inb(0x60);
    	if (mousecount >= 3+mouse_haswheel) {
	    	mousecount=0;
	    	mousemoves=1;
	    	
    	}
	}
	print_c('0'+mousecount, WHITE, BLACK);
	//vesa_draw_string(0,180,"aaddddddddddddddddtt");
	outb(0x20,0x20);
	outb(0x20,0xA0);	
}


void mouse_wait(char type){
	unsigned int time_out=100000; //unsigned int
	if(type==0){
		while(time_out--){//data
		  if((inb(0x64) & 1)==1){
		  	break;
		  }
		}
	return;
	}else{
		while(time_out--){//siga
		  if((inb(0x64) & 2)==0){
		    return;
		  }
		}
		return;
  }	
}
void mouse_write(unsigned char data){
  mouse_wait(1);
  outb( 0xD4,0x64);
  mouse_wait(1);
  outb(data,0x60);
}
unsigned char mouse_read(){
  mouse_wait(0);
  return inb(0x60);
}

void mouse_install(){
	unsigned char status;
	mouse_wait(1);
	outb(0xa8,0x64);//enable device
	mouse_wait(1);
	outb(0x20,0x64);
	mouse_wait(0);
	status=(inb(0x60)|2);//enable interrupt
	mouse_wait(1);
	outb(0x60,0x64);
	mouse_wait(1);
	outb(status,0x60);
	mouse_write(0xF6);
	mouse_read();
	mouse_write(0xF4);
	mouse_read();
	//vesa_draw_string(0,100,"mouse_install");
	
	/*outb(0xa8,0x64);
	outb(0xd4,0x64);
	outb(0xf4,0x60);
	outb(0x60,0x64);
	outb(0x47,0x60);*/
	outb(inb(0x21)&0xfb, 0x21);
	outb(inb(0xa1)&0xef, 0xa1);
}