/*************************************************************************
    > File Name: sem_open__shm_child_文件共享
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年03月23日 星期五 09时40分49秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>

int main()
{
	key_t key;
	int shm;
	char *s;
	sem_t *sem;

	// 创建一个新的 LLL 文件, 权限为 0644, 一个初始化信号量为 1
	sem = sem_open("LLL", O_CREAT, 0644, 1);
	key = 1000;
	// key 设置或创建共享内存关键字.
	// 27 : 共享内存的大小
	// 0655 : 权限位的设置
	shm = shmget(key, 27, 0655);
	// shm : 共享内存关键字,
	// NULL : 地址空间
	// 0 : 特殊位标志
	s = (char *)shmat(shm, NULL, 0);

	char *head = s;

	sem_wait(sem);
	while (*head != '#')
		putchar(*head++);
	sem_post(sem);

	*s = '*';

	sem_close(sem);
	shmctl(shm, IPC_RMID, 0);

	exit(0);
}
