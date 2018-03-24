#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <errno.h>
//#include <sys/in.h>

typedef struct sockaddr *SA;

int Listen(int, unsigned short *);
void error_print(const char *);
void *accept_request(void *);
int get_line(int, char *, int);
void Print(int, const char *);
void sccess(int);
void Execl(int, const char *, const char *, char *);

void Execl(int client, const char *path, const char *head, char *ptr)
{
    int input[2];
    int output[2];
    pid_t pid;
    char buf[1024];
    int num = 1;
    int len = -1;
    int i;
    int status;

    buf[0] = 'A';
    buf[1] = '\0';
    if (strcasecmp(head, "GET") == 0)
        while(num > 0 && strcmp("\n", buf))
            num = get_line(client, buf, sizeof(buf));
    else
    {
        num = get_line(client, buf, sizeof(buf));

        while(num > 0 && strcmp("\n", buf))
        {
            buf[15] = '\0';

            printf("%s\n", buf);

            if(strcasecmp(buf, "Content-Length:") == 0)
                len = atoi(&(buf[16]));

            num = get_line(client, buf, sizeof(buf));
        }
        printf("len = %d\n", len);
        if(len == -1)
        {
            error_print("Content_Length error\n");
            return;
        }
    }

    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    send(client, buf, strlen(buf), 0);

    if (pipe(input) < 0 || pipe(output) < 0)
        error_print("pipe error\n");
    if ((pid = fork()) < 0)
        error_print("fork error\n");
    if (pid == 0)
    {
        char meth_env[255];
        char query_env[255];
        char length_env[255];

        dup2(input[0], 0);
        dup2(output[1], 1);
        close(input[1]);
        close(output[0]);

        sprintf(meth_env, "REQUEST_METHOD=%s", head);
        putenv(meth_env);

        if(strcasecmp(head, "GET") == 0)
        {
            sprintf(query_env, "QUERY_STRING=%s", ptr);
            putenv(query_env);
        }
        else
        {
            sprintf(length_env, "CONTENT_LENGTH=%d", len);
            putenv(length_env);
        }

        execl(path, path, NULL);

        exit(0);
    }
    else
    {
        close(input[0]);
        close(output[1]);

        char c;
        if (strcasecmp(head, "POST") == 0)
            for (i = 0; i < len; i++)
            {
                recv(client, &c, 1, 0);
                write(input[1], &c, 1);
            }
        while(read(output[0], &c, 1) > 0)
            send(client, &c, 1, 0);

        close(input[1]);
        close(output[0]);
        waitpid(pid, &status, 0);
    }
}

void sccess(int client)
{
    char buf[1024];
    int n;
    n = sprintf(buf, "HTTP/1.0 200 OK\r\n");
    send(client, buf, n, 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
    n = sprintf(buf, "\r\n");
    send(client, buf, n, 0);
}

void Print(int client, const char *path)
{
    FILE *fp = NULL;

    if ((fp = fopen(path, "r")) == NULL)
        error_print("fopen error\n");

    int n = 1;
    char buf[1024];
    buf[0] = 'A';
    buf[1] = '\0';
    while (n > 0 && strcmp("\n", buf))
        n = get_line(client, buf, sizeof(buf));

    sccess(client);

    fgets(buf, sizeof(buf), fp);
    while (!feof(fp))
    {
        send(client, buf, strlen(buf), 0);
        fgets(buf, sizeof(buf), fp);
    }

    fclose(fp);
}

int get_line(int client, char *buf, int size)
{
    int i = 0;
    int n;
    char c = '\0';

    while ((i < size - 1) && (c != '\n'))
    {
        n = recv(client, &c, 1, 0);
        if (n > 0)
        {
            if (c == '\r')
            {
                n = recv(client, &c, 1, MSG_PEEK);
                if (c == '\n' && n > 0)
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

void *accept_request(void *sock_fd)
{
    int client = *(int *)sock_fd;
    char buf[1024];
    char head[255];
    char url[512];
    char path[512];
    int cgi = 0;
    int num = 0;

    num = get_line(client, buf, sizeof(buf));

    printf("buf = %s\n", buf);
    int i, j;
    i = j = 0;
    while (!isspace(buf[j]) && (i < sizeof(head) - 1))
    {
        head[i] = buf[j];
        i++;
        j++;
    }
    head[i] = '\0';

    printf("head = %s\n", head);

    while (isspace(buf[j]) && j < sizeof(buf))
        j++;
    i = 0;
    while (!isspace(buf[j]) && (i < sizeof(url) - 1))
    {
        url[i] = buf[j];
        i++;
        j++;
    }
    url[i] = '\0';

    if (strcasecmp(head, "GET") && strcasecmp(head, "POST"))
        error_print("head error\n");
    if (strcasecmp(head, "POST") == 0)
        cgi = 1;

    char *ptr = NULL;
    if (strcasecmp(head, "GET") == 0)
    {
        ptr = url;
        while (*ptr != '\0' && *ptr != '\n' && *ptr != '?')
            ptr++;
        if (*ptr == '?')
        {
            cgi = 1;
            *ptr = '\0';
            ptr++;
        }
    }

    int n;
    n = sprintf(path, "htdocs%s", url);
    if (path[n - 1] == '/')
        strcat(path, "index.html");
    struct stat st;
    if (stat(path, &st) == -1)
    {
        while (num > 0 && strcmp("\n", buf))
            num = get_line(client, buf, sizeof(buf));
    }
    else
    {
        if ((st.st_mode & S_IFMT) == S_IFDIR)
            strcat(path, "/index.html");
        if ((st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH) || (st.st_mode & S_IXUSR))
            cgi = 1;
        if (!cgi)
            Print(client, path);
        else
            Execl(client, path, head, ptr);
    }
    pthread_exit((void *)0);
}

void error_print(const char *s)
{
    write(STDOUT_FILENO, s, strlen(s));
}

int Listen(int sock_fd, unsigned short *port)
{
    struct sockaddr_in sock_in;
    socklen_t socklen;

    socklen = sizeof(sock_in);
    bzero(&sock_in, socklen);
    sock_in.sin_family = AF_INET;
    sock_in.sin_port = htons(*port);
    sock_in.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        error_print("socket error\n");
    if (bind(sock_fd, (SA)&sock_in, socklen) == -1)
        error_print("bind error\n");

    if (*port == 0)
    {
        if (getsockname(sock_fd, (SA)&sock_in, &socklen) == -1)
            error_print("getsockname error\n");
        *port = ntohs(sock_in.sin_port);
    }

    if (listen(sock_fd, 5) < 0)
        error_print("listen error\n");

    return sock_fd;
}

int main(void)
{
    int sock_fd, client;
    struct sockaddr_in sock_in;
    socklen_t socklen;
    unsigned short port = 0;
    pthread_t thread;

    socklen = sizeof(sock_in);
    sock_fd = Listen(sock_fd, &port);

    printf("port = %d\n", port);
    while (1)
    {
        if ((client = accept(sock_fd, (SA)&sock_in, &socklen)) == -1)
            error_print("accept error");
        printf("%d\n", client);
        if (pthread_create(&thread, NULL, accept_request, (void *)&client) != 0)
            error_print("pthread_create error\n");
    }

    close(sock_fd);
    exit(0);
}
