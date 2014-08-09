#include <screen.h>
#include <isr.h>
#include <asm.h>
#include <kernel.h>
#include <task.h>
#include <libcc.h>
#include <timer.h>
#include <kprintf.h>
#include <kb.h>
#include <page.h>
#include <vesa.h>
#include <string.h>
#include <mouse.h>
#include <box.h>
#include <interrupt.h>
#include "display.h"

static long task1_stack0[1024] = {0xf, };
static long task1_stack3[1024] = {0xf, };
static long task2_stack0[1024] = {0xf, };
static long task2_stack3[1024] = {0xf, };

static long task3_stack0[1024] = {0xf, };
static long task3_stack3[1024] = {0xf, };




void do_task1(void) {
	u32 i=0;
	char wheel[] = {'\\', '|', '/', '-'};
	for(;;){
    	u32 *p=0xff0000;
        *p=i;
        vesa_printf(0,00,"task1:%d\n",i);
        __asm__ ("incb 0xb8000+160*24+4");
        //putchar('A');
		//vesa_draw_char(0,00,wheel[i]);
        //vesa_fill_rect(++i,0+100,50,50,0xffffffff);
        //vesa_fill_rect(i,0+100,1,50,0);
		//__asm__ ("int $0x80\n\t");
	}
}

void do_task2(void) {
	char wheel[] = {'\\', '|', '/', '-'};
	u32 i=0;
    for(;;){
    	u32 *p=0xff0000;
    	//vesa_draw_char(0,20,wheel[i]);
       	vesa_printf(0,20,"task2:%d\n",*p);
        __asm__ ("incb 0xb8000+160*24+6");

        //putchar('B');
		//__asm__("xchgw %bx,%bx");
	}
}
void do_task3(){
	//gmMain(2,"");
	int i,j,k;
	int f1,f2,f3;
	struct box b;
	struct box b1;
	struct box b2;
	struct gdi g_gdi;
	struct canvas *g_canvas;

	vesa_printf(20,40,"gdi_create\n");
	gdi_create(&g_gdi,100,100);
	vesa_printf(20,60,"canvas_create\n");
   	g_canvas=canvas_create(g_gdi.width,g_gdi.height,g_gdi.buffer,g_gdi.pitch,g_gdi.flip,g_gdi.object);

	b.kx=130;
	b.ky=130;
	b.width=100;
	b.height=100;
	b.color=0xffff;
	b.dirty=DIRTY;

	b1.kx=100;
	b1.ky=100;
	b1.width=100;
	b1.height=100;
	b1.color=0xff00;
	b1.dirty=DIRTY;

	b2.kx=150;
	b2.ky=150;
	b2.width=150;
	b2.height=150;
	b2.color=0x00ff;
	b2.dirty=DIRTY;

	// Main message loop:
	i=0;
	j=10;
	k=0;
	f1=1;
	f2=1;
	f3=1;
	vesa_printf(20,80,"canvas_redraw\n");
	//canvas_redraw(g_canvas);
    for(;;){
    	u32 *p=0xff0000;
       	vesa_printf(0,40,"task3:%d\n",*p);
        __asm__ ("incb 0xb8000+160*24+6");

        //putchar('C');

        b.dirty=DIRTY;
		b1.dirty=DIRTY;
		b2.dirty=DIRTY;
		box_draw(g_canvas,&b,i,i);
		box_draw(g_canvas,&b1,j,j);
		box_draw(g_canvas,&b2,k,k);
		canvas_update(g_canvas);
		i+=f1;
		j+=f2;
		k+=f3;
		b2.width+=f3;
		b2.height+=f3;
		if(i>k){
			f1=-1;
		}else if(i<=k){
			f1=1;
		}
		if(j>=250){
			f2=-2;
		}else if(j<100){
			f2=2;
		}
		if(k>=250){
			f3=-1;
		}else if(k<100){
			f3=1;
		}

    }
    gdi_release(&g_gdi);
	canvas_release(g_canvas);
}

void test_float(){
	float a=0.111;
	float b=0.2223;
	printf("a:%f a*b:%f",a,b*a);
}
void init(void) {
	char wheel[] = {'\\', '|', '/', '-'};
	int i = 0;
	struct task_struct task1;
	struct task_struct task2;
	struct task_struct task3;

    gdt_install();

	set_tss((unsigned long long)&TASK0.tss);
	set_ldt((unsigned long long)&TASK0.ldt);
	__asm__ ("ltrw	%%ax\n\t"::"a"(TSS_SEL));
	__asm__ ("lldt	%%ax\n\t"::"a"(LDT_SEL));

	idt_install();
	pic_install();
	timer_install(1000);
	kb_install();
	syscall_init();
	mem_init();
    //mem_test();
    vesa_init();
	mouse_install();
   	page_init();

   	__asm__("int $0x08\n\t");

   	//test_map_page();
  	test_float();
	vesa_fill_rect(200,270,50,50,RED);
	vesa_draw_string(200,40,"test0123456789");
	vesa_draw_string_with_color(200,00,"welcom to eeos!",GREEN,BLACK);
	vesa_draw_string_with_color(200,20,"This is my world.",RED,WHITE);
	vesa_fill_rect(220,300,50,50,GREEN);
  	//vesa_put_ascii(0,0,"ddd",0xffffffff);
  	vesa_draw_line(100,100,240,200,RED);

	//__asm__("xchgw %bx,%bx");
	//__asm__("int 0x10");
	//__asm__("ud2\n\t"::);
	//__asm__("xchgw %bx,%bx");
	//sem_test1();
	__asm__("xchgw %bx,%bx");
	new_task(&task1,
			(unsigned int)do_task1,
			(unsigned int)task1_stack0+sizeof task1_stack0,
			(unsigned int)task1_stack3+sizeof task1_stack3);
	new_task(&task2,
			(unsigned int)do_task2,
			(unsigned int)task2_stack0+sizeof task2_stack0,
			(unsigned int)task2_stack3+sizeof task2_stack3);
	/*new_task(&task3,
			(unsigned int)do_task3,
			(unsigned int)task3_stack0+sizeof task3_stack0,
			(unsigned int)task3_stack3+sizeof task3_stack3);
	vesa_printf(360,0,"all:");
	print_tasks(400,0);*/
	//printf("test=%d",1/0);
	cls();
	//sem_test1();
	//test_task_op();

	//test_pipe();
	//vesa_printf(360,300,"all:");
	//print_tasks(400,300);
	//vesa_printf(0,0,"stop");


	sti();
	__asm__ ("movl %%esp,%%eax\n\t" \
			 "pushl %%ecx\n\t" \
			 "pushl %%eax\n\t" \
			 "pushfl\n\t" \
			 "pushl %%ebx\n\t" \
			 "pushl $1f\n\t" \
			 "iret\n" \
			 "1:\tmovw %%cx,%%ds\n\t" \
			 "movw %%cx,%%es\n\t" \
			 "movw %%cx,%%fs\n\t" \
			 "movw %%cx,%%gs" \
			 ::"b"(USER_CODE_SEL),"c"(USER_DATA_SEL));
	for (;;) {
		//vesa_printf(360,300,"all-:");
		//print_tasks(400,300);
		__asm__ ("movb	%%al,	0xb8000+160*24"::"a"(wheel[i]));
		vesa_draw_char(0,460,wheel[i]);
		if (i == sizeof wheel){
			i=0;
		}
		else
			++i;
	}
}
