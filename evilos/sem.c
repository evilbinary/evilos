#include <sem.h>
#include <asm.h>
#include <mutex.h>
#include <vesa.h>
#include <kmalloc.h>
#include <task.h>


void print_waitq(e32 x,e32 y,struct taskqueue_t *lwq){
	struct task_t* item;
	int i=0;
	SIMPLEQ_FOREACH(item,lwq,entries){
		vesa_printf(x,y+i*16,"addr:%x task:%x",item,item->task);
		i++;
	}

}
void sleep_on_queue(struct taskqueue_t *lwq){

//  struct task_struct *t=mem_malloc(sizeof(struct task_struct));
   /* struct task_struct *t=remove_task();;
    if(t!=NULL){
    	struct task_t* item=mem_malloc(sizeof(struct task_t));
    	item->data=100;
    	item->task=t;
    	t->state = TS_WAITING;
    	SIMPLEQ_INSERT_TAIL(lwq,item,entries);
	}*/

	//t->state = TS_SLEEP;

	struct task_t* item=kmalloc(sizeof(struct task_t));
	if(item!=NULL){
		struct task_struct *t=get_current_task();
		if(t->state==TS_SLEEP)
			return;
		item->task=t;
		item->data=100;
		t->state = TS_SLEEP;
		struct task_t  *it=SIMPLEQ_FIRST(lwq);
		if(it==NULL){
			SIMPLEQ_INSERT_TAIL(lwq,item,entries);
		}else{
			if(it->task==t){
				kfree(t);
			}else{
				SIMPLEQ_INSERT_TAIL(lwq,item,entries);
			}
		}
		sleep_on(&t);

		scheduler();
	}else{
		e32 i;
		vesa_printf(100,80,"file:%s,line:%s,fun:%s %s",__FILE__,__LINE__,__FUNCTION__,"malloc failed");
		for(i=99999;i>0;i--){}
	}

}



void wake_up_queue(struct taskqueue_t *lwq){
    /*struct task_t  *item=SIMPLEQ_FIRST(lwq);
    //vesa_printf(100,80,"%s %x","wake_up",item->task);
    if(item!=NULL){
		if(item->task!=NULL){
			insert_task(item->task,TS_RUNABLE);
			//scheduler();
		}
		SIMPLEQ_REMOVE_HEAD(lwq,entries);
		mem_free(item);
	}*/
	struct task_t  *item=SIMPLEQ_FIRST(lwq);
    if(item!=NULL){
		if(item->task!=NULL){
			//item->task->state=TS_RUNABLE;
			//item->task->priority = INITIAL_PRIO;
			//item->task->state = TS_STOPPED;
			wake_up(&item->task);
			//scheduler();
		}
		SIMPLEQ_REMOVE_HEAD(lwq,entries);
		kfree(item);
	}

 }

void sem_init(struct semaphore_t* sem,int val){
	SIMPLEQ_INIT(&sem->wq);
	mutex_init(&sem->mutex);
	sem->count=val;
}

void sem_wait(struct semaphore_t* sem){
	//printf("FILE:%s LINE:%d FUNC:%s\n",__FILE__,__LINE__,__FUNCTION__);

	//kprintf("FILE:%s LINE:%d FUNC:%s\n",__FILE__,__LINE__,__FUNCTION__);
	mutex_lock(&sem->mutex);
	sem->count--;

	//critical_enter();
	//vesa_printf(100,80,"%s %d","sem_wait",sem->count);
    if((sem->count)<0){
    	//vesa_printf(100,100,"%s %d","sleep_on",sem->count);
    	//__asm__("xchgw %bx,%bx");
        sleep_on_queue(&sem->wq);
    }
    mutex_unlock(&sem->mutex);
    //scheduler();
    //critical_exit();
	//printf("FILE:%s LINE:%d FUNC:%s\n",__FILE__,__LINE__,__FUNCTION__);
}
void sem_post(struct semaphore_t* sem){
	//printf("FILE:%s LINE:%d FUNC:%s\n",__FILE__,__LINE__,__FUNCTION__);
	mutex_lock(&sem->mutex);
    sem->count++;

	//critical_enter();
    if((sem->count)<=0){
    	//vesa_printf(100,100,"%s %d","wake_up",sem->count);
    	//printf("%s %d","wake_up",sem->count);
    	wake_up_queue(&sem->wq);
    }
     mutex_unlock(&sem->mutex);
    //critical_exit();
	//printf("FILE:%s LINE:%d FUNC:%s\n",__FILE__,__LINE__,__FUNCTION__);
}


struct semaphore_t t_sem;

static long tt1_stack0[1024] = {0xf, };
static long tt1_stack3[1024] = {0xf, };
static long tt2_stack0[1024] = {0xf, };
static long tt2_stack3[1024] = {0xf, };
struct task_struct tt1;
struct task_struct tt2;


void do_t1(){
	u32 i=0;

	for(;;){
		u32 *p=0xfff000;
       *p=i;
       	//vesa_printf(0,200,"t1:%d cout:%d\n",i++,t_sem.count);
		printf("t1:%d cout:%d \n",i++,t_sem.count);

       //if(i%1000==0)
	   	sem_wait(&t_sem);
	}
}
void do_t2(){
	e32 i=0;
	for(;;){
		u32 *p=0xfff000;
		//vesa_printf(0,240,"t2:%d cout:%d\n",i++,t_sem.count);
		printf("t2:%d cout:%d \n",i++,t_sem.count);
		if(i%10000==0){
			//vesa_printf(0,220,"+++++++++++++++++");
			//print_waitq(0,220,&t_sem.wq);
			//print_waitq(0,260,&t_sem.wq);
			sem_post(&t_sem);

		}


	}
}
void sem_test(){
	/*struct task_struct *t1=mem_malloc(sizeof(struct task_struct));
	char *t1_stack0=mem_malloc(1024);
	char *t1_stack3=mem_malloc(1024);
	struct task_struct *t2=mem_malloc(sizeof(struct task_struct));
	char *t2_stack0=mem_malloc(1024);
	char *t2_stack3=mem_malloc(1024);
	*/
	//sem=mem_malloc(sizeof(struct semaphore_t));
	sem_init(&t_sem,1);


	new_task(&tt1,
			(unsigned int)do_t1,
			(unsigned int)tt1_stack0+1024,
			(unsigned int)tt1_stack3+1024);
	new_task(&tt2,
		(unsigned int)do_t2,
		(unsigned int)tt2_stack0+1024,
		(unsigned int)tt2_stack3+1024);

}
struct semaphore_t s1;
struct semaphore_t s2;
void do_tt1(){
	u32 i=0;
	for(;;){
		//if(i%100==0){
			printf("t1:%d s1:%d s2:%d ",i++,s1.count,s2.count);
			sem_wait(&s1);
//       	vesa_printf(0,100,"t1:%d s1:%d s2:%d ",i++,s1.count,s2.count);
//			print_waitq(0,120,&s1.wq);
//			print_waitq(0,140,&s2.wq);
//__asm__("xchgw %bx,%bx");
	    	sem_post(&s2);
		//}
	}
}
void do_tt2(){
	u32 i=0;
	for(;;){
		//if(i%200==0){
		printf("t2:%d s1:%d s2:%d ",i++,s1.count,s2.count);
		sem_wait(&s2);
//		vesa_printf(0,160,"t2:%d s1:%d s2:%d ",i++,s1.count,s2.count);
//		print_waitq(0,180,&s1.wq);
//		print_waitq(0,200,&s2.wq);
//__asm__("xchgw %bx,%bx");
		sem_post(&s1);
		//}
	}
}

void sem_test1(){
	sem_init(&s1,1);
	sem_init(&s2,0);
	new_task(&tt1,
			(unsigned int)do_tt1,
			(unsigned int)tt1_stack0+1024,
			(unsigned int)tt1_stack3+1024);
	new_task(&tt2,
		(unsigned int)do_tt2,
		(unsigned int)tt2_stack0+1024,
		(unsigned int)tt2_stack3+1024);
}


