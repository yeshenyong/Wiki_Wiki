# Linux（kill）的N种方法

参考自：

https://zhuanlan.zhihu.com/p/143635282

https://blog.csdn.net/smarxx/article/details/6664219

### 根据进程名字kill 

入门篇

```sh
ps -aux
ps -ef
kill -9 1884	# 某个进程
```

进阶篇

改进1：

```sh
ps -ef | grep chrome
kill -9 1884
```

改进2：

pgrep：专门用于进程查询的grep

```sh
pgrep chrome
1884
kill -9 1884
```

改进3：

pidof：xx 的PID（美中不足，需要给出进程全名）

```sh
pidof chrome-bin
kill -9 1884
```

改进4：

"grep -v grep"是在列出的进程中去除含有关键字"grep"的进程

"cut -c 9-15"是截取输入行的第9个字符到第15个字符，而这正好是进程号PID

"xargs kill -9"中的xargs命令是用来把前面命令的输出结果（PID）作为"kill -9"命令的参数，并执行该命令。"kill -9"会强行杀掉指定进程。

```sh
ps -ef | grep chrome | grep -v grep | cut -c 9-15 | xargs kill -9
```

太长了是吧

改进5：

```sh
pgrep chrome | xargs kill -9
```

改进6：

```sh
ps -ef | grep chrome | awk '{print $2}' | xargs kill -9
kill: No such process
```

有一个比较郁闷的地方，进程已经正确找到并且终止了，但是执行完却提示找不到进程。

其中awk '{print $2}' 的作用就是打印（print）出第二列的内容。根据常规篇，可以知道ps输出的第二列正好是PID。就把进程相应的PID通过xargs传递给kill作参数，杀掉对应的进程

改进7：

```sh
kill -9 `ps -aux | grep chrome | awk '{print $2}'`
```

改进8：

```sh
kill -9 `pgrep chrome`
```

改进9：

pkill：pgrep + kill

```sh
pkill -9 chrome
```

改进10：

killall：和pkill 相似，但是要求进程名完整

```sh
killall -9 chrome
```





### kill 与 kill -9 的区别

> kill相当于赐死，kill -9相当于斩立决

kill命令默认的信号就是15，也就是 kill -15 ,被称为优雅的退出。

当使用kill -15时，系统会发送一个SIGTERM的信号给对应的程序。当程序接收到该信号后，具体要如何处理是自己可以决定的。

这时候，应用程序可以选择：

1、立即停止程序

2、释放响应资源后停止程序

3、忽略该信号，继续执行程序

因为kill -15信号只是通知对应的进程要进行"安全、干净的退出"，程序接到信号之后，退出前一般会进行一些"准备工作"，如资源释放、临时文件清理等等，如果准备工作做完了，再进行程序的终止。但是，如果在"准备工作"进行过程中，遇到阻塞或者其他问题导致无法成功，那么应用程序可以选择忽略该终止信号。

这也就是为什么我们有的时候使用kill命令是没办法"杀死"应用的原因，因为默认的kill信号是SIGTERM（15），而SIGTERM（15）的信号是可以被阻塞和忽略的。

大部分程序接收到SIGTERM信号后，会先释放自己的资源，然后在停止。

但是也有程序可以在接受到信号量后，做一些其他的事情，并且这些事情是可以配置的。

如果程序正在等待IO，可能就不会立马做出相应，或者等待其他项目的响应。

#### **kill -9**

我们常碰到一些java项目就是如此，发送kill命令，程序不会退出，要等到 几十秒之后才会退出，一般没耐心等待，所以这个时候，我们就会用 kill -9 必杀命令。

和kill -15相比，kill -9就相对强硬一点，系统会发出SIGKILL信号，他要求接收到该信号的程序应该立即结束运行，不能被阻塞或者忽略。

所以，相比于kill -15命令，kill -9在执行时，应用程序是没有时间进行"准备工作"的，所以这通常会带来一些副作用，数据丢失或者终端无法恢复到正常状态等。



### 总结

kill传递给要终止的进程的信号，实际上是POSIX信号，它们在signal.h里面定义附录摘录了部分POSIX信号，及其相关说明。支持两种方式传递终止信号：

```sh
kill -s signal PID
比如
kill -s 9 1884
```

```sh
kill -signal PID
比如
kill -9 1884
```



上面各种改进方法，设计到的技巧，有两个在其他地方也有借鉴意义

1）进程号的获取部分

**用awk提取第2列或者用cut提取第8-15个字母**。其他地方也可一用awk提取第n列，或者用cut提取第m-第n哥字母。

2）进程号的传递

**用xargs把进程号传递给后面的kill做参数运行；**

**直接用两个“`”（“~”下面那个后单引号）把命令放到对应参数的位置来传递参数。**









