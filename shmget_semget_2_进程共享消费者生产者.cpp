#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <pthread.h>
#include <string.h>

#define mutex 0
#define cache 1
#define Process 2
#define addr 0x7888

int Create(int);
int *get_shmat_at();
void P(int);
void inti();
void V(int);
void D();


void D()
{
    int id = Create(0);
    semctl(id, 0, IPC_RMID, NULL);
    id = shmget(addr, 0, 0);
    shmctl(id, IPC_RMID, NULL);
}

void V(int count)
{
    int id = Create(0);
    int *cake = (int *)get_shmat_at();
    struct sembuf Pr = {Process, 1, 0};
    struct sembuf dis_cake = {cache, -1, 0};
    struct sembuf dis_mut = {mutex, -1, 0};
    struct sembuf mut = {mutex, 1, 0};

    while (1)
    {
        semop(id, &dis_cake, 1);
        semop(id, &dis_mut, 1);
        (*cake) -= count;
        fprintf(stdout, "V cake = %d\n", *cake);
        semop(id, &mut, 1);
        semop(id, &Pr, 1);
        sleep(1);
    }

    shmdt(cake);
}

void init()
{
    int id = Create(3);
    // id = get_shmat_at(id);
    // int sh = semget(0x8888, 4, IPC_CREAT | IPC_EXCL | 664);
    semctl(id, mutex, SETVAL, 1);
    semctl(id, cache, SETVAL, 0);
    semctl(id, Process, SETVAL, 5);

    id = shmget(addr, 4, IPC_CREAT | IPC_EXCL | 0664);
    int *cake = (int *)shmat(id, NULL, 0);
    *cake = 0;
    shmdt(cake);
}

void P(int count)
{
    int id = Create(0);
    int *cake = get_shmat_at();
    struct sembuf dis_p = {Process, -1, 0};
    struct sembuf full = {cache, 1, 0};
    struct sembuf dis_mut = {mutex, -1, 0};
    struct sembuf mut = {mutex, 1, 0};

    while (1)
    {
        semop(id, &dis_p, 1);
        semop(id, &dis_mut, 1);
        (*cake) += count;
        fprintf(stdout, "P cake = %d\n", *cake);
        semop(id, &mut, 1);
        semop(id, &full, 1);
        sleep(1);
    }
    shmdt(cake);
}

int Create(int ops)
{
    int id;
    if (ops > 0)
        id = semget(addr, ops, IPC_CREAT | IPC_EXCL | 0664);
    else
        id = semget(addr, 0, 0);
    return id;
}

int *get_shmat_at()
{
    int id = shmget(addr, 0, 0);
    int *cake = (int *)shmat(id, NULL, 0);

    return cake;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("less argv[]\n");
        exit(1);
    }
    char ch = argv[1][0];
    if(ch == 'p')
        P(1);
    else if(ch == 'v')
        V(1);
    else if(ch == 'i')
        init();
    else if(ch == 'd')
        D();


    exit(EXIT_SUCCESS);
}