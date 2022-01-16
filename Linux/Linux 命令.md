# Linux 命令



### sed 命令

```sh
# 显示指定行号内容
sed -n '100p' file.txt
```



### awk

awk 是行处理器，优点是处理庞大文件时不会出现内存溢出或处理缓慢的问题，通常用来格式化文本信息。awk依次对每一行进行处理，然后输出。 
命令形式 awk [-F|-f|-v] ‘BEGIN{} //{command1; command2} END{}’ file 
 [-F|-f|-v] 大参数，-F指定分隔符，-f调用脚本，-v定义变量 var=value 
 ’ ’ 引用代码块 
 BEGIN 初始化代码块，在对每一行进行处理之前，初始化代码，主要是引用全局变量，设置FS分隔符 
 // 匹配代码块，可以使字符串或正则表达式 
 {} 命令代码块，包含一条或多条命令 
 ;多条命令使用分号分隔 
 END 结尾代码块，对每一行进行处理后再执行的代码块，主要进行最终计算或输出 
由于篇幅限制，可自行查找更详细的信息。这里awk命令的作用是从文件中每一行取出我们需要的字符串

### cat

1.一次显示整个文件 cat filename 
2.创建一个文件 cat > fileName 
3.将几个文件合并为一个文件 cat file1 file2 > file 
参数： 
 -n 或 –number 由 1 开始对所有输出的行数编号 
 -b 或 –number-nonblank 和 -n 相似，只不过对于空白行不编号 
 -s 或 –squeeze-blank 当遇到有连续两行以上的空白行，就代换为一行的空白行 
 -v 或 –show-nonprinting

```shell
cat check_info.log.2017-09-20 | awk -F '(txt=|&client)' '{print $2}'
```

awk 命令中 -F 指定每一行的分隔符 在这里 ‘(txt=|&client)’是分隔符，它是一个正则表达式。意义是，用’txt=’或’&client’ 作为分隔符 
举例来说下面这行

```sh
sdate=2017-09-20 23:59:32&txt=com.ford.fordmobile&client_id=x
```

会被分割成

```shell
sdate=2017-09-20 23:59:32&
com.ford.fordmobile
_id=x
```



而`{print $2}`的意思是将每行得到的第二个值打印出来，$0代表获取的所有值 
执行效果如下 



```shell
先排序是因为去重与统计的 ‘unip’命令只能处理相邻行

cat check_info.log.2017-09-20 | awk -F '(txt=|&client)' '{print $2}'|sort
```



```sh
统计数量与去重

cat check_info.log.2017-09-20 | awk -F '(txt=|&client)' '{print $2}'| sort | uniq -c
```



```shell
按重复次数排序

cat check_info.log.2017-09-20 | awk -F '(txt=|&client)' '{print $2}'| sort | uniq -c | sort -nr

sort 的 -n：依照数值的大小排序；-r 按照相反顺序排列
```



```sh
将结果输出到文件中

cat check_info.log.2017-09-20 | awk -F '(txt=|&client)' '{print $2}'| sort | uniq -c | sort -nr > testfile
```



### sort

 sort将文件的每一行作为一个单位，相互比较，比较原则是从首字符向后，依次按ASCII码值进行比较，最后将他们按升序输出。 
 -b：忽略每行前面开始出的空格字符； 
 -c：检查文件是否已经按照顺序排序； 
 -d：排序时，处理英文字母、数字及空格字符外，忽略其他的字符； 
 -f：排序时，将小写字母视为大写字母； 
 -i：排序时，除了040至176之间的ASCII字符外，忽略其他的字符； 
 -m：将几个排序号的文件进行合并； 
 -M：将前面3个字母依照月份的缩写进行排序； 
 -n：依照数值的大小排序； 
 -o<输出文件>：将排序后的结果存入制定的文件； 
 -r：以相反的顺序来排序； 
 -t<分隔字符>：指定排序时所用的栏位分隔字符；



### uniq

uniq 命令用于报告或忽略文件中的重复行，一般与sort命令结合使用 
  -c或——count：在每列旁边显示该行重复出现的次数； 
 -d或–repeated：仅显示重复出现的行列； 
 -f<栏位>或–skip-fields=<栏位>：忽略比较指定的栏位； 
 -s<字符位置>或–skip-chars=<字符位置>：忽略比较指定的字符； 
 -u或——unique：仅显示出一次的行列； 
 -w<字符位置>或–check-chars=<字符位置>：指定要比较的字符。



### pam_tally2

**PAM身份验证安全配置实例**

 \- 强制使用强密码（用户密码安全配置）

 \- 用户SSH登录失败尝试次数超出限制后锁定账户（帐户锁定/解锁和时间设置）

 \- 允许普通用户使用sudo而不是su（限制普通用户登录到root用户）

 \- 禁止直接使用root用户通过SSH登录

PAM（Pluggable Authentication Modules ）是由Sun提出的一种认证机制。它通过提供一些动态链接库和一套统一的API，将系统提供的服务 和该服务的认证方式分开，使得系统管理员可以灵活地根据需要给不同的服务配置不同的认证方式而无需更改服务程序，同时也便于向系 统中添加新的认证手段。从本篇开始会总结一些常用的pam模块及其实现的功能，今天讲的是pam_tally2模块。

可以通过以下指令查看361way用户登录的错误次数及详细信息：

可以通过以下命令清空361way用户的错误登录次数，即手动解锁：（root用户）

（faillock 也可以）

```shell
pam_tally2 --user=username
pam_tally2 --user=username --reset

失败登录日志文件，默认为/var/log/tallylog
```



pam_tally2与pam_tally模块的区别是前者增加了自动解锁时间的功能，后者没有。所以在老的发行版中，如果使用了pam_tally模块时，可以使用pam_tally 、faillog配合crontab 进行自动解锁。

https://www.cnblogs.com/klb561/p/9236360.html



### strace 命令

​	strace官网的描述, strace是一个可用于诊断、调试和教学的[Linux](https://so.csdn.net/so/search?from=pc_blog_highlight&q=Linux)用户空间跟踪器。我们用它来**监控用户空间进程和内核的交互**，比如系统调用、信号传递、进程状态变更等（比如你要查看epoll 是否有mmap调用）

> strace底层使用内核的ptrace特性来实现其功能

​	在运维的日常工作中，故障处理和问题诊断是个主要的内容，也是必备的技能。strace作为一种动态跟踪工具，能够帮助运维高效地定位进程和服务故障。它像是一个侦探，通过系统调用的蛛丝马迹，告诉你异常的真相。

```
strace -tt -T -v -f -e trace=file -o /data/log/strace.log -s 1024 -p 23489
```

> -tt 在每行输出的前面，显示毫秒级别的时间
> -T 显示每次系统调用所花费的时间
> -v 对于某些相关调用，把完整的环境变量，文件stat结构等打出来。
> -f 跟踪目标进程，以及目标进程创建的所有子进程
> -e 控制要跟踪的事件和跟踪行为,比如指定要跟踪的系统调用名称
> -o 把strace的输出单独写到指定的文件
> -s 当系统调用的某个参数是字符串时，最多输出指定长度的内容，默认是32个字节
> -p 指定要跟踪的进程pid, 要同时跟踪多个pid, 重复多次-p选项即可。



```shell
问题：机器上有个叫做run.sh的常驻脚本，运行一分钟后会死掉。需要查出死因。

定位：进程还在运行时，通过ps命令获取其pid, 假设我们得到的pid是24298

strace -o strace.log -tt -p 24298

strace -tt -T -f -e trace=file -o /data/log/strace.log -s 1024 ./nginx
```

​	当发现进程或服务异常时，我们可以通过strace来跟踪其系统调用，“看看它在干啥”，进而找到异常的原因。熟悉常用系统调用，能够更好地理解和使用strace。

https://blog.csdn.net/cs729298/article/details/81906375



Linux内核目前有300多个系统调用，详细的列表可以通过syscalls手册页查看。这些系统调用主要分为几类：

```sh
文件和设备访问类 比如open/close/read/write/chmod等
进程管理类 fork/clone/execve/exit/getpid等
信号类 signal/sigaction/kill 等
内存管理 brk/mmap/mlock等
进程间通信IPC shmget/semget * 信号量，共享内存，消息队列等
网络通信 socket/connect/sendto/sendmsg 等
其他
```



> grep -r CMD ./filename -A 3 > vim o1
>
> grep -r  CMD ./filename | wc -l



### pstack 和 strace

工作中难免会遇到各种各样的 bug，对于开发环境 or 测试环境的问题还好解决，可以使用 gdb 打断点或者在代码中埋点来定位异常;
但是遇到线上的 bug 就很难受了，由于生产环境不能随意替换、中断程序，如果日志中找不到问题原因，解决问题就会很棘手

这时候就需要请出这两位 debug 利器了 ———— pstack & strace

> pstack 是 Linux 系统下的一个命令行工具，此命令可以显示指定进程每个线程的堆栈快照，便于排查程序异常和性能评估
>
> pstack 是基于 gdb 实现的，通过 man pstack 可以发现，它其实是 gstack 的一个软链接
>
> 而 gstack 则是封装了 gdb 功能的 shell 脚本，通过 " thread apply all bt " 的命令获得输出所有的线程堆栈信息，再用 sed 进行替换和过滤



**strace查看系统调用**

首先我们拉起server/client程序，并使用[strace](http://www.cnblogs.com/bangerlee/archive/2012/02/20/2356818.html)对server进程进行跟踪：

```
# ps -elf | grep server | grep -v grep

0 S root 16739 22642 0 76 0 - 634 1024 14:26 pts/2 00:00:00 ./server

\# strace -o server.strace -Ttt -p 16739

Process 16739 attached - interrupt to quit
```

 

稍等一段时间之后，我们将strace停掉， server.strace文件中有以下输出：



```shell
14:46:39.741366 select(8, [3 4], NULL, NULL, {1, 0}) = 1 (in [4], left {0, 1648}) <0.998415>

14:46:40.739965 recvfrom(4, "hello", 6, 0, NULL, NULL) = 5 <0.000068>

14:46:40.740241 write(1, "hello\n", 6)  = 6 <0.000066>

14:46:40.740414 rt_sigprocmask(SIG_BLOCK, [CHLD], [], 8) = 0 <0.000046>

14:46:40.740565 rt_sigaction(SIGCHLD, NULL, {SIG_DFL, [], 0}, 8) = 0 <0.000048>

14:46:40.740715 rt_sigprocmask(SIG_SETMASK, [], NULL, 8) = 0 <0.000046>

14:46:40.740853 nanosleep({1, 0}, {1, 0}) = 0 <1.000276>

14:46:41.741284 sendto(4, "hello\0", 6, 0, NULL, 0) = 6 <0.000111>
```



可以看到server接收数据之后(对应recvfrom调用)，经过1s左右时间将消息发出(对应sendto调用)，从响应时间看，与抓包的结果吻合。又可以看出nanosleep系统调用耗费了1s时间。



因而可以断定响应延时由nanosleep对应的函数调用造成。



那具体是哪一个函数调用呢？在strace输出结果中并不能找到答案，因其输出显示都是系统调用，要显示程序中函数调用栈信息，就轮到pstack上场了。

 

**pstack查看函数堆栈**

pstack是一个脚本工具，其核心实现就是使用了gdb以及thread apply all bt命令，下面我们使用pstack查看server进程函数堆栈：

```shell
# sh pstack.sh 16739

\#0 0x00002ba1f8152650 in __nanosleep_nocancel () from /lib64/libc.so.6

\#1 0x00002ba1f8152489 in sleep () from /lib64/libc.so.6

\#2 0x00000000004007bb in ha_ha ()

\#3 0x0000000000400a53 in main ()
```

从以上信息可以看出，函数调用关系为：main->ha_ha->sleep，因而我们可以找到ha_ha函数进行分析和优化修改。

```
strace + pstack
```





### vmstat

vmstat是Virtual Meomory Statistics（虚拟内存统计）的缩写，可实时动态监视操作系统的虚拟内存、进程、CPU活动。

vmstat [-V] [-n] [delay [count]]

> \- -V表示打印出版本信息；- -n表示在周期性循环输出时，输出的头部信息仅显示一次；- delay是两次输出之间的延迟时间；- count是指按照这个时间间隔统计的次数。

```
/root- vmstat 5 5
procs -----------memory---------- ---swap-- -----io---- --system-- -----cpu-----
r  b   swpd   free   buff  cache   si   so    bi    bo   in   cs us sy id wa st
6  0      0 27900472 204216 28188356    0    0     0     9    1    2 11 14 75  0  0
9  0      0 27900380 204228 28188360    0    0     0    13 33312 126221 22 20 58  0  0
2  0      0 27900340 204240 28188364    0    0     0    10 32755 125566 22 20 58  0  0
```

**字段说明**

**Procs**（进程）:
\- r: 运行队列中进程数量
\- b: 等待IO的进程数量

**Memory**（内存）:
\- swpd: 使用虚拟内存大小
\- free: 可用内存大小
\- buff: 用作缓冲的内存大小
\- cache: 用作缓存的内存大小

**Swap**:
\- si: 每秒从交换区写到内存的大小
\- so: 每秒写入交换区的内存大小

**IO**：（现在的Linux版本块的大小为1024bytes）
\- bi: 每秒读取的块数
\- bo: 每秒写入的块数

**system**：
\- in: 每秒中断数，包括时钟中断
\- cs: 每秒上下文切换数

**CPU**（以百分比表示）

us: 用户进程执行时间(user time)
\- sy: 系统进程执行时间(system time)
\- id: 空闲时间(包括IO等待时间)
\- wa: 等待IO时间





### ipcs 查询进程间通信状态

> ​	ipcs 是 Linux 下显示进程间通信设施状态的工具。可以显示消息队列、共享内存和信号量的信息。对于程序员非常有用。

```sh
ipcs
------ Shared Memory Segments --------
key        shmid      owner      perms      bytes      nattch     status
 
------ Semaphore Arrays --------
key        semid      owner      perms      nsems
0x00000000 229376     weber      600        1
 
------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages

- ipcs -m 查看系统使用的IPC共享内存资源
- ipcs -q 查看系统使用的IPC队列资源
- ipcs -s 查看系统使用的IPC信号量资源
```

```sh
ipcs -l
 
------ Shared Memory Limits --------
max number of segments = 4096
max seg size (kbytes) = 4194303
max total shared memory (kbytes) = 1073741824
min seg size (bytes) = 1
 
------ Semaphore Limits --------
max number of arrays = 128
max semaphores per array = 250
max semaphores system wide = 32000
max ops per semop call = 32
semaphore max value = 32767
 
------ Messages: Limits --------
max queues system wide = 2048
max size of message (bytes) = 524288
default max size of queue (bytes) = 5242880
```

以上输出显示，目前这个系统的允许的最大内存为1073741824kb；最大可使用128个信号量，每个消息的最大长度为524288bytes；

**修改IPC系统参数**

以linux系统为例，在root用户下修改`/etc/sysctl.conf` 文件，保存后使用`sysctl -p`生效:

```shell
- cat /etc/sysctl.conf
# 一个消息的最大长度
kernel.msgmax = 524288
 
# 一个消息队列上的最大字节数
# 524288*10
kernel.msgmnb = 5242880
 
#最大消息队列的个数
kernel.msgmni=2048
 
#一个共享内存区的最大字节数
kernel.shmmax = 17179869184
 
#系统范围内最大共享内存标识数
kernel.shmmni=4096
 
#每个信号灯集的最大信号灯数 系统范围内最大信号灯数 每个信号灯支持的最大操作数 系统范围内最大信号灯集数
#此参数为系统默认，可以不用修改
#kernel.sem = <semmsl> <semmni>*<semmsl> <semopm> <semmni>
kernel.sem = 250 32000 32 128
```







### 常用指令

```shell
# 查找目录下.py 结尾的文件行数
find . -name *.py | xargs wc -l
# 查看进程打开文件描述符(对每个文件描述符可以vim查看数据)
ls /proc/pid/fd/
# 文件查找指定字符串并显示下3行，并显示当前行号
grep -r key=4250105673933320892 ha3_doc.* -A 3 -n
```

