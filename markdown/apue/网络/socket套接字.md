# socket套接字

标签（空格分隔）： 套接字

---

套接字是通信端点的抽象.
**socket()函数创建套接字**

```c
#include <sys/socket.h>

int socket(int domain, int type, int protocol)
// 成功, 返回 套接字文件描述符; 失败, 返回 -1.
```

- **参数 :**
- **domain : 确定通信的特性. 域( 通常以 AF_ 开头的地址族 )**
- - AF_INET : IPv4 因特网域
- - AF_INET6 : IPv6 因特网域
- - AF_UNIX : UNIX 域
- - AF_UPSPEC : 未指定
- **type : 套接字的类型.**
- - SOCK_STREAM : 有序, 可靠的, 双向, 面向连接的(TCP) 
- - SOCK_DGRAM : 固定长度, 不可靠, 无连接的(UDP)
- **protocol : 传输协议. protocol = 0, 默认传输协议.** 
- - IPPROTO_IP : IPv4
- - IPPROTO_IPV6 : IPv6
- - IPPROTO_TCP : TCP协议
- - IPPROTO_UDP : UDP协议
```c
socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)   // TCP
socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)    // UDP
socket(AF_INET, SOCK_STREAM, 0);            //创建TCP套接字
socket(AF_INET, SOCK_DGRAM, 0);             //创建UDP套接字
```

```c
// IPv4
#include <sys/un.h>

struct sockaddr_in 
{  
    uint8_t  sin_len;               // 整个sockaddr_in结构体的长度
    sa_family_t  sin_family;        // 指定该地址家族，在这里必须设为AF_INET
    in_port_t  sin_port;            // 端口
    struct in_addr  sin_addr;        // IPv4
    char sin_zero[8];               // 一般设置为 0
};  

#include <netinet.h>

struct in_addr
{
    in_addr_t s_addr;       // IPv4的地址, 使用inet_addr()函数获得.
}
```

- - - 
### 服务器期准备调用的函数

#### 套接字名称

```c
include <netinet/in.h>  
  
struct sockaddr 
{  
    unsigned short    sa_family;    // 指定该地址家族, AF_INET, AF_UNIX...
    char              sa_data[14];  // 数据成员大小
};  

// 关联地址和套接字
int bind(int socket, const struct sockaddr *address, size_t address_len)
// 成功, 返回 0; 失败, 返回 -1.
```

#### 创建套接字队列

**使得一个进程可以接受其它进程的请求，从而成为一个服务器进程.**

```c
#include <sys/socket.h>

// listen函数在一般在调用bind之后-调用accept之前调用
// backlog : 接受连接请求的数量上限
int listen(int sockfd, int backlog)
// 成功, 返回 0; 失败, 返回 -1.
```

#### 接受连接

**接受请求, 建立连接**

```c
#include <sys/socket.h>

// 客户的信息存放在 addr 结构体里面. 不关心客户信息, addr = NULL 即可.
int accept(int sockfd, struct sockaddr *restrict addr, socklen_t *restrict len)
// 成功, 返回套接字文件描述符; 失败, 返回 -1.
```

- - -
### 客户端使用函数

#### 请求连接

```c
#include <sys/socket.h>

// 如果 sockfd 没有绑定地址. sonnect 会默认绑定一个地址.
int connect(int sockfd, const struct sockaddr *addr, socklen_t len)
// 成功, 返回 0; 失败, 返回 -1.
```

- - -

#### 关闭连接

**客户端, 服务器都要调用close()函数**

- - -

#### 一个伪服务器与客户端

服务器端
```c
/*************************************************************************
    > File Name: socket_bind_服务器端.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年03月09日 星期五 09时02分21秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(void)
{
	int server_sockfd, client_sockfd;
	int  server_len, client_len;
	struct sockaddr_in server_address, client_address;
	
	// server_address 设置信息
	server_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	// 设置端口号
	server_address.sin_port = 9734;
	server_len = sizeof(server_address);

	// 主机与地址进行关联
	if(bind(server_sockfd, (struct sockaddr *)&server_address, server_len) != 0)
	{
		fprintf(stderr, "bind error");
		exit(1);
	}

	// 进行网络监听
	listen(server_sockfd, 5);

	// 接受请求
	client_len = sizeof(client_address);
	client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, (socklen_t * __restrict) &client_len);
	

	// 写读
	while(1)
	{
		char ch;

		if(read(client_sockfd, &ch, 1) <= 0)
			exit(1);
		if(write(STDOUT_FILENO, &ch, 1) != 1)
		{
			fprintf(stderr, "write error");
			exit(1);
		}
	}

	close(client_sockfd);
	exit(0);
}
```


客户端
```c
/*************************************************************************
    > File Name: connect_客户端.cpp
    > Author: Function_Dou
    > Mail: 
    > Created Time: 2018年03月09日 星期五 09时18分12秒
 ************************************************************************/
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>

int main(void)
{
	int sockfd, len;
	struct sockaddr_in address;
	int result;

	// 设置网络设置
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	// 设置端口号
	address.sin_port = 9734;

	// 请求连接
	len = sizeof(address);
	result = connect(sockfd, (struct sockaddr *) &address, len);
	if(result == -1)
	{
		perror("connect error");
		return -1;
	}

	char ch;
	while(1)
	{
		read(STDIN_FILENO, &ch, 1);
		
		if(write(sockfd, &ch, 1) != 1)
		{
			fprintf(stderr, "write error\n");
			exit(1);
		}
	}

	close(sockfd);

	exit(0);
}
```

**两个程序要分别运行, 先运行服务器端, 再运行客户端.**
- - -
ps : 差不多apue这本书快总结完了, 可能之后很长一段时间都不会写了, 要继续看网络编程了, 还要重看csapp那本书.