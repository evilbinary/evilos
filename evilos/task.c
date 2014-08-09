#include <task.h>
#include <kernel.h>
#include <asm.h>
#include <type.h>
#include <page.h>
#include <mutex.h>

static unsigned long TASK0_STACK[256] = {0xf};

struct task_struct TASK0 = {
	/* tss */
	{	/* back_link */
		0,
		/* esp0                                    ss0 */
		(unsigned)&TASK0_STACK+sizeof TASK0_STACK, DATA_SEL,
		/* esp1 ss1 esp2 ss2 */
		0, 0, 0, 0,
		/* cr3 */
		PAGE_DIR_ADDR,
		/* eip eflags */
		0, 0,
		/* eax ecx edx ebx */
		0, 0, 0, 0,
		/* esp ebp */
		0, 0,
		/* esi edi */
		0, 0,
		/* es          cs             ds */
		USER_DATA_SEL, USER_CODE_SEL, USER_DATA_SEL,
		/* ss          fs             gs */
		USER_DATA_SEL, USER_DATA_SEL, USER_DATA_SEL,
		/* ldt */
		0x20,
		/* trace_bitmap */
		0x00000000},
	/* tss_entry */
	0,
	/* idt[2] */
	{DEFAULT_LDT_CODE, DEFAULT_LDT_DATA},
	/* idt_entry */
	0,
	/* state */
	TS_RUNNING,
	/* priority */
	INITIAL_PRIO,
	/* next */
	0,
};

unsigned long long
set_tss(unsigned long long tss) {
	unsigned long long tss_entry = 0x0080890000000067ULL;
	tss_entry |= ((tss)<<16) & 0xffffff0000ULL;
	tss_entry |= ((tss)<<32) & 0xff00000000000000ULL;
	return gdt[CURR_TASK_TSS] = tss_entry;
}

unsigned long long
set_ldt(unsigned long long ldt) {
	unsigned long long ldt_entry = 0x008082000000000fULL;
	ldt_entry |= ((ldt)<<16) & 0xffffff0000ULL;
	ldt_entry |= ((ldt)<<32) & 0xff00000000000000ULL;
	return gdt[CURR_TASK_LDT] = ldt_entry;
}

void set_gdt_des(u32 index,u16 attrib,u32 addr,u16 limitl){
    GDT *tgdt=(GDT*)gdt;
	tgdt[index].limit_low=limitl;//sizeof(Descriptor)*5;
	tgdt[index].base_low=(u16)addr&0xffff;
	tgdt[index].base_middle=(u8)(addr>>16)&0xff;
	tgdt[index].attr_plus_limit=attrib;
	tgdt[index].base_high=(u8)(addr>>24)&0xff;
}



unsigned long long
get_tss(void) {
	return gdt[CURR_TASK_TSS];
}

unsigned long long
get_ldt(void) {
	return gdt[CURR_TASK_LDT];
}

struct task_struct *current = &TASK0;

void scheduler(void) {
	struct task_struct *v = &TASK0, *tmp = 0;
	int cp;
	if(current->state ==TS_SLEEP){
		current=v;
	}
	cp= current->priority;
	for (; v; v = v->next) {
		if ((v->state==TS_RUNABLE) && (cp>=v->priority)) {
			tmp = v;
			cp = v->priority;
			//vesa_printf(100,520,"addr:%x state:%d p:%d",v,v->state,v->priority);
		}
	}
	if (tmp && (tmp!=current)) {
		current->tss_entry = get_tss();
		current->ldt_entry = get_ldt();
		tmp->tss_entry = set_tss((unsigned long long)((unsigned int)&tmp->tss));
		tmp->ldt_entry = set_ldt((unsigned long long)((unsigned int)&tmp->ldt));
		current->state = TS_RUNABLE;
		tmp->state = TS_RUNNING;
		current = tmp;
		__asm__ __volatile__("ljmp	$" TSS_SEL_STR ",	$0\n\t");
	}
}

void new_task(struct task_struct *task, unsigned int eip,
				unsigned int stack0, unsigned int stack3) {
	memcpy(task, &TASK0, sizeof(struct task_struct));
	task->tss.esp0 = stack0;
	task->tss.eip = eip;
	task->tss.eflags = 0x3202;
	task->tss.esp = stack3;

	task->priority = INITIAL_PRIO;

	task->state = TS_STOPPED;
	task->next = current->next;
	current->next = task;
	task->state = TS_RUNABLE;
}

void insert_task(struct task_struct *task,int state){
	task->priority = INITIAL_PRIO;
	task->state = TS_STOPPED;
	task->next = current->next;
	current->next = task;
	task->state = state;
	//scheduler();

}


struct task_struct * remove_task(){
	struct task_struct *v = &TASK0;
	struct task_struct *t=NULL;
	for (; v; v = v->next) {
		if (v->next==current) {
			t=v->next;
			v->next=t->next;
			t->state=TS_STOPPED;
			break;
			//vesa_printf(100,520,"addr:%x state:%d p:%d",v,v->state,v->priority);
		}
	}
	return t;
}
void sleep_on(struct task_struct **p){
	//struct task_struct *tmp;
	if (!p)
		return;
	if (current == &(TASK0)){
		//panic("task[0] trying to sleep");
		return;
	}
	//mutex_lock(current);
	//tmp = *p;
	*p = current;
	//current->priority=999999999;
	current->state =TS_SLEEP;
	//scheduler();
	//mutex_unlock(current);
}
void wake_up(struct task_struct **p){
	if (p!=NULL&&(*p)!=NULL) {
		//mutex_lock(current);
		(**p).state=TS_RUNABLE;
		//mutex_unlock(current);
	}
}

struct task_struct *get_current_task(){
	return current;
}


void print_tasks(e32 x,e32 y){
	struct task_struct *t=&TASK0;
	int i;
	for(i=0;t!=NULL;t=t->next){
		vesa_printf(x,y+(i+1)*16,"                                              ");
		vesa_printf(x,y+i*16,"addr:%x state:%d p:%d    ",t,t->state,t->priority);
		i++;
	}
}

void test_task_op(){
	struct task_struct *t=remove_task();
	vesa_printf(00,80,"remove");
	print_tasks(00,100);
	vesa_printf(00,300,"addr:%x state:%d p:%d",t,t->state,t->priority);

	insert_task(t,TS_RUNABLE);
	vesa_printf(00,380,"insert");
	print_tasks(00,400);
}
