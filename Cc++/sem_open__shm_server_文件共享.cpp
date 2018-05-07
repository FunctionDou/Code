/*************************************************************************
    > File Name: sem_open__shm_server_文件共享.cpp
    > Author: Function_Dou
    > Mail: NOT 1
    > Created Time: 2018年03月23日 星期五 09时09分40秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>

int main()
{
	key_t key;
	int shm;
	char *s, *head;
	key = 1000;
	sem_t *sem;

	sem = sem_open("LLL", O_CREAT, 0644, 1);
	shm = shmget(key, 27, IPC_CREAT | 0666);
	s = (char *)shmat(shm, NULL, 0);
	head = s;
	sem_wait(sem);
	char ch;
	while((ch = getchar()) != '\n')
		*head++ = ch;
	*head = '#';
	sem_post(sem);

	while(*s != '*')
		sleep(1);
	sem_close(sem);
	sem_unlink("LLL");

	shmctl(shm, IPC_RMID, 0);
	


	exit(0);
}
