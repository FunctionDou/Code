
// 工作队列结构
struct workqueue_struct
{
  struct cpu_workqueue_struct cpu_wq[NR_CPUS];
  struct list_head list;
  const char *name;
  int sinqlethread;
  int freezeable;
  int rt;
};

// 工作队列的执行
for( ; ; )
{
  // TASK_INTERRUPTIBLE : 工作队列设置为等待
  prepare_to_wait(&cwq->more_work, &wait, TASK_INTERRUPTIBLE);

  // 如果工作队列是一个空链表, 那么就进行调度, 工作队列休眠
  if(list_empty(&cwq->worklist))
    shedule();

  // 工作队列链表非空, 就将其要执行的进行分离
  finish_wait(&cwq->more_work, &wait);
  // 执行上一步分离的工作.
  run_workqueue(cwq);
};

// run_workqueue() 的部分代码
while(!list_empty(&cwq->worklist))
{
  struct work_struct *work;
  work_func_t f;
  void *data;

  // 执行工作
  work = list_entry(cwq->worklist.next, struct work_struct, entry);
  f = work->func;

  // 清除执行了的工作
  list_del_init(cwq->worklist.next);
  work_clear_pending(work);

  // 重复执行
  f(work);
}