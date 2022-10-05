# C++ hang

hang：悬挂

### 场景一

![08fcbae6713f3287554681a37604dbb9.png](https://img-blog.csdnimg.cn/img_convert/08fcbae6713f3287554681a37604dbb9.png)

很多线程池的实现中，线程池都是静态容量的，它们在初始的时候有一个固定的容量，代表可以运行的最大线程数，但这并不是总能让人满意的，想象这样一个场景，你创建了一个容量为4的线程池，你也创建了一组互相收发数据的函数，假设这些函数都是阻塞的，你一开始 并不知道需要多少线程，你按照习惯一个个进入线程池，结果没过多久，线程池里就都是收数据的线程，整个程序就被阻塞住了。所以，很多时候一个动态的线程池就很有必要



### 场景二

高并发的情况下，程序hang住了，gstack 显示，主要hang在了stl内部

/usr/local/include/c++/9.3.0/bits/stl_tree.h +1928

![img](https://img-blog.csdnimg.cn/20210416170555863.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaWp1bmxpeWFucWluZw==,size_16,color_FFFFFF,t_70)

在调用的std map的find的 时候使用红黑树，触发内存混乱。

修改方式：

1、加锁

2、map操作只在一个线程中顺序执行

总结：

stl本身不是线程安全的，多线程使用的时候需要小心

### 场景三

我这里说的hang住，并不是指死锁，在真实的工作场景中，死锁的情况其实并不很多见。在我工作的几年中，有遇到好几次都是程序hang在某个IO请求上的

#### 1. 排查

排查hang住的最有用的命令是strace和lsof命令
比如我们有一个服务叫 atm-client

##### 1.1 首先查出服务对应的进程

```sh
ps -ef| grep atm-client| grep -v grep
```

输出结果

```sh
root          5      1  0  2017 ?        01:05:39 atm-client
```

##### 1.2 对于多线程的程序可以先列出进程的所有线程

```sh
ps -mp 5 -o THREAD,tid
```

输出结果

```sh
USER     %CPU PRI SCNT WCHAN  USER SYSTEM    TID
root      0.3   -    - -         -      -      -
root      0.0  19    - poll_s    -      -      5
root      0.0  19    - sk_wai    -      -     11
root      0.0  19    - sk_wai    -      -     12
root      0.0  19    - sk_wai    -      -     13
root      0.0  19    - sk_wai    -      -     14
root      0.0  19    - sk_wai    -      -     15
root      0.0  19    - sk_wai    -      -     16
root      0.0  19    - sk_wai    -      -     17
root      0.0  19    - sk_wai    -      -     18
root      0.0  19    - sk_wai    -      -     19
root      0.0  19    - sk_wai    -      -     20
root      0.0  19    - sk_wai    -      -     21
```

##### 1.3 strace 可以跟踪进程或者线程的系统调用

跟踪线程

```sh
strace -p 11
```

或者跟踪进程

```sh
strace -fp 5
 >  -f          Trace  child  processes as they are created by currently traced processes as a result of the fork(2), vfork(2) and clone(2) system calls. Note
               that -p PID -f will attach all threads of process PID if it is multi-threaded, not only thread with thread_id = PID.
```

输出结果：

```sh
Process 11 attached
recvfrom(15, "*-1\r\n", 65536, 0, NULL, NULL) = 5
sendto(15, "*3\r\n$5\r\nBRPOP\r\n$10\r\ntask_queue\r\n"..., 39, 0, NULL, 0) = 39
recvfrom(15,
```

recvfrom函数的原型如下

```c
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                 struct sockaddr *src_addr, socklen_t *addrlen);
```

可以看到程序再此系统调用上一直没有返回。
其中15是进程对应的文件描述符

##### 1.4 用lsof命令看看此文件描述符对应打开的文件

```
lsof -p 5
```

输出结果

```sh
COMMAND   PID USER   FD   TYPE    DEVICE SIZE/OFF      NODE NAME
atm-clien   5 root  cwd    DIR     253,2     4096    404318 /data
atm-clien   5 root  rtd    DIR     253,2     4096    393217 /
atm-clien   5 root  txt    REG     253,2  3341384    403057 /usr/bin/python2.7
atm-clien   5 root  mem    REG     253,2    18936    394250 /lib/x86_64-linux-gnu/libuuid.so.1.3.0
atm-clien   5 root  mem    REG     253,2    30952    396617 /usr/lib/x86_64-linux-gnu/libffi.so.6.0.1
atm-clien   5 root  mem    REG     253,2   136232    401631 /usr/lib/python2.7/lib-dynload/_ctypes.x86_64-linux-gnu.so
atm-clien   5 root  mem    REG     253,2   387272    394238 /lib/x86_64-linux-gnu/libssl.so.1.0.0
atm-clien   5 root  mem    REG     253,2    38480    401643 /usr/lib/python2.7/lib-dynload/_ssl.x86_64-linux-gnu.so
atm-clien   5 root  mem    REG     253,2  1934624    394146 /lib/x86_64-linux-gnu/libcrypto.so.1.0.0
atm-clien   5 root  mem    REG     253,2    20664    401636 /usr/lib/python2.7/lib-dynload/_hashlib.x86_64-linux-gnu.so
atm-clien   5 root  mem    REG     253,2   109232    401648 /usr/lib/python2.7/lib-dynload/datetime.x86_64-linux-gnu.so
atm-clien   5 root  mem    REG     253,2    53555    404558 /usr/local/lib/python2.7/dist-packages/setproctitle.so
atm-clien   5 root  mem    REG     253,2    33448    401641 /usr/lib/python2.7/lib-dynload/_multiprocessing.x86_64-linux-gnu.so
atm-clien   5 root  mem    REG     253,2    54064    401638 /usr/lib/python2.7/lib-dynload/_json.x86_64-linux-gnu.so
atm-clien   5 root  mem    REG     253,2  1820336    404319 /usr/lib/locale/locale-archive
atm-clien   5 root  mem    REG     253,2  1071552    394172 /lib/x86_64-linux-gnu/libm-2.19.so
atm-clien   5 root  mem    REG     253,2   100728    394252 /lib/x86_64-linux-gnu/libz.so.1.2.8
atm-clien   5 root  mem    REG     253,2    10680    394247 /lib/x86_64-linux-gnu/libutil-2.19.so
atm-clien   5 root  mem    REG     253,2    14664    394150 /lib/x86_64-linux-gnu/libdl-2.19.so
atm-clien   5 root  mem    REG     253,2  1857312    394134 /lib/x86_64-linux-gnu/libc-2.19.so
atm-clien   5 root  mem    REG     253,2   141574    394224 /lib/x86_64-linux-gnu/libpthread-2.19.so
atm-clien   5 root  mem    REG     253,2   149120    394114 /lib/x86_64-linux-gnu/ld-2.19.so
atm-clien   5 root    0r   CHR       1,3      0t0 145523740 /dev/null
atm-clien   5 root    1w   CHR       1,3      0t0 145523740 /dev/null
atm-clien   5 root    2w   CHR       1,3      0t0 145523740 /dev/null
atm-clien   5 root    3w   REG     8,196 13683276  67109063 /bbd/logs/ATM/client.log
atm-clien   5 root    4u  IPv4 267010683      0t0       TCP 192.168.100.20:59062->192.168.100.101:7777 (ESTABLISHED)
atm-clien   5 root    5u  IPv4 267815488      0t0       TCP 192.168.100.20:41933->192.168.100.101:7777 (ESTABLISHED)
atm-clien   5 root    6u  IPv4 270794754      0t0       TCP 192.168.100.20:11288->192.168.100.101:7777 (ESTABLISHED)
atm-clien   5 root    7u  IPv4 262063515      0t0       TCP 192.168.100.20:13957->192.168.100.101:7777 (ESTABLISHED)
atm-clien   5 root    8u  IPv4 261634310      0t0       TCP 192.168.100.20:50690->192.168.100.101:7777 (ESTABLISHED)
atm-clien   5 root    9u  IPv4 263474639      0t0       TCP 192.168.100.20:17110->192.168.100.101:7777 (ESTABLISHED)
atm-clien   5 root   10u  IPv4 250697410      0t0       TCP 192.168.100.20:54419->192.168.100.101:7777 (ESTABLISHED)
atm-clien   5 root   11u  IPv4 257778131      0t0       TCP 192.168.100.20:42587->192.168.100.101:7777 (ESTABLISHED)
atm-clien   5 root   12u  IPv4 269664872      0t0       TCP 192.168.100.20:15913->192.168.100.101:7777 (ESTABLISHED)
atm-clien   5 root   13u  IPv4 264889589      0t0       TCP 192.168.100.20:19731->192.168.100.101:7777 (ESTABLISHED)
atm-clien   5 root   14u  IPv4 270352963      0t0       TCP 192.168.100.20:46913->192.168.100.101:7777 (ESTABLISHED)
atm-clien   5 root   15u  IPv4 270360331      0t0       TCP 192.168.100.20:47265->192.168.100.101:7777 (ESTABLISHED)
atm-clien   5 root   16u  IPv4 267913655      0t0       TCP 192.168.100.20:46663->192.168.100.101:7777 (ESTABLISHED)
atm-clien   5 root   17u  IPv4 262974958      0t0       TCP 192.168.100.20:52389->192.168.100.101:7777 (ESTABLISHED)
atm-clien   5 root   18u  IPv4 265357373      0t0       TCP 192.168.100.20:38733->192.168.100.101:7777 (ESTABLISHED)
atm-clien   5 root   19u  IPv4 257788657      0t0       TCP 192.168.100.20:42997->192.168.100.101:7777 (ESTABLISHED)
atm-clien   5 root   20u  IPv4 235599723      0t0       TCP 192.168.100.20:40133->192.168.100.101:7777 (ESTABLISHED)
atm-clien   5 root   21u  IPv4 252429173      0t0       TCP 192.168.100.20:20831->192.168.100.101:7777 (ESTABLISHED)
atm-clien   5 root   22u  IPv4 270907708      0t0       TCP 192.168.100.20:16200->192.168.100.101:7777 (ESTABLISHED)
atm-clien   5 root   23u  IPv4 267407937      0t0       TCP 192.168.100.20:22827->192.168.100.101:7777 (ESTABLISHED)
atm-clien   5 root   24u  IPv4 268780497      0t0       TCP 192.168.100.20:31092->192.168.100.101:7777 (ESTABLISHED)
atm-clien   5 root   25u  IPv4 234480714      0t0       TCP 192.168.100.20:43478->192.168.100.101:7777 (ESTABLISHED)
atm-clien   5 root   26u  IPv4 220149929      0t0       TCP 192.168.100.20:61232->192.168.100.101:7777 (ESTABLISHED)
atm-clien   5 root   27u  IPv4 258614924      0t0       TCP 192.168.100.20:26050->192.168.100.101:7777 (ESTABLISHED)
atm-clien   5 root   28w   REG     8,196     1682  67109072 /bbd/logs/ATM/client.error.log
```

已经能够看到fd为15的连接

```sh
atm-clien   5 root   15u  IPv4 270360331      0t0       TCP 192.168.100.20:47265->192.168.100.101:7777 (ESTABLISHED)
```

192.168.100.101:7777 是我们的一个redis服务, 前面使用了nginx做TCP代理
到此程序hang住的情况已经完全查明

#### 2.解决方法

其实除了阻塞在redis连接上，我还遇到过HTTP请求hang住，解决问题的方法非常简单粗暴，只需要设定socket超时即可
以Python程序举例

##### 2.1 HTTP

```python
requests.get('http://github.com', timeout=0.1)
```

> timeout 仅对连接过程有效，与响应体的下载无关。 timeout 并不是整个下载响应的时间限制，而是如果服务器在 timeout 秒内没有应答，将会引发一个异常（更精确地说，是在 timeout 秒内没有从基础套接字上接收到任何字节的数据时）If no timeout is specified explicitly, requests do not time out.

##### 2.2 Redis

```python
redis_db = redis.Redis(host=redis_conf['host'],
                                    port=redis_conf['port'],
                                    db=redis_conf['db'],
                                    password=redis_conf['password'],
                                    socket_timeout=10
                                    )
```

一般来讲，超时分为2种，一种是建立连接阶段的超时，一种是读取响应时的超时，程序hang住一般都发生在读取响应的阶段

#### 3.分析原因

一般情况下连接断开，或者建联不成功，程序都是能感应到，程序能够自动退出，或是异常，恰恰是当程序执行系统调用recvfrom阻塞读的时候，会死等而hang住
这种情况往往会发生在使用反向代理的情况下，下面是我猜测的，产生问题的原因
![这里写图片描述](https://ut-bucket01.sh1a.qingstor.com/woshiaotian/20220421/69e95e02-c156-11ec-a32f-5626e1cdcfe2.png)
如上图，Nginx做反向代理，给Client提供服务，后端服务器于Nginx建立连接分别是C1、C2、C3, Client与Nginx建立的连接是C0
想象这样一种情况，Client发出一个HTTP请求，nginx收到请求将请求通过C1转发给H1，接收的过程中，H1宕机了，或者连接C1被关闭了，Nginx怎么办，它可能是立马将C2与C0关联，转发它们之间的交互数据，可是这个时候Client还在等待上次HTTP的响应。在这个过程中C0并没有断开过，但是Client无法收到它要的Resonse数据包了











