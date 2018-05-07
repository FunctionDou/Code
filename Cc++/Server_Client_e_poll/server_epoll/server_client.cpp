#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <poll.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <string.h>

#define N 64

int Write(int , char *);
void Server(unsigned short, char *);
void Client(unsigned short, char *);
int Listen(unsigned short *port);
void insert(int , int *);
void Client_write(int);

void insert(int fd, int * epoll_fd)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    epoll_ctl(*epoll_fd, EPOLL_CTL_ADD, fd, &event);
}

int Listen(unsigned short *port, char *ip)
{
    struct sockaddr_in server_addr;
    socklen_t sock_len;
    sock_len = sizeof(server_addr);
    int server_fd;

    memset((char *)&server_addr, 0, sock_len);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(*port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(bind(server_fd, (struct sockaddr *)&server_addr, sock_len))
        exit(EXIT_FAILURE);
    if(listen(server_fd, 5) < 0)
        exit(EXIT_FAILURE);

    return server_fd;
}

int Write(int fd, char *s)
{
    int n;
    n = write(fd, s, strlen(s));

    return n;
}

void Server(unsigned short port, char *ip)
{
    int server_fd, client_fd;
    struct sockaddr_in client_address;
    socklen_t sock_len;
    memset((char *)&client_address, 0, sizeof(sock_len));

    sock_len = sizeof(client_address);

    server_fd = Listen(&port, ip);

    int epoll_fd;
    epoll_fd = epoll_create(1);
    insert(server_fd, &epoll_fd);
    insert(STDOUT_FILENO, &epoll_fd);

    struct epoll_event events[FD_SETSIZE];

    char buf[N];
    int n;
    int num;
    while(1)
    {
        num = epoll_wait(epoll_fd, events, FD_SETSIZE, -1);

        if(num == 0)
            continue;

        for(int i = 0 ; i < num; i++)
        {
            if(events[i].data.fd == server_fd && events[0].events & EPOLLIN)
            {
                client_fd = accept(server_fd, (struct sockaddr *)&client_address, &sock_len);
                insert(client_fd, &epoll_fd);
                Write(STDOUT_FILENO, "accept\n");
            }

            else if(events[i].events & EPOLLIN)
            {
                n = read(events[i].data.fd, buf, sizeof(buf));
                if(n > 0)
                {
                    write(STDOUT_FILENO, buf, n);
                    write(events[i].data.fd, buf, n);
                }
                else if(n == 0)
                {
                    close(events[i].data.fd);
                    break;
                }
            }
        }
    }
}

void Client(unsigned short port, char *ip)
{
    struct sockaddr_in client_addr;
    socklen_t sock_len;
    sock_len = sizeof(client_addr);
    int client_fd;

    memset((char *)&client_addr, 0, sock_len);
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(port);
    client_addr.sin_addr.s_addr = inet_addr(ip);

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(connect(client_fd, (struct sockaddr *)&client_addr, sock_len) == -1)
        exit(EXIT_FAILURE);

    Client_write(client_fd);

    close(client_fd);
}

void Client_write(int client_fd)
{
    fd_set FD, fd_temp;
    FD_ZERO(&FD);
    FD_SET(STDIN_FILENO, &FD);
    FD_SET(client_fd, &FD);

    int max;
    int n;
    char buf[N];
    max = client_fd > STDIN_FILENO ? client_fd + 1 : STDIN_FILENO + 1;
    while(1)
    {
        fd_temp = FD;
        select(max, &fd_temp, NULL, NULL, NULL);

        if(FD_ISSET(STDIN_FILENO, &fd_temp))
        {
            n = read(STDIN_FILENO, buf, sizeof(buf));
            if(n > 0)
                write(client_fd, buf, n);
            else if(n == 0)
            {
                shutdown(client_fd, SHUT_WR);
            }   
        }
        else if(FD_ISSET(client_fd, &fd_temp))
        {
            n = read(client_fd, buf, sizeof(buf));
            if(n > 0)
                write(STDOUT_FILENO, buf, n);
            else if(n == 0)
            {
                FD_CLR(client_fd, &FD);
                break;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if(argv[1][0] == 's')
        Server(atoi(argv[2]), argv[3]);
    else if(argv[1][0] == 'c')
        Client(atoi(argv[2]), argv[3]);

    exit(EXIT_SUCCESS);
}
