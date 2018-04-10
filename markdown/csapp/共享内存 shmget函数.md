## 内核共享内存

---

### shmget

头文件 函数

```c
#include <sys/ipc.h>
#include <sys/shm.h>

// 得到一个共享内存标识符或创建一个共享内存对象并返回共享内存标识符
int shmget(key_t key, size_t size, int shmflg)
// 成功：返回共享内存的标识符; 出错：-1，错误原因存于error中
```

| **key**                            | 会建立新共享内存对象                                         |
| :--------------------------------- | ------------------------------------------------------------ |
|                                    |                                                              |
| **size**                           | 大于0的整数：新建的共享内存大小，以字节为单位                |
| **size = 0**                       | 只获取共享内存时指定为0                                      |
|                                    |                                                              |
| **shmflg**                         | **共享内存标识符**                                           |
| **shmflg = IPC_CREAT**             | 如果内核中不存在键值与key相等的共享内存，则新建一个共享内存；如果存在这样的共享内存，返回此共享内存的标识符 |
| **shmflg = IPC_CREAT \| IPC_EXCL** | 如果内核中不存在键值 与key相等的共享内存，则新建一个共享内存；如果存在这样的共享内存则报错 |
| **shmflg = 0**                     | **取共享内存标识符，若不存在则函数会报错**                   |



### shmat

```c
#include <sys/types.h>
#include <sys/shm.h>

// 连接共享内存标识符为shmid的共享内存，连接成功后把共享内存区对象映射到调用进程的地址空间，随后可像本地空间一样访问
void *shmat(int shmid, const void *shmaddr, int shmflg)
// 成功：附加好的共享内存地址;出错：-1，错误原因存于error中
```



| shmid       | **共享内存标识符**                                           |
| ----------- | ------------------------------------------------------------ |
| **shmaddr** | 指定共享内存出现在进程内存地址的什么位置，**直接指定为NULL让内核自己决定一个合适的地址位置** |
| **shmflg**  | **SHM_RDONLY：为只读模式，0 为读写模式**                     |

### shmdt

```c
#include <sys/types.h>
#include <sys/shm.h>

// 连接的共享内存
int shmdt(const void *shmaddr)
// 成功：0; 出错：-1，错误原因存于error中
```

| shmaddr | 连接的共享内存的起始地址 |
| ------- | ------------------------ |
|         |                          |

### shmctl

```c
#include <sys/types.h>
#include <sys/shm.h>

// 完成对共享内存的控制
int shmctl(int shmid, int cmd, struct shmid_ds *buf)
// 成功：0; 出错：-1，错误原因存于error中
```

| shmid              | 共享内存标识符                                               |
| ------------------ | ------------------------------------------------------------ |
| cmd =IPC_STAT      | 得到共享内存的状态，把共享内存的shmid_ds结构复制到buf中      |
| cmd = IPC_SET      | 改变共享内存的状态，把buf所指的shmid_ds结构中的uid、gid、mode复制到共享内存的shmid_ds结构内 |
| **cmd = IPC_RMID** | **删除这片共享内存**                                         |
| buf                | 共享内存管理结构体。具体说明参见共享内存内核结构定义部分     |

---

### 一个通信例子

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>

#define addr 0x8888

int get_id(int num)
{
    int id;
    if (num > 0)
    {
        // 第一次申请了64哥字节的空间
        id = shmget(addr, 64, IPC_CREAT | IPC_EXCL | 0664);
        // 把共享内存区映射到进程的地址空间
        char *s = (char *)shmat(id, NULL, 0);
        s = NULL;
        // 链接共享单元
        shmdt(s);
    }
    else
        // 共享单元已经创建的时侯, 进行连接就行, 不用创建了
        id = shmget(addr, 0, 0);

    return id;
}

int main(int argc, char *argv[])
{
    // argv[1] > 0 第一次, 建立共享空间, 并连接
    // argv[0] = 0 直接进行连接
    int id = get_id(atoi(argv[1]));
    char *s = (char *)shmat(id, 0, 0);
    char buf[1024];

    if (argv[2][0] == 'w')
    {
        int i;
        read(1, buf, sizeof(buf));
        i = 0;
        while (buf[i] != '\0')
            s[i] = buf[i++];
        s[i] = '\0';
    }
    else if (argv[2][0] == 'r')
        write(0, s, strlen(s));
    else if (argv[2][0] == 'd')
        shmctl(id, IPC_RMID, 0);

    exit(EXIT_SUCCESS);
}
```

运行截图

![018-04-08 23-59-00 的屏幕截](../../../图片/2018-04-08 23-59-00 的屏幕截图.png)

记住, 最后一定要用 <font color=#b20>**否则创建的共享区会一直在关机前都会存在, 不会释放**通过 **. ipcs查看共享空间块**</font>

```c
./aout 0 d
```



