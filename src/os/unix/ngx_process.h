
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PROCESS_H_INCLUDED_
#define _NGX_PROCESS_H_INCLUDED_


#include <ngx_setaffinity.h>
#include <ngx_setproctitle.h>


typedef pid_t       ngx_pid_t;

#define NGX_INVALID_PID  -1

typedef void (*ngx_spawn_proc_pt) (ngx_cycle_t *cycle, void *data);

typedef struct {
    //进程ID
    ngx_pid_t           pid;

    //由waitpid系统调用获取到的进程状态
    int                 status;
    /*
     *  由socketpair系统调用产生出的用于进程间通信的socket句柄
     *  用于master与worker进程间的通信
     */
    ngx_socket_t        channel[2];

    //子进程的循环执行方法，当父进程调用ngx_spawn_process生成子进程时使用
    ngx_spawn_proc_pt   proc;
    void               *data;
    //进程名称，操作系统中显示的进程名称与name相同
    char               *name;

    //标志位，为1时表示在重新生成子进程
    unsigned            respawn:1;
    //标志位，为1时表示正在生成子进程
    unsigned            just_spawn:1;
    //标志位，为1时表示在进行父、子进程分离
    unsigned            detached:1;
    //标志位，为1时表示进程正在退出
    unsigned            exiting:1;
    //标志位，为1时表示进程已经退出
    unsigned            exited:1;
} ngx_process_t;


typedef struct {
    char         *path;
    char         *name;
    char *const  *argv;
    char *const  *envp;
} ngx_exec_ctx_t;


/*
 *  定义1024个元素的ngx_processes数组，也就是最多只能有1024个子进程
 */
#define NGX_MAX_PROCESSES         1024

#define NGX_PROCESS_NORESPAWN     -1
#define NGX_PROCESS_JUST_SPAWN    -2
#define NGX_PROCESS_RESPAWN       -3
#define NGX_PROCESS_JUST_RESPAWN  -4
#define NGX_PROCESS_DETACHED      -5

//pid_t getpid(void)，存在于unistd.h中，返回当前进程id
#define ngx_getpid   getpid

#ifndef ngx_log_pid
#define ngx_log_pid  ngx_pid
#endif


ngx_pid_t ngx_spawn_process(ngx_cycle_t *cycle,
    ngx_spawn_proc_pt proc, void *data, char *name, ngx_int_t respawn);
ngx_pid_t ngx_execute(ngx_cycle_t *cycle, ngx_exec_ctx_t *ctx);
ngx_int_t ngx_init_signals(ngx_log_t *log);
void ngx_debug_point(void);


#if (NGX_HAVE_SCHED_YIELD)
#define ngx_sched_yield()  sched_yield()
#else
#define ngx_sched_yield()  usleep(1)
#endif


extern int            ngx_argc;
extern char         **ngx_argv;
extern char         **ngx_os_argv;

extern ngx_pid_t      ngx_pid;
extern ngx_socket_t   ngx_channel;
extern ngx_int_t      ngx_process_slot;
extern ngx_int_t      ngx_last_process;
extern ngx_process_t  ngx_processes[NGX_MAX_PROCESSES];


#endif /* _NGX_PROCESS_H_INCLUDED_ */
