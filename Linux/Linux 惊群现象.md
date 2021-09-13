# Linux "惊群现象"

### 前言

​	最近在配置NGINX时遇到“惊群”一词。如今计算机都是多核了，网络编程框架也逐步丰富多了，我所知道的有多进程、多线程、异步事件驱动常用的三种模型。最经典的模型就是Nginx中所用的Master-Worker多进程异步驱动模型。今天和大家一起讨论一下网络开发中遇到的“惊群”现象。之前只是听说过这个现象，网上查资料也了解了基本概念，在实际的工作中还真没有遇到过。今天周末，结合自己的理解和网上的资料，彻底将“惊群”弄明白。需要弄清楚如下几个问题：

（1）什么是“惊群”，会产生什么问题？

（2）“惊群”的现象怎么用代码模拟出来？

（3）如何处理“惊群”问题，处理“惊群”后的现象又是怎么样呢？

### **何为惊群**

​	如今网络编程中经常用到多进程或多线程模型，大概的思路是父进程创建socket，bind、listen后，通过fork创建多个子进程，每个子进程继承了父进程的socket，调用accpet开始监听等待网络连接。这个时候有多个进程同时等待网络的连接事件，当这个事件发生时，这些进程被同时唤醒，就是“惊群”。这样会导致什么问题呢？我们知道进程被唤醒，需要进行内核重新调度，这样每个进程同时去响应这一个事件，而最终只有一个进程能处理事件成功，其他的进程在处理该事件失败后重新休眠或其他。网络模型如下图所示：
![img](https://images2015.cnblogs.com/blog/305504/201706/305504-20170624094342195-1469243485.png)



​	简而言之，惊群现象（thundering herd）就是当多个进程和线程在同时阻塞等待同一个事件时，如果这个事件发生，会唤醒所有的进程，但最终只可能有一个进程/线程对该事件进行处理，其他进程/线程会在失败后重新休眠，这种性能浪费就是惊群。

### **编码模拟“惊群”现象**

　我们已经知道了“惊群”是怎么回事，那么就按照上面的图编码实现看一下效果。我尝试使用多进程模型，创建一个父进程绑定一个端口监听socket，然后fork出多个子进程，子进程们开始循环处理（比如accept）这个socket。测试代码如下所示：

```c++
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <assert.h>  
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
 
#define IP   "127.0.0.1"
#define PORT  8888
#define WORKER 4
 
int worker(int listenfd, int i)
{
    while (1) {
        printf("I am worker %d, begin to accept connection.\n", i);
        struct sockaddr_in client_addr;  
        socklen_t client_addrlen = sizeof( client_addr );  
        int connfd = accept( listenfd, ( struct sockaddr* )&client_addr, &client_addrlen );  
        if (connfd != -1) {
            printf("worker %d accept a connection success.\t", i);
            printf("ip :%s\t",inet_ntoa(client_addr.sin_addr));
            printf("port: %d \n",client_addr.sin_port);
        } else {
            printf("worker %d accept a connection failed,error:%s", i, strerror(errno));
　　　　　　　　 close(connfd);
        }
    }
    return 0;
}
 
int main()
{
    int i = 0;
    struct sockaddr_in address;  
    bzero(&address, sizeof(address));  
    address.sin_family = AF_INET;  
    inet_pton( AF_INET, IP, &address.sin_addr);  
    address.sin_port = htons(PORT);  
    int listenfd = socket(PF_INET, SOCK_STREAM, 0);  
    assert(listenfd >= 0);  
 
    int ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address));  
    assert(ret != -1);  
 
    ret = listen(listenfd, 5);  
    assert(ret != -1);  
 
    for (i = 0; i < WORKER; i++) {
        printf("Create worker %d\n", i+1);
        pid_t pid = fork();
        /*child  process */
        if (pid == 0) {
            worker(listenfd, i);
        }
 
        if (pid < 0) {
            printf("fork error");
        }
    }
 
    /*wait child process*/
    int status;
    wait(&status);
    return 0;
}
```

编译执行，在本机上使用telnet 127.0.0.1 8888测试，结果如下所示：

![img](https://images2015.cnblogs.com/blog/305504/201706/305504-20170624101159023-2003971079.png)

​	按照“惊群"现象，期望结果应该是4个子进程都会accpet到请求，其中只有一个成功，另外三个失败的情况。而实际的结果显示，父进程开始创建4个子进程，每个子进程开始等待accept连接。当telnet连接来的时候，只有worker2 子进程accpet到请求，而其他的三个进程并没有接收到请求。

​	这是什么原因呢？难道惊群现象是假的吗？于是赶紧google查一下，惊群到底是怎么出现的。

​	其实在Linux2.6版本以后，内核内核已经解决了accept()函数的“惊群”问题，大概的处理方式就是，当内核接收到一个客户连接后，只会唤醒等待队列上的第一个进程或线程。所以，如果服务器采用accept阻塞调用方式，在最新的Linux系统上，已经没有“惊群”的问题了。

​	但是，对于实际工程中常见的服务器程序，大都使用select、poll或epoll机制，此时，服务器不是阻塞在accept，而是阻塞在select、poll或epoll_wait，这种情况下的“惊群”仍然需要考虑。接下来以epoll为例分析：

使用epoll非阻塞实现代码如下所示：

```c++
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
 
#define IP   "127.0.0.1"
#define PORT  8888
#define PROCESS_NUM 4
#define MAXEVENTS 64
 
static int create_and_bind ()
{
    int fd = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    inet_pton( AF_INET, IP, &serveraddr.sin_addr);  
    serveraddr.sin_port = htons(PORT);
    bind(fd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    return fd;
}
 
static int make_socket_non_blocking (int sfd)
{
    int flags, s;
    flags = fcntl (sfd, F_GETFL, 0);
    if (flags == -1) {
        perror ("fcntl");
        return -1;
    }
    flags |= O_NONBLOCK;
    s = fcntl (sfd, F_SETFL, flags);
    if (s == -1) {
        perror ("fcntl");
        return -1;
    }
    return 0;
}
 
void worker(int sfd, int efd, struct epoll_event *events, int k) {
    /* The event loop */
    while (1) {
        int n, i;
        n = epoll_wait(efd, events, MAXEVENTS, -1);
        printf("worker  %d return from epoll_wait!\n", k);
        for (i = 0; i < n; i++) {
            if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events &EPOLLIN))) {
                /* An error has occured on this fd, or the socket is not ready for reading (why were we notified then?) */
                fprintf (stderr, "epoll error\n");
                close (events[i].data.fd);
                continue;
            } else if (sfd == events[i].data.fd) {
                /* We have a notification on the listening socket, which means one or more incoming connections. */
                struct sockaddr in_addr;
                socklen_t in_len;
                int infd;
                char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
                in_len = sizeof in_addr;
                infd = accept(sfd, &in_addr, &in_len);
                if (infd == -1) {
                    printf("worker %d accept failed!\n", k);
                    break;
                }
                printf("worker %d accept successed!\n", k);
                /* Make the incoming socket non-blocking and add it to the list of fds to monitor. */
                close(infd); 
            }
        }
    }
}
 
int main (int argc, char *argv[])
{
    int sfd, s;
    int efd;
    struct epoll_event event;
    struct epoll_event *events;
    sfd = create_and_bind();
    if (sfd == -1) {
        abort ();
    }
    s = make_socket_non_blocking (sfd);
    if (s == -1) {
        abort ();
    }
    s = listen(sfd, SOMAXCONN);
    if (s == -1) {
        perror ("listen");
        abort ();
    }
    efd = epoll_create(MAXEVENTS);
    if (efd == -1) {
        perror("epoll_create");
        abort();
    }
    event.data.fd = sfd;
    event.events = EPOLLIN;
    s = epoll_ctl(efd, EPOLL_CTL_ADD, sfd, &event);
    if (s == -1) {
        perror("epoll_ctl");
        abort();
    }
 
    /* Buffer where events are returned */
    events = calloc(MAXEVENTS, sizeof event);
    int k;
    for(k = 0; k < PROCESS_NUM; k++) {
        printf("Create worker %d\n", k+1);
        int pid = fork();
        if(pid == 0) {
            worker(sfd, efd, events, k);
        }
    }
    int status;
    wait(&status);
    free (events);
    close (sfd);
    return EXIT_SUCCESS;
}
```

​	父进程中创建套接字，并设置为非阻塞，开始listen。然后fork出4个子进程，在worker中调用epoll_wait开始accpet连接。使用telnet测试结果如下：

![img](https://images2015.cnblogs.com/blog/305504/201706/305504-20170624113202163-1850796612.png)

​	从结果看出，与上面是一样的，只有一个进程接收到连接，其他三个没有收到，说明没有发生惊群现象。这又是为什么呢？

​	在早期的Linux版本中，内核对于阻塞在epoll_wait的进程，也是采用全部唤醒的机制，所以存在和accept相似的“惊群”问题。新版本的的解决方案也是只会唤醒等待队列上的第一个进程或线程，所以，新版本Linux 部分的解决了epoll的“惊群”问题。所谓部分的解决，意思就是：对于部分特殊场景，使用epoll机制，已经不存在“惊群”的问题了，但是对于大多数场景，epoll机制仍然存在“惊群”。

​	epoll存在惊群的场景如下：在worker保持工作的状态下，都会被唤醒，例如在epoll_wait后调用sleep一次。改写woker函数如下：

```c++
void worker(int sfd, int efd, struct epoll_event *events, int k) {
    /* The event loop */
    while (1) {
        int n, i;
        n = epoll_wait(efd, events, MAXEVENTS, -1);
        /*keep running*/
        sleep(2);
        printf("worker  %d return from epoll_wait!\n", k); 
        for (i = 0; i < n; i++) {
            if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events &EPOLLIN))) {
                /* An error has occured on this fd, or the socket is not ready for reading (why were we notified then?) */
                fprintf (stderr, "epoll error\n");
                close (events[i].data.fd);
                continue;
            } else if (sfd == events[i].data.fd) {
                /* We have a notification on the listening socket, which means one or more incoming connections. */
                struct sockaddr in_addr;
                socklen_t in_len;
                int infd;
                char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
                in_len = sizeof in_addr;
                infd = accept(sfd, &in_addr, &in_len);
                if (infd == -1) {
                    printf("worker %d accept failed,error:%s\n", k, strerror(errno));
                    break;
                }   
                printf("worker %d accept successed!\n", k); 
                /* Make the incoming socket non-blocking and add it to the list of fds to monitor. */
                close(infd); 
            }   
        }   
    }   
}
```

终于看到惊群现象的出现了。

### **解决惊群问题**

​	Nginx中使用**mutex**互斥锁解决这个问题，**具体措施有使用全局互斥锁，每个子进程在epoll_wait()之前先去申请锁，申请到则继续处理，获取不到则等待，并设置了一个负载均衡的算法（当某一个子进程的任务量达到总设置量的7/8时，则不会再尝试去申请锁）来均衡各个进程的任务量**。后面深入学习一下Nginx的惊群处理过程。










