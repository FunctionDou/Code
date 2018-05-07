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
void init_poll(struct pollfd *);
void Client_write(int);
void insert(int , struct pollfd *);

void insert(int fd, struct pollfd *poll_fd)
{
    for(int i = 0; i < FD_SETSIZE; i++)
        if(poll_fd[i].fd == -1)
        {
            poll_fd[i].fd = fd;
            break;
        }
}

void init_poll(struct pollfd * poll_fd)
{
    for(int i = 0; i < FD_SETSIZE; i++)
    {
        poll_fd[i].fd = -1;
        poll_fd[i].events = POLLIN;
    }
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
    
    struct pollfd poll_fd[FD_SETSIZE];
    init_poll(poll_fd);
    poll_fd[0].fd = server_fd;
    poll_fd[1].fd = STDIN_FILENO;

    int n;
    char buf[N];
    while(1)
    {
        poll(poll_fd, FD_SETSIZE, -1);
        if(poll_fd[0].revents == POLLIN)
        {
            client_fd = accept(server_fd, (struct sockaddr *)&client_address, &sock_len);
            Write(STDOUT_FILENO, "accept\n");
            insert(client_fd, poll_fd);
        }

        for(int i = 2; i < FD_SETSIZE; i++)
        {
            
            if(poll_fd[1].revents == POLLIN)
            {
                n = read(STDIN_FILENO, buf, n);
                for(int j = 2; j < FD_SETSIZE; j++)
                    write(poll_fd[j].fd, buf, n);
            }
            else if(poll_fd[i].fd != -1 && poll_fd[i].revents == POLLIN)
            {
                n = read(poll_fd[i].fd, buf, sizeof(buf));
                if(n > 0)
                {
                    write(STDOUT_FILENO, buf, n);
                    write(poll_fd[i].fd, buf, n);
                }
                else if(n == 0)
                {
                    close(poll_fd[i].fd);
                    poll_fd[i].fd = -1;
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
            Write(STDOUT_FILENO, "connect\n");
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
