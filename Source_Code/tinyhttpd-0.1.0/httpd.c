/* J. David's webserver */
/* This is a simple webserver.
 * Created November 1999 by J. David Blackstone.
 * CSE 4344 (Network concepts), Prof. Zeigler
 * University of Texas at Arlington
 */
/* This program compiles for Sparc Solaris 2.6.
 * To compile for Linux:
 *  1) Comment out the #include <pthread.h> line.
 *  2) Comment out the line that defines the variable newthread.
 *  3) Comment out the two lines that run pthread_create().
 *  4) Uncomment the line that runs accept_request().
 *  5) Remove -lsocket from the Makefile.
 */
#include <stdio.h>
#include <sys/socket.h> // 套接字
#include <sys/types.h>
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h>  // IPV4
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h> // 文件操作
#include <pthread.h>  // 线程
#include <sys/wait.h> // wait
#include <stdlib.h>

// 判断是否为空字符串(如 : "\t", " ", "\n等等")
#define ISspace(x) isspace((int)(x))

#define SERVER_STRING "Server: jdbhttpd/0.1.0\r\n"

void *accept_request(void *);
// 400 的网页错误
void bad_request(int);
// 将数据输入到套接字里面
void cat(int, FILE *);
// 500 的网页错误
void cannot_execute(int);
// perror 报错
void error_die(const char *);
// 初始化管道, 并且创建新的进程, 子进程执行新的可执行文件
void execute_cgi(int, const char *, const char *, const char *);
// 获取从套接字里传入的参数
int get_line(int, char *, int);
// 输出 HTTP/1.0 200 OK
void headers(int, const char *);
// 输出 网页404消息
void not_found(int);
// 打开文件, 并将文件里面的数据写到套接字里面
void serve_file(int, const char *);
// 创建套接字(bind, listen), 并将套接字返回
int startup(u_short *);
// 向套接字里面传递无法处理的 html 信息
void unimplemented(int);

/**********************************************************************/
/* A request has caused a call to accept() on the server port to
 * return.  Process the request appropriately.
 * Parameters: the socket connected to the client */
/**********************************************************************/
void *accept_request(void *client)
{
	int Cline = *(int *)client;
	char buf[1024];
	int numchars;
	char method[255];
	char url[255];
	char path[512];
	size_t i, j;
	struct stat st; // 获取文件信息
	int cgi = 0;	/* becomes true if server decides this is a CGI
                    * program */
	//
	char *query_string = NULL;

	// 获取从套接write(STDOUT_FILENO, buf, sizeof(buf));字里传入的参数
	numchars = get_line(Cline, buf, sizeof(buf));
	i = 0;
	j = 0;

	write(STDOUT_FILENO, buf, strlen(buf));

	// 判断不是(" ", '\t', '\r', '\n')空字符, 并且长度没有超过 method 的 len-1
	while (!ISspace(buf[j]) && (i < sizeof(method) - 1))
	{
		method[i] = buf[j];
		i++;
		j++;
	}
	method[i] = '\0';

	printf("method = %s\n", method);
	// int strcasecmpIconst char *s1, const char *s)
	// strcasecmp : 比较两个字符串, 相等返回 0; s1 > s2 返回 大于0; s1 < s2 : 返回 负数.
	// 如果不是 GET 或者也不是 POST 则无法处理
	if (strcasecmp(method, "GET") && strcasecmp(method, "POST"))
	{
		// 向套接字里面传递 html 的无法处理信息
		//unimplemented(Cline);
		return ((void *)0);
	}

	// POST
	if (strcasecmp(method, "POST") == 0)
		cgi = 1;

	i = 0;
	// buf[j] == '\r','\t','\n' 时, 使其 ++, 继续下一个串
	while (ISspace(buf[j]) && (j < sizeof(buf)))
		j++;
	// 将 buf 中的另一个字符串的内容复制给 url 数组中保存
	while (!ISspace(buf[j]) && (i < sizeof(url) - 1) && (j < sizeof(buf)))
	{
		url[i] = buf[j];
		i++;
		j++; // 判断不是(" ", '\t', '\r', '\n')空字符, 并且长度没有超过 method 的 len-1
	}
	url[i] = '\0';

	// 如果是 GET 类型
	if (strcasecmp(method, "GET") == 0)
	{
		query_string = url;
		// 直到达到 '?', '\0' 结束循环
		while ((*query_string != '?') && (*query_string != '\0'))
			query_string++;
		if (*query_string == '?')
		{
			cgi = 1;
			*query_string = '\0';
			query_string++;
		}
	}

	// 将 usrl 数组中的字符串保存到 path 的字符串中
	sprintf(path, "htdocs%s", url);
	// 遇到 '/' 就将 path 字符串与 index.html 连接起来
	if (path[strlen(path) - 1] == '/')
		strcat(path, "index.html");

	printf("path = %s\n", path);

	// 获取 path 的文件信息,
	// 读取失败
	if (stat(path, &st) == -1)
	{
		// get_line 返回的值不为 0, 并且 buf 遇到 '\n'就继续从套接字里面返回数据,
		while ((numchars > 0) && strcmp("\n", buf)) /* read & discard headers */
			numchars = get_line(Cline, buf, sizeof(buf));
		// 输出 网页404消息
		not_found(Cline);
	}
	// 文件类型
	else
	{
		if ((st.st_mode & S_IFMT) == S_IFDIR)
			strcat(path, "/index.html");
		// 用户权限的判断
		if ((st.st_mode & S_IXUSR) ||
			(st.st_mode & S_IXGRP) ||
			(st.st_mode & S_IXOTH))
			cgi = 1;
		if (!cgi)
			serve_file(Cline, path);
		// else
		// 	execute_cgi(Cline, path, method, query_string);
	}

	// 关闭套接字
	close(Cline);

	return (void *)0;
}

/**********************************************************************/
/* Inform the client that a request it has made has a problem.
 * Parameters: client socket */
/**********************************************************************/
// 400 的网页代码错误
void bad_request(int client)
{
	char buf[1024];

	sprintf(buf, "HTTP/1.0 400 BAD REQUEST\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "<P>Your browser sent a bad request, ");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "such as a POST without a Content-Length.\r\n");
	send(client, buf, sizeof(buf), 0);
}

/**********************************************************************/
/* Put the entire contents of a file out on a socket.  This function
 * is named after the UNIX "cat" command, because it might have been
 * easier just to do something like pipe, fork, and exec("cat").
 * Parameters: the client socket descriptor
 *             FILE pointer for the file to cat */
/**********************************************************************/
// 输出文件内容
void cat(int client, FILE *resource)
{
	char buf[1024];

	// 从文件里面读取字符串数据
	fgets(buf, sizeof(buf), resource);
	// 没有与带结束标志 EOF
	while (!feof(resource))
	{
		// 将数据写入到套接字里面
		send(client, buf, strlen(buf), 0);
		fgets(buf, sizeof(buf), resource);
	}
}

/**********************************************************************/
/* Inform the client that a CGI script could not be executed.
 * Parameter: the client socket descriptor. */
/**********************************************************************/
// 500 的网页错误
void cannot_execute(int client)
{
	char buf[1024];

	sprintf(buf, "HTTP/1.0 500 Internal Server Error\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<P>Error prohibited CGI execution.\r\n");
	send(client, buf, strlen(buf), 0);
}

/**********************************************************************/
/* Print out an error message with perror() (for system errors; based
 * on value of errno, which indicates system call errors) and exit the
 * program indicating an error. */
/**********************************************************************/
// perror 的报错
void error_die(const char *sc)
{
	perror(sc);
	exit(1);
}

/**********************************************************************/
/* Execute a CGI script.  Will need to set environment variables as
 * appropriate.
 * Parameters: client socket descriptor
 *             path to the CGI script */
/**********************************************************************/
// 初始化管道, 并且创建新的进程, 子进程执行新的可执行文件
void execute_cgi(int client, const char *path, const char *method, const char *query_string) // path 为新运行的可执行文件的文件名
{
	char buf[1024];
	// 输出管道
	int cgi_output[2];
	// 输入管道
	int cgi_input[2];
	pid_t pid;
	int status;
	int i;
	char c;
	int numchars = 1;
	int content_length = -1;

	buf[0] = 'A';
	buf[1] = '\0';
	// GET 类型
	if (strcasecmp(method, "GET") == 0)
		while ((numchars > 0) && strcmp("\n", buf)) /* read & discard headers */
			numchars = get_line(client, buf, sizeof(buf));
	else /* POST */
	{
		// 继续从套接字读数据
		numchars = get_line(client, buf, sizeof(buf));
		while ((numchars > 0) && strcmp("\n", buf))
		{
			// 15个字符就停止
			buf[15] = '\0';
			// 比较

			printf("buf = %s\n", buf);

			if (strcasecmp(buf, "Content-Length:") == 0)
				// 从第 16 个字符开始, 跳过空白字符, 将字符转为 int
				content_length = atoi(&(buf[16]));
			// 继续读数据
			numchars = get_line(client, buf, sizeof(buf));
		}
		printf("conlen = %d\n", content_length);
		// 没有读取到 int , 输出 400 的错误信息
		if (content_length == -1)
		{
			bad_request(client);
			return;
		}
	}

	sprintf(buf, "HTTP/1.0 200 OK\r\n");
	send(client, buf, strlen(buf), 0);

	// 初始化输出, 输入管道
	if (pipe(cgi_output) < 0)
	{
		cannot_execute(client);
		return;
	}
	if (pipe(cgi_input) < 0)
	{
		cannot_execute(client);
		return;
	}

	// 创建子进程
	if ((pid = fork()) < 0)
	{
		cannot_execute(client);
		return;
	}
	if (pid == 0) /* child: CGI script */
	{
		char meth_env[255];
		char query_env[255];
		char length_env[255];

		// 将输出重定向到 cgi_output[1]
		dup2(cgi_output[1], 1);
		// 将输入重定向到 cgi_intput[0]
		dup2(cgi_input[0], 0);
		// 关闭其管道的一侧
		close(cgi_output[0]);
		close(cgi_input[1]);

		sprintf(meth_env, "REQUEST_METHOD=%s", method);
		// int putenv(const char *string)
		// putenv() 修改环境变量
		putenv(meth_env);

		// GET 类型
		if (strcasecmp(method, "GET") == 0)
		{
			// 修改环境变量
			sprintf(query_env, "QUERY_STRING=%s", query_string);
			putenv(query_env);
		}
		// 为 POST 修改另一个环境变量
		else
		{ /* POST */
			sprintf(length_env, "CONTENT_LENGTH=%d", content_length);
			putenv(length_env);
		}

		// 启动新的文件
		execl(path, path, NULL);

		exit(0);
	}
	else
	{ /* parent */
		// 父进程关闭其他读写端
		close(cgi_output[1]);
		close(cgi_input[0]);

		// POST 类型
		if (strcasecmp(method, "POST") == 0)
			for (i = 0; i < content_length; i++)
			{
				// 套接字里面读取一个数据
				recv(client, &c, 1, 0);
				// 写进管道里面
				write(cgi_input[1], &c, 1);
			}
		while (read(cgi_output[0], &c, 1) > 0)
			send(client, &c, 1, 0);

		close(cgi_output[0]);
		close(cgi_input[1]);
		waitpid(pid, &status, 0);
	}
}

/**********************************************************************/
/* Get a line from a socket, whether the line ends in a newline,
 * carriage return, or a CRLF combination.  Terminates the string read
 * with a null character.  If no newline indicator is found before the
 * end of the buffer, the string is terminated with a null.  If any of
 * the above three line terminators is read, the last character of the
 * string will be a linefeed and the string will be terminated with a
 * null character.
 * Parameters: the socket descriptor
 *             the buffer to save the data in
 *             the size of the buffer
 * Returns: the number of bytes stored (excluding null) */
/**********************************************************************/
// 读取传入的每个字符.
int get_line(int sock, char *buf, int size) // sock 套接字描述符
{
	int i = 0;
	char c = '\0';
	int n;

	while ((i < size - 1) && (c != '\n'))
	{
		// 向套接字描述符读一个空字符并传入c里面.
		n = recv(sock, &c, 1, 0);
		/* DEBUG printf("%02X\n", c); */
		// 接收成功
		if (n > 0)
		{
			if (c == '\r')
			{
				// '\r' : 查看阻塞的信息, 并将信息传入c中
				n = recv(sock, &c, 1, MSG_PEEK);
				/* DEBUG printf("%02X\n", c); */
				// c == '\n' 继续再读一个字符
				if ((n > 0) && (c == '\n'))
					recv(sock, &c, 1, 0);
				else
					c = '\n';
			}
			// 将读入的字符传入数组里面
			buf[i] = c;
			i++;
		}
		else
			c = '\n';
	}
	// 结束传入 '\0 结束
	buf[i] = '\0';

	// 返回数组的长度(包含 '\0').
	return (i);
}

/**********************************************************************/
/* Return the informational HTTP headers about a file. */
/* Parameters: the socket to print the headers on
 *             the name of the file */
/**********************************************************************/
// 输出 HTTP/1.0 200 OK
void headers(int client, const char *filename)
{
	char buf[1024];
	(void)filename; /* could use filename to determine file type */

	strcpy(buf, "HTTP/1.0 200 OK\r\n");
	send(client, buf, strlen(buf), 0);
	strcpy(buf, SERVER_STRING);
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-Type: text/html\r\n");
	send(client, buf, strlen(buf), 0);
	strcpy(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
}

/**********************************************************************/
/* Give a client a 404 not found status message. */
/**********************************************************************/
// 输出 网页404消息
void not_found(int client)
{
	char buf[1024];

	sprintf(buf, "HTTP/1.0 404 NOT FOUND\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, SERVER_STRING);
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-Type: text/html\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<HTML><TITLE>Not Found</TITLE>\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<BODY><P>The server could not fulfill\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "your request because the resource specified\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "is unavailable or nonexistent.\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "</BODY></HTML>\r\n");
	send(client, buf, strlen(buf), 0);
}

/**********************************************************************/
/* Send a regular file to the client.  Use headers, and report
 * errors to client if they occur.
 * Parameters: a pointer to a file structure produced from the socket
 *              file descriptor
 *             the name of the file to serve */
/**********************************************************************/
// 打开文件, 并调用 cat 将数据写到套接字里面
void serve_file(int client, const char *filename)
{
	FILE *resource = NULL;
	int numchars = 1;
	char buf[1024];

	buf[0] = 'A';
	buf[1] = '\0';
	while ((numchars > 0) && strcmp("\n", buf)) /* read & discard headers */
		numchars = get_line(client, buf, sizeof(buf));

	// 打开文件, resource 保存文件指针
	resource = fopen(filename, "r");
	// 文件打开失败
	if (resource == NULL)
		not_found(client);
	else
	{
		headers(client, filename);
		cat(client, resource);
	}
	fclose(resource);
}

/**********************************************************************/
/* This function starts the process of listening for web connections
 * on a specified port.  If the port is 0, then dynamically allocate a
 * port and modify the original port variable to reflect the actual
 * port.
 * Parameters: pointer to variable containing the port to connect on
 * Returns: the socket */
/**********************************************************************/
// 创建套接字, bind, listen, 并返回套接字
int startup(u_short *port)
{
	int httpd = 0;
	struct sockaddr_in name;

	// 创建套接字
	httpd = socket(PF_INET, SOCK_STREAM, 0);
	if (httpd == -1)
		error_die("socket");

	// 结构提内容初始化为 0
	// bzero(&name, sizeof(name));        
	memset(&name, 0, sizeof(name));

	name.sin_family = AF_INET;				  // 域
	name.sin_port = htons(*port);			  // 端口
	name.sin_addr.s_addr = htonl(INADDR_ANY); // 地址
	// 套接字与地址绑定
	if (bind(httpd, (struct sockaddr *)&name, sizeof(name)) < 0)
		error_die("bind");
	if (*port == 0) /* if dynamically allocating a port */
	{
		socklen_t namelen = sizeof(name);
		// 获取与某个套接字关联的本地协议地址
		if (getsockname(httpd, (struct sockaddr *)&name, &namelen) == -1)
			error_die("getsockname");
		// 将主机字节顺序转为网络字节顺序
		*port = ntohs(name.sin_port);
	}
	if (listen(httpd, 5) < 0)
		error_die("listen");

	return (httpd);
}

/**********************************************************************/
/* Inform the client that the requested web method has not been
 * implemented.
 * Parameter: the client socket */
/**********************************************************************/
// 向套接字里面传递无法处理的 html 信息
void unimplemented(int client)
{
	char buf[1024];

	sprintf(buf, "HTTP/1.0 501 Method Not Implemented\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, SERVER_STRING);
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-Type: text/html\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<HTML><HEAD><TITLE>Method Not Implemented\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "</TITLE></HEAD>\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<BODY><P>HTTP request method not supported.\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "</BODY></HTML>\r\n");
	send(client, buf, strlen(buf), 0);
}

/**********************************************************************/

int main(void)
{
	int server_sock = -1;
	u_short port = 0;
	int client_sock = -1;
	struct sockaddr_in client_name;
	socklen_t client_name_len = sizeof(client_name);
	pthread_t newthread;

	server_sock = startup(&port);
	printf("httpd running on port %d\n", port);

	while (1)
	{
		client_sock = accept(server_sock, (struct sockaddr *)&client_name, &client_name_len);
		if (client_sock == -1)
			error_die("accept");
		/* accept_request(client_sock); */
		if (pthread_create(&newthread, NULL, accept_request, (void *)&client_sock) != 0)
			perror("pthread_create");
	}

	close(server_sock);

	return (0);
}
