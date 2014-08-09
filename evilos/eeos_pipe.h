#ifndef _ETK_UCOS_PIPE_H
#define _ETK_UCOS_PIPE_H

#include "type.h"

#ifndef FD_SETSIZE
#define FD_SETSIZE      64
#endif /* FD_SETSIZE */

typedef struct fd_set {
        u32   fd_count;               /* how many are SET? */
        u32  fd_array[FD_SETSIZE];   /* an array of SOCKETs */
} fd_set;

typedef struct timeval {
	long   tv_sec;     /* seconds */
	long   tv_usec;    /* microseconds*/
}time_t;

#define FD_ZERO(set) (((fd_set *)(set))->fd_count=0)
#define FD_SET(fd, set) do { \
    if (((fd_set *)(set))->fd_count < FD_SETSIZE) \
        ((fd_set *)(set))->fd_array[((fd_set *)(set))->fd_count++]=(fd);\
} while(0)


int eeos_pipe_init(void);
int eeos_pipe_deinit(void);
int eeos_pipe_create(int fd[]);
int eeos_pipe_read(int fd, void *buf, int count);
int eeos_pipe_write(int fd, void *buf, int count);
int eeos_pipe_close(int fd);
int eeos_fd_select(fd_set *fdset, struct timeval *tv);
int eeos_fd_isset(int fd, fd_set *fdset);

#endif
