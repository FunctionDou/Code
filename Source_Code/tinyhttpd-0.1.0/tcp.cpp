#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>

#define ISspace(x) isspace((int)(x))
typedef struct sockaddr *SA;

void error_perror(const char *);
int Listen(int sock_fd, unsigned short *port);
void *accept_request(void *);
int get_line(int, char *, int);
void File(int, const char *);
void sccess(int, const char *);
void cat(int, FILE *);

void cat(int client, FILE *fp)
{
    char buf[1024];
    fgets(buf, sizeof(buf), fp);
    while (!feof(fp))
    {
        send(client, buf, strlen(buf), 0);
        fgets(buf, sizeof(buf), fp);
    }
}

void sccess(int client, const char *filename)
{
    char buf[1204];
    (void)filename;

    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    send(client, buf, strlen(buf), 0);
    //send(client, buf, strlen(buf), 0);
    //sprintf(buf, "Content-Type: text/html\r\n");
    sprintf(buf, "Content-Type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
    strcpy(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
}

void File(int client, const char *path)
{
    char c;
    char buf[1024];
    int n = 1;      
    buf[0] = 'A';
    buf[1] = '\0';

    while (n > 0 && strcmp("\n", buf))
        n = get_line(client, buf, sizeof(buf));

    FILE *fp = NULL;
    if ((fp = fopen(path, "r")) == NULL)
        error_perror("FILE error");
    else
    {
        sccess(client, path);
        cat(client, fp);
    }
    fclose(fp);
}

int get_line(int client, char *buf, int size)
{
    char c = '\0';
    int i = 0;
    int n;

    while ((i < size - 1) && (c != '\n'))
    {
        n = recv(client, &c, 1, 0);
        if (n > 0)
        {
            if (c == '\r')
            {
                n = recv(client, &c, 1, MSG_PEEK);
                if (n > 0 && c == '\n')
                    recv(client, &c, 1, 0);
                else
                    c = '\n';
            }

            buf[i] = c;
            i++;
        }
        else
            c = '\n';
    }
    buf[i] = '\0';

    return i;
}

void error_perror(const char *s)
{
    errno = 1;
    perror(s);
    errno = 0;
    //write(STDOUT_FILENO, s, strlen(s));
}

void *accept_request(void *client)
{
    char buf[1024];
    char head[255];
    char url[512];
    char path[512];
    int n;
    int i, j;
    int cgi = 0;
    int Client = *(int *)client;

    n = get_line(Client, buf, sizeof(buf));
    printf("buf = %s\n", buf);
    i = 0;
    j = 0;
    while (!ISspace(buf[j]) && (i < sizeof(head) - 1))
    {
        head[i] = buf[j];
        i++;
        j++;
    }
    head[i] = '\0';
    printf("head = %s\n", head);

    if (strcasecmp(head, "GET") && strcasecmp(head, "POST"))
        error_perror("GET POST error");
    if (strcasecmp(head, "POST") == 0)
        cgi = 1;

    while (isspace(buf[j]) && (j < sizeof(buf) - 1))
        j++;

    i = 0;
    while (!isspace(buf[j]) && (j < sizeof(buf) - 1))
    {
        url[i] = buf[j];
        i++;
        j++;
    }
    url[i] = '\0';

    char *ptr = NULL;
    if (strcasecmp(head, "GET") == 0)
    {
        ptr = url;
        while (*ptr != '\0' && *ptr != '?')
            ptr++;
        if (*ptr == '?')
        {
            cgi = 1;
            *ptr = '\0';
            ptr++;
        }
    }

    printf("url = %s\n", url);
    sprintf(path, "htdocs%s", url);
    if (path[strlen(path) - 1] == '/')
        strcat(path, "index.html");

    printf("path = %s\n", path);

    struct stat st;
    if (stat(path, &st) == -1)
    {
        while (n > 0 && strcmp("\n", buf))
            n = get_line(Client, buf, sizeof(buf));
        error_perror("stat error");
    }
    else
    {
        if ((st.st_mode & S_IFMT) == S_IFDIR)
            strcat(path, "/index.html");
        if ((st.st_mode & S_IXGRP) || (st.st_mode & S_IXUSR) || (st.st_mode & S_IXOTH))
            cgi = 1;

        if (!cgi)
            File(Client, path);
        else
            ;
    }

    close(Client);

    pthread_exit((void *)0);
}

int Listen(int sock_fd, unsigned short *port)
{
    struct sockaddr_in sock_in;
    socklen_t socklen;
    bzero(&sock_in, socklen);
    sock_in.sin_family = AF_INET;
    sock_in.sin_port = htons(*port);
    //sock_in.sin_port = *port;
    sock_in.sin_addr.s_addr = htonl(INADDR_ANY);
    //sock_in.sin_addr.s_addr = inet_addr("127.0.0.1");
    socklen = sizeof(sock_in);

    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        error_perror("socket error");
    if (bind(sock_fd, (SA)&sock_in, socklen) < 0)
        error_perror("bind error");
    if (*port == 0)
    {
        if (getsockname(sock_fd, (SA)&sock_in, &socklen) == -1)
            error_perror("getsockname error");
        *port = ntohs(sock_in.sin_port);
    }
    if (listen(sock_fd, 5) < 0)
        error_perror("listen error");

    return sock_fd;
}

int main(void)
{
    pthread_t thread;
    int sock_fd, client;
    unsigned short port = 0;
    struct sockaddr_in sock_in;
    socklen_t socklen = sizeof(sock_in);
    sock_fd = Listen(sock_fd, &port);

    printf("port = %d\n", port);
    while (1)
    {
        if ((client = accept(sock_fd, (SA)&sock_in, &socklen)) == -1)
            error_perror("accept strlen(path) error");
        printf("client = %d\n", client);
        if (pthread_create(&thread, NULL, accept_request, (void *)&client) != 0)
            error_perror("phread_create error");
    }

    close(sock_fd);
    exit(0);
}
