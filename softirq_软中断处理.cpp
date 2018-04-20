// 软中断描述符
struct softirq_action
{ 
    void (*action)(struct softirq_action *);
};

u32 pending;
// 获取软中断
pending = local_softirq_pending();

// 如果有软中断, pending就不为0
if(pending)
{
    struct softirq_action *h;

    // 清除当前的软中断, 当前中断已经保存在pending中了
    set_softirq_pending(0);

    // h 指向当前softirq_vec 的第一项, 也就是第一个向量号
    h = softirq_vec;
    do{
        // 向量号为 1, 代表次信号开启, 需要被执行
        if(pending & 1)
            // action 执行当前向量号的中断
            h->action(h);
        
        /* 下面就是移动至下一个向量号, 直到 pending 没有要处理的中断信号了 */
        h++;
        pending >>= 1;
    }while(pending);
}

// tasklet
struct tasklet_struct
{
      //将多个tasklet链接成单向循环链表
      struct tasklet_struct *next;
      //TASKLET_STATE_SCHED(Tasklet is scheduled for execution)  TASKLET_STATE_RUN(Tasklet is running (SMP only))
      unsigned long state;
      //0:激活tasklet 非0:禁用tasklet
      atomic_t count;
      //用户自定义函数
      void (*func)(unsigned long); 
      // 函数入参
      unsigned long data; 
};

// 管道的结构
struct pipe_inode_info
{
    // 管道/FIFO的等待队列
    struct wait_queue* wait;
    // 已经写了多少个缓冲区了(最多16个, 是一个缓冲数组)
    unsigned int nrbufs;
    // 包含将读数据的第一个缓冲区的索引 
    unsigned int curbuf;
    // 管道缓冲区描述符数组
    struct pipe_buffer[16] bufs;
    // 高速缓存页框指针
    struct page * tmp_page;
    // 读进程标志
    unsigned int readers;
    // 写进程标志
    unsigned int writers;
};

i_sem // 信号量, 防止管道间竞争
