#include "type.h"
#include "eeos_pipe.h"
#include "mutex.h"
#include "task.h"
#include "sem.h"

#define NR_PIPE				8
#define MIN_PIPE_NO			3
#define MAX_PIPE_NO			(MIN_PIPE_NO + NR_PIPE)
#define PIPE_HANDLE(fd)		((fd-MIN_PIPE_NO)>>0x01)
#define IS_VALID(fd)		(fd >= MIN_PIPE_NO && fd < MIN_PIPE_NO+(NR_PIPE<<1))
#define IS_RD_FD(fd)		((fd-MIN_PIPE_NO+1)&0x01)
#define IS_WR_FD(fd)		((fd-MIN_PIPE_NO+0)&0x01)
#define BIT_FLAG(fd)		(0x01<<PIPE_HANDLE(fd))
#define FALSE 0
#define TRUE  1
#define NO_ERR 1
#define FLAG_SET 1
#define FLAG_CLR 0
#define OS_TICKS_PER_SEC 1000
#define ETK_MALLOC(size) kmalloc(size)
#define ETK_FREE(addr) kfree(addr)


typedef struct _ByteQueue{
	u8 *p;
	mutex_t *mtx;
	e32 nbytes;
	e32 maxcount;
	e32 read_pos;
	e32 write_pos;
	e8 rd_opened;
	e8 wr_opened;
}ByteQueue;

typedef struct _PipeMgr{
	e8 inited;
	struct semaphore_t fg[NR_PIPE];
	ByteQueue bq[NR_PIPE];
}PipeMgr;

static PipeMgr g_mgr = {0xff};

#define PTROFBYTEQ(fd)	(&g_mgr.bq[PIPE_HANDLE(fd)])

int eeos_pipe_init(void){
	u8 err;
	e32 i;

	//vesa_printf("pipe_init\n");
	if(g_mgr.inited)
		return -1;

	memset(&g_mgr, 0, sizeof(PipeMgr));
	if(&g_mgr.fg == NULL)
		return -1;
	g_mgr.inited = TRUE;

	return 0;
}

int eeos_pipe_deinit(void){
	u8 err;
	e32 i;
	ByteQueue *bq;

	//OSFlagDel(g_mgr.fg, OS_DEL_ALWAYS, &err);
	for(i=0; i<NR_PIPE; i++){
		bq = &g_mgr.bq[i];
		if(bq->rd_opened || bq->wr_opened){
			//mutex_destroy(bq->mtx);
			if(bq->p) ETK_FREE(bq->p);
			memset(bq, 0, sizeof(ByteQueue));
			return 0;
		}
	}
	return 0;
}

int eeos_pipe_create(int fd[]){
	e32 i = 0;
	for(i=0; i<NR_PIPE; i++){
		if((!g_mgr.bq[i].rd_opened) && (!g_mgr.bq[i].wr_opened))
			break;
	}
	if(i == NR_PIPE){
		return -1;
	}
	memset(&g_mgr.bq[i], 0, sizeof(ByteQueue));
	g_mgr.bq[i].maxcount = 1024 * 4;
	g_mgr.bq[i].p =(u8*)ETK_MALLOC(g_mgr.bq[i].maxcount);
	if(g_mgr.bq[i].p == NULL){
		return -1;
	}
	g_mgr.bq[i].mtx = ETK_MALLOC(sizeof(mutex_t));
	if(g_mgr.bq[i].mtx == NULL){
		ETK_FREE(g_mgr.bq[i].p);
		return -1;
	}else{
		mutex_init(g_mgr.bq[i].mtx);
	}
	g_mgr.bq[i].rd_opened = TRUE;
	g_mgr.bq[i].wr_opened = TRUE;
	sem_init(&g_mgr.fg[i],1);
	fd[0] = (i<<1) + MIN_PIPE_NO + 0;
	fd[1] = (i<<1) + MIN_PIPE_NO + 1;
	//vesa_printf("eeos_pipe_create\n");
	return 0;
}

int eeos_pipe_read(int fd, void *buf, int count){
	u8 err = 0, opt = 0;
	e32 i = 0, j = 0;
	u32 readbytes = 0;
	u8 *p = NULL;
	ByteQueue *bq = NULL;
	u32 flags = 0;

	if((!IS_VALID(fd)) || (!IS_RD_FD(fd)))
		return 0;
	else if(buf == NULL || count ==0)
		return 0;
	bq = PTROFBYTEQ(fd);
	if(!bq->rd_opened)
		return 0;
	sem_wait(&g_mgr.fg[fd]);
	mutex_lock(bq->mtx);

	p = buf;
	for(i=bq->read_pos, j=0; i<bq->write_pos && j<count; i++, j++){
		p[j] = bq->p[i%bq->maxcount];
		readbytes++;
	}
	bq->read_pos += readbytes;
	bq->nbytes = bq->write_pos - bq->read_pos;
	opt = bq->nbytes > 0 ? FLAG_SET : FLAG_CLR;


	//sem_post(&g_mgr.fg[(fd)]);
	sem_post(&g_mgr.fg[(fd+1)]);

	mutex_unlock(bq->mtx);

	return readbytes;

}

int eeos_pipe_write(int fd, void *buf, int count){
	e32 i, j, emptys;
	e32 writebytes = 0;
	u8 *p = NULL;
	ByteQueue *bq = NULL;
	if((!IS_VALID(fd)) || (!IS_WR_FD(fd)))
		return 0;
	else if(buf == NULL || count ==0)
		return 0;
	bq = PTROFBYTEQ(fd);
	if(!bq->wr_opened)
		return 0;

	mutex_lock(bq->mtx);

	p = buf;
	emptys = bq->maxcount - bq->nbytes;
	for(i=bq->write_pos, j=0; j<count && j<emptys; i++, j++){
//		vesa_printf(100,100,"i:%d max:%d j:%d",i,bq->maxcount,j);
		vesa_printf(100,100,"bq->p:%x",bq->p);
		bq->p[i%(bq->maxcount)] = p[j];

		//int aa=p[j];
		writebytes++;
	}
	bq->write_pos += writebytes;
	bq->nbytes = bq->write_pos - bq->read_pos;
	sem_post(&g_mgr.fg[fd-1]);


	mutex_unlock(bq->mtx);

	return writebytes;
}

int eeos_pipe_close(int fd){
	ByteQueue *bq = NULL;
	if(!IS_VALID(fd))
		return -1;
	bq = PTROFBYTEQ(fd);
	if(IS_RD_FD(fd)){
		if(!bq->rd_opened)
			return -1;
		bq->rd_opened = FALSE;
	}else if(IS_WR_FD(fd)){
		if(!bq->wr_opened)
			return -1;
		bq->wr_opened = FALSE;
	}else{
		return -1;
	}
	if((!bq->rd_opened) && (!bq->wr_opened)){
		//mutex_destroy(bq->mtx);
		ETK_FREE(bq->p);
		memset(bq, 0, sizeof(ByteQueue));
		return 0;
	}
	return 0;
}

int eeos_fd_select(fd_set *fdset, struct timeval *tv){
	u8 err;
	u32 us = 0;
	e32 i = 0, n = 0;
	u32 flags = 0, curflags=0;

	for(i=0; i<fdset->fd_count; i++){
		flags |= BIT_FLAG(fdset->fd_array[i]);
		if(BIT_FLAG(fdset->fd_array[i])){
			sem_post(&g_mgr.fg[i]);
			curflags++;
		}
	}
	us = 0;
	if(tv != NULL){
		us = (u32)(tv->tv_sec*1000000 + tv->tv_usec);
		if(us > 0)
			us = (us / 1000) / (1000 / OS_TICKS_PER_SEC);
		else
			us = 1;
	}
	//do{
		//lcd_set_pos(100,150);
		//vesa_printf("dadada%d\n",rand()%100);
		//curflags = OSFlagPend(g_mgr.fg, flags, OS_FLAG_WAIT_SET_ANY, (INT16U)us, &err);
		//if(err == OS_TIMEOUT && us > 0){
		//	break;
		//}
		//vesa_printf("eeos_fd_select\n");
		//sem_wait(&g_mgr.fg[BIT_FLAG(flags)]);
	//}while(err != NO_ERR);
	//sem_wait(g_mgr.fg[])
	if(curflags>0){
		n =0 ;
		/*i = fdset->fd_count-1;
		i = i > 32u ? 32u : i;
		for(i=i; i>=0; i--)
		{
			if(curflags & (0x01<<i))
				n ++;
		}*/

		n++;
	}

	return curflags;
}

int eeos_fd_isset(int fd, fd_set *fdset){
	e32 i;
	e32 flags=0;
	if(fdset == NULL)
		return 0;
	for(i=0; i<fdset->fd_count&&i<32u; i++){
		if(fdset->fd_array[i] == fd)
			break;
	}
	if(i < fdset->fd_count&&i<32u ){
		//do{
			//flags = OSFlagQuery(g_mgr.fg, &err);
		//}while(err != NO_ERR);
//		int j;
//		for(j=0;j<NR_PIPE;j++){
//			flags=	g_mgr.fg[fd].count;
//			printf("flags=%d ",flags);
//		}
//		printf("\n");
		flags=	g_mgr.fg[fd].count;
//		printf("flags=%d ",flags);
		if(flags>=0 && BIT_FLAG(fd))
			return 1;
	}
	return 0;
}




static long ttt1_stack0[1024] = {0xf, };
static long ttt1_stack3[1024] = {0xf, };
static long ttt2_stack0[1024] = {0xf, };
static long ttt2_stack3[1024] = {0xf, };
struct task_struct ttt1;
struct task_struct ttt2;
static int fd_read = 0;
static int fd_write = 0;

void do_ttt1(){
	u32 i=0;
	char buf[6];
	int fd[2];
	fd_set fdset;
	eeos_pipe_init();
	if(eeos_pipe_create(fd))
		return ;
	fd_read = fd[0];
	fd_write = fd[1];
	printf("fd_read:%d fd_write:%d\n",fd[0],fd[1]	);

	for(;;){

       	vesa_printf(0,200,"-------pipe-r:%d \n",i++);
		if(i%100==0){
		if(fd_read > 0){
			FD_ZERO(&fdset);
			FD_SET(fd_read, (&fdset));
			vesa_printf(0,220,"waiting...\n");
			if(eeos_fd_select(&fdset, NULL)){
				//vesa_printf(100,200,"-------pipe-r:%d \n",i++);
				vesa_printf(0,240,"isset...\n");
				if(eeos_fd_isset(fd_read, &fdset)){
					vesa_printf(100,200,"-------pipe-r:%d \n",i++);
					vesa_printf(0,260,"task0 read...\n");
					if(4 == eeos_pipe_read(fd_read, buf, 4)){
						vesa_printf(0,280," read OK:%s\n",buf);
					}
					else
						vesa_printf(0,280,"Read Error\n");
				}
			}else{
				vesa_printf(0,220,"##waiting...\n");
			}
		}
}
		//sleep(1);
	}
}
void do_ttt2(){
	u32 i=0;
	char buf[6];
	for(;;){
		vesa_printf(0,300,"+++++++pipe-w:%d \n",i++);
		if(i%300==0){
 		if(fd_write > 0){
			memset(buf,0,sizeof(buf));
			vesa_printf(0,320,"task1 write:%s\n",buf);
			buf[0]='0'+i;
			if(5 == eeos_pipe_write(fd_write, buf, 5))
				vesa_printf(0,340," write OK\n");
			else
				vesa_printf(0,340,"Write Error\n");

		}
 		//sleep(3);
}
	}
}



void test_pipe(){
	new_task(&ttt1,
			(unsigned int)do_ttt1,
			(unsigned int)ttt1_stack0+1024,
			(unsigned int)ttt1_stack3+1024);
	new_task(&ttt2,
		(unsigned int)do_ttt2,
		(unsigned int)ttt2_stack0+1024,
		(unsigned int)ttt2_stack3+1024);
}

