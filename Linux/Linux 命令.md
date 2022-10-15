# Linux 命令



### sed 命令

```sh
# 显示指定行号内容
sed -n '100p' file.txt
# 往指定行(首行)添加语句
sed -i '1i\I am heading' filename
sed -i 'col(i)\string' filename
```



### split

该指令将大文件分割成较小的文件，在默认情况下将按照每1000行切割成一个小文件。

```sh
split [--help][--version][-<行数>][-b <字节>][-C <字节>][-l <行数>][要切割的文件][输出文件名]
-l:按行切分, 如：-l 100 , 按100行切分
-b:按字节切分,如：-b 100k,按100k切分
-d:切分文件后缀为数字形式
-a:可指定后缀位数, 如-a2即两位,如x00,x01, 还可指定文件前缀, 如-a2 yes,则yes01,yes02

split -l 600000 -d -a 1 ogbl_collab/ogbl_collab_train_edge edge_;
```



example

```sh
$ split -6 README       #将README文件每六行分割成一个文件 
$ ls                                #执行ls指令  
#获得当前目录结构  
README xaa xad xag xab xae xah xac xaf xai    
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



### 用户组操作

查看用户组

```sh
cat /etc/group
```

添加组

```
 groupadd student
```

修改组名称

```
 groupmod -n stu student
```

修改组编号

```
 groupmod -g 111 stu
```

添加分组并指定编号

```
 groupadd -g 222 teacher
```

删除分组

```
 groupdel 222
```

添加分组

```
 groupadd teacher
```

为用户指定所属组

```
 useradd -g teacher zhangsan
```

为用户指定所属组

```
 useradd -g teacher lisi
```

为用户指定工作目录

```
 useradd -d /home/zhangsan zhangsan
```

指定注释

```
 usermod -c iamateacher zhangsan
```

修改用户名

```
 usermod -l zhangsan zhangsan2
```

指定文件夹

```
 usermod -d /home/zhangsan2 zhangsan2
```

修改用户所属组

```
 usermod -g stu zhangsan2
```

删除用户

```
 userdel zhangsan2
```

删除所属文件夹

```sh
 userdel -r lisi
```



### 用户命令

显示登录的用户名

```
 whoami
```

显示指定用户信息，包括用户编号，用户名 主要组的编号及名称，附属组列表

```
 id zhangsan  
```

显示 zhangsan 用户所在的所有组

```
 groups zhangsan
```

显示用户详细资料

```sh
 finger zhangsan  
```



### chown

修改文件用户所属

```sh
sudo chown -R yeshenyong:users ./dir
```



### nohup

英文全称 no hang up（不挂起），用于在系统后台不挂断地运行命令，退出终端不会影响程序的运行

**nohup** 命令，在默认情况下（非重定向时），会输出一个名叫 nohup.out 的文件到当前目录下，如果当前目录的 nohup.out 文件不可写，输出重定向到 **$HOME/nohup.out** 文件中

以下命令在后台执行 root 目录下的 runoob.sh 脚本：

```sh
nohup /root/runoob.sh &
# 在终端如果看到以下输出说明运行成功：
# appending output to nohup.out

# 这时我们打开 root 目录 可以看到生成了 nohup.out 文件。

# 如果要停止运行，你需要使用以下命令查找到 nohup 运行脚本到 PID，然后使用 kill 命令来删除：
ps -aux | grep "runoob.sh" 
```



### strace 命令

​	strace官网的描述, strace是一个可用于诊断、调试和教学的[Linux](https://so.csdn.net/so/search?from=pc_blog_highlight&q=Linux)用户空间跟踪器。我们用它来**监控用户空间进程和内核的交互**，比如系统调用、信号传递、进程状态变更等（比如你要查看epoll 是否有mmap调用）

> strace底层使用内核的ptrace特性来实现其功能

​	在运维的日常工作中，故障处理和问题诊断是个主要的内容，也是必备的技能。strace作为一种动态跟踪工具，能够帮助运维高效地定位进程和服务故障。它像是一个侦探，通过系统调用的蛛丝马迹，告诉你异常的真相。

```sh
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



### free 命令

free 命令看mem 内存use、free

```sh
free -m -s -h 5
```

每5秒更新，并显示单位



### perf 命令

碰到 CPU 使用率升高的问题，你可以借助 top、pidstat 等工具，确认引发 CPU 性能问题的来源；再使用 perf 等工具，排查出引起性能问题的具体函数。

示例：https://blog.csdn.net/RJ0024/article/details/117551145

#### perf stat

> 用于在运行指令时，监测并分析其统计结果。虽然perf top也可以指定pid，但是必须先启动应用才能查看信息。perf stat能完整统计应用整个生命周期的信息。

```sh
perf stat ls
-p：stat events on existing process id (comma separated list). 仅分析目标进程及其创建的线程。
-a：system-wide collection from all CPUs. 从所有CPU上收集性能数据。
-r：repeat command and print average + stddev (max: 100). 重复执行命令求平均。
-C：Count only on the list of CPUs provided (comma separated list), 从指定CPU上收集性能数据。
-v：be more verbose (show counter open errors, etc), 显示更多性能数据。
-n：null run - don’t start any counters，只显示任务的执行时间 。
-x SEP：指定输出列的分隔符。
-o file：指定输出文件，–append指定追加模式。
–pre ：执行目标程序前先执行的程序。
–post ：执行目标程序后再执行的程序。
```

> Performance counter stats for 'ls':  
>
>          0.653782 task-clock                #    0.691 CPUs utilized            
>                 0 context-switches          #    0.000 K/sec                    
>                 0 CPU-migrations            #    0.000 K/sec                    
>               247 page-faults               #    0.378 M/sec                    
>         1,625,426 cycles                    #    2.486 GHz                      
>         1,050,293 stalled-cycles-frontend   #   64.62% frontend cycles idle     
>           838,781 stalled-cycles-backend    #   51.60% backend  cycles idle     
>         1,055,735 instructions              #    0.65  insns per cycle          
>                                             #    0.99  stalled cycles per insn  
>           210,587 branches                  #  322.106 M/sec                    
>            10,809 branch-misses             #    5.13% of all branches          
>    
>       0.000945883 seconds time elapsed 

输出包括ls的执行时间，以及10个性能事件的统计。
task-clock：任务真正占用的处理器时间，单位为ms。CPUs utilized = task-clock / time elapsed，CPU的占用率。

context-switches：上下文的切换次数。

CPU-migrations：处理器迁移次数。Linux为了维持多个处理器的负载均衡，在特定条件下会将某个任务从一个CPU

迁移到另一个CPU。

page-faults：缺页异常的次数。当应用程序请求的页面尚未建立、请求的页面不在内存中，或者请求的页面虽然在内

存中，但物理地址和虚拟地址的映射关系尚未建立时，都会触发一次缺页异常。另外TLB不命中，页面访问权限不匹配

等情况也会触发缺页异常。

cycles：消耗的处理器周期数。如果把被ls使用的cpu cycles看成是一个处理器的，那么它的主频为2.486GHz。

可以用cycles / task-clock算出。

stalled-cycles-frontend：略过。

stalled-cycles-backend：略过。

instructions：执行了多少条指令。IPC为平均每个cpu cycle执行了多少条指令。

branches：遇到的分支指令数。branch-misses是预测错误的分支指令数。

```sh
# perf stat -a -A ls > /dev/null
```



#### perf top 

> perf top 是比较常用于展示占用CPU始终最多的函数或者指令，一般以此来查找热点函数。

```sh
root@xxxx:~# apt install linux-tools-common
root@xxxx:~# perf top

Samples: 6K of event 'cycles', 4000 Hz, Event count (approx.): 48144737 lost: 0/0 drop: 0/0
Overhead  Shared Object                           Symbol
   3.84%  [kernel]                                [k] native_write_msr
   2.25%  [kernel]                                [k] update_blocked_averages
   1.89%  [kernel]                                [k] update_sd_lb_stats.constprop.0
   1.80%  [kernel]                                [k] pvclock_clocksource_read
```

> Samples : 采样数, perf 总共采集了 6K 个 CPU 时钟事件。
>
> event : 事件类型。
>
> Event count (approx.) : 事件总数量
>
> 行列:
>
> Overhead: 是该符号的性能事件在所有采样中的比例，用百分比来表示。
>
> Shared: Shared ，是该函数或指令所在的动态共享对象（Dynamic Shared Object），如内核、进程名、动态链接库名、内核模块名等。
>
> Object: Object ，是动态共享对象的类型。比如 [.] 表示用户空间的可执行程序、或者动态链接库，而 [k] 则表示内核空间。
>
> Symbol: Symbol 是符号名，也就是函数名。当函数名未知时，用十六进制的地址来表示。

> ```
> -e：指定性能事件
> 
> -a：显示在所有CPU上的性能统计信息
> 
> -C：显示在指定CPU上的性能统计信息
> 
> -p：指定进程PID
> 
> -t：指定线程TID
> 
> -K：隐藏内核统计信息
> 
> -U：隐藏用户空间的统计信息
> 
> -s：指定待解析的符号信息
> ```





> 碰到 CPU 使用率升高的问题，你可以借助 top、pidstat 等工具，确认引发 CPU 性能问题的来源；再使用 perf 等工具，排查出引起性能问题的具体函数。





### htop 命令

> 为什么比top 好？

可以横向或纵向滚动浏览进程列表，以便看到所有的进程和完整的命令行。 

在启动上比top 更快。 杀进程时不需要输入进程号。 htop 支持鼠标选中操作（反应不太快）。 

top 已不再维护。

```sh
-C --no-color　　　　 　　 使用一个单色的配色方案 
-d --delay=DELAY　　　　 设置延迟更新时间，单位秒 
-h --help　　　　　　 　　 显示htop 命令帮助信息 
-u --user=USERNAME　　 只显示一个给定的用户的过程 
-p --pid=PID,PID…　　　 只显示给定的PIDs 
-s --sort-key COLUMN　 依此列来排序 
-v –version　　　　　　　 显示版本信息 
交互式命令（INTERACTIVE COMMANDS） 
上下键或PgUP, PgDn 选定想要的进程，左右键或Home, End 移动字段，当然也可以直接用鼠标选定进程； 
Space 标记/取消标记一个进程。命令可以作用于多个进程，例如 "kill"，将应用于所有已标记的进程 
U 取消标记所有进程 
s 选择某一进程，按s:用strace追踪进程的系统调用 
l 显示进程打开的文件: 如果安装了lsof，按此键可以显示进程所打开的文件 
I 倒转排序顺序，如果排序是正序的，则反转成倒序的，反之亦然 
+, - When in tree view mode, expand or collapse subtree. When a subtree is collapsed a "+" sign shows to the left of the process name. 
a (在有多处理器的机器上) 设置 CPU affinity: 标记一个进程允许使用哪些CPU 
u 显示特定用户进程 
M 按Memory 使用排序 
P 按CPU 使用排序 
T 按Time+ 使用排序 
F 跟踪进程: 如果排序顺序引起选定的进程在列表上到处移动，让选定条跟随该进程。这对监视一个进程非常有用：通过这种方式，你可以让一个进程在屏幕上一直可见。使用方向键会停止该功能。 
K 显示/隐藏内核线程 
H 显示/隐藏用户线程 
Ctrl-L 刷新 
Numbers PID 查找: 
输入PID，光标将移动到相应的进程上
```





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



### alias

alias 设置对当前用户有效的方法

只需一步，在 ~/.bashrc 末尾加入你的alias 命令即可，比如 

```sh
alias gs="git status" --color=auto
alisa lk='ls -lh
source ~/.bashrc
```

重新打开一个终端就行了



### lsof 命令

https://blog.csdn.net/MTbaby/article/details/52641206

(list open file)

**例2.**查看谁正在使用某个文件，也就是说查找某个文件相关的进程

```sh
root@iZwz9i5zftiq15tkzdk682Z:~/workflow_exp# lsof /bin/bash
COMMAND   PID USER  FD   TYPE DEVICE SIZE/OFF   NODE NAME
bash     3035 root txt    REG  252,1  1113504 917506 /bin/bash
bash     3074 root txt    REG  252,1  1113504 917506 /bin/bash
bash     3084 root txt    REG  252,1  1113504 917506 /bin/bash
bash     3872 root txt    REG  252,1  1113504 917506 /bin/bash
bash    10504 root txt    REG  252,1  1113504 917506 /bin/bash
bash    19314 root txt    REG  252,1  1113504 917506 /bin/bash
bash    26981 root txt    REG  252,1  1113504 917506 /bin/bash
bash    27355 root txt    REG  252,1  1113504 917506 /bin/bash
bash    28436 root txt    REG  252,1  1113504 917506 /bin/bash
```

**例3.**递归查看某个目录的文件信息

```sh
 [mt555@localhost Desktop]$ cd /opt/soft/
 [mt555@localhost Desktop]$ lsof mt/33
 COMMAND   PID USER   FD   TYPE DEVICE SIZE    NODE NAME
 bash    24941 root  cwd    DIR    8,2 4096 2258872 mt2/33
 vi      24976 root  cwd    DIR    8,2 4096 2258872 mt2/33
 [mt555@localhost Desktop]$
```

**例4.**不使用+D选项，遍历查看某个目录的所有文件信息的方法

```sh
命令：lsof | grep ‘mt2/33’
```

**例5.**列出某个用户打开的文件信息

```sh
命令：lsof -u username
```

说明: -u 选项，u其实是user的缩写

**例6**：列出某个程序进程所打开的文件信息

```sh
命令：lsof -c mysql
```

说明:-c 选项将会列出所有以mysql这个进程开头的程序的文件，其实你也可以写成 lsof | grep mysql, 但是第一种方法明显比第二种方法要少打几个字符了

**例7**：列出多个进程多个打开的文件信息

```sh
命令：lsof -c mysql -c apache
```

...

> - -a 列出打开文件存在的进程
> - -c<进程名> 列出指定进程所打开的文件
> - -g 列出GID号进程详情
> - -d<文件号> 列出占用该文件号的进程
> - +d<目录> 列出目录下被打开的文件
> - +D<目录> 递归列出目录下被打开的文件
> - -n<目录> 列出使用NFS的文件
> - -i<条件> 列出符合条件的进程。（4、6、协议、:端口、 @ip ）
> - -p<进程号> 列出指定进程号所打开的文件
> - -u 列出UID号进程详情
> - -h 显示帮助信息
> - -v 显示版本信息

> COMMAND：进程的名称
>
> PID：进程标识符
>
> PPID：父进程标识符（需要指定-R参数）
>
> USER：进程所有者
>
> PGID：进程所属组
>
> FD：文件描述符，应用程序通过文件描述符识别该文件。如cwd、txt等
>
> DEVICE：指定磁盘的名称
>
> SIZE：文件的大小
>
> NODE：索引节点（文件在磁盘上的标识）
>
> NAME：打开文件的确切名称



### $ 符号

`$` 符号作为一个元字符有不同的含义。当用于匹配模式时，它意味着任何以给定字符串结尾的字符串。例如，当同时使用元字符 `|` 和 `$` 时：

```sh
$ ls | grep png$BlountScreenPicture.pngDisplaySettings.pngEbookStats.pngStrategicPlanMenu.pngScreenshot from 01-24 19-35-05.png
```



### ^ 符号

符号 `^` 将结果限制在以给定字符串开始的项目上。例如，当同时使用元字符 `|` 和 `^` 时：

```shell
$ ls | grep ^ScreenScreenshot from 01-24 19-35-05.png
```



## C++ 相关

### ar 命令

ar命令可以用来创建、查询、修改库。库是一单独的文件，里面包含了按照特定的结构组织起来的源文件。原始文件的内容、模式、时间戳、属主、组等属性都保留在库文件中

命令选项：

```sh
-d：删除库文件中的成员文件
-m：变更成员文件在库文件中的次序
-p：显示库文件中的成员文件内容
-q：将文件附加在库文件末端
-r：将文件插入库文件中
-t：显示库文件中所包含的文件
-x：从库文件中取出成员文件
-a<成员文件>：将文件插入库文件中指定的成员文件之后
-b<成员文件>：将文件插入库文件中指定的成员文件之前
-c：建立库文件
-f：截掉要放入库文件中过长的成员文件名称
-i<成员文件>：将文件插入库文件中指定的成员文件之前
-o：保留库文件中文件的日期
-s：若库文件中包含了对象模式，可利用此参数建立备存文件的符号表
-S：不产生符号表
-u：只将日期较新文件插入库文件中
-v：程序执行时显示详细的信息
-V：显示版本信息
```

生成静态库文件

`ar –rc test.a test.o`表示将.o打包到库文件中



### objdump

objdump（反汇编应用程序、目标文件） 

```sh
objdump -d main.o（反汇编应用程序）
objdump -f main.o （显示文件头信息）
objdump -s -j .comment main.o（显示指定section 段信息（comment 段）
```



### ldd

ldd （list dynamic dependencies）列出动态库依赖关系，列出一个程序所需要动态库（so） a. 

```sh
which ldd b. 
ldd *.so
```

```sh
# 动态库查询路径优先级
LD_PRELOAD>LD_LIBRARY_PATH>/etc/ld.so.cache>/lib>/usr/lib
```





### 常用指令

```shell
# 查找目录下.py 结尾的文件行数
find . -name *.py | xargs wc -l
# 查看进程打开文件描述符(对每个文件描述符可以vim查看数据)
ls /proc/pid/fd/
# 文件查找指定字符串并显示下3行，并显示当前行号
grep -r key=4250105673933320892 ha3_doc.* -A 3 -n
# 文件查找指定字符串成，并记录有多少行
grep -r "searchstr" doc.* | wc -l

# 查找log 字段的每一行计算下来平均值
cat test.log | grep "GameServer_r: " | awk '{sum+=$2} END {print "Avg = ", sum/NR}'
cat test.log | grep "GameServer_r: " | awk -F 'GameServer_r: ' '{sum += $2}; END {printf "%0.3f\n", sum/NR}'

# 查看服务器打开的文件描述符
ps -ef | grep websrv
sudo lsof -p 6346 # 使用 -p 选项指定进程号（lsof，list open file）
# 查看所有内核参数
sysctl -a
# 切分行
split -l 150000 -d -a 1 ogbl_collab/ogbl_collab_node node_;
sed -i '1i\id:int64\tattr:string' node_1;
# 查找文件与删除
find . -type f -name "*.txt" | xargs rm -f
find . -type f -name "*.txt"
find . -type f -name "*.txt" -exec rm {} \;

# 批量运行.sh
find xxx -name BUILD | xargs bash gen_makefile.sh

# Just Find
find xxx -name dir | xargs ls -lrt
```

