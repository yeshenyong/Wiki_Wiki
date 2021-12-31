# 操作系统——CPU

### CPU管理的直观想法



> 管理CPU，先要使用CPU

**CPU的工作原理**

![image-20211014150410029](C:\Users\10505\AppData\Roaming\Typora\typora-user-images\image-20211014150410029.png)

 	冯诺依曼说了：需要把程序存放到内存里，让后只设置一个PC地址，如PC=50，CPU根据50发出一条取址指令放在地址总线上，内存中50的指令从总线上传到CPU上。且PC自动累加，CPU开始不断取址执行工作。

​	CPU开始解析处理这条指令



CPU怎么工作？

> ​	自动的取址——执行

CPU怎么管理？

- 设置好PC初值就完事！

自动不断取址执行





> 看看这样做有没有问题？提出问题

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200311175123260.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzEzNTE3OA==,size_16,color_FFFFFF,t_70)



有I/O指令与没有IO指令，执行时间比例过高



发现有IO的执行语句很慢（因为要驱动磁盘）但是没有IO时计算快，

**CPU工作10^6-IO执行一次-CPU再工作，效率为50%**
如果大多数为IO，少量为CPU工作，那么CPU的利用率基本为0，怎么解决?



**多道程序，交替执行**（上下文切换）

<img src="https://img-blog.csdnimg.cn/20200311175155888.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzEzNTE3OA==,size_16,color_FFFFFF,t_70" alt="在这里插入图片描述" style="zoom:50%;" />





<img src="https://img-blog.csdnimg.cn/20200311175202555.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzEzNTE3OA==,size_16,color_FFFFFF,t_70" alt="在这里插入图片描述" style="zoom:50%;" />



 

- 一个CPU上交替的执行多个程序：并发



> 怎么做到？

修改寄存器PC就行了吗？

<img src="https://img-blog.csdnimg.cn/20200311175210691.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzEzNTE3OA==,size_16,color_FFFFFF,t_70" alt="在这里插入图片描述" style="zoom:50%;" />



**记录切出去的时候执行的样子**
那么运行的程序和静态程序就不一样了
由此引入“进程”概念



<img src="https://img-blog.csdnimg.cn/20200311175216832.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzEzNTE3OA==,size_16,color_FFFFFF,t_70" alt="在这里插入图片描述" style="zoom:50%;" />







### 多进程图像

> 多进程图像从启动开始到关机结束

fork 系统调用启动进程的接口



shell 命令核心代码

```c
int main(int argc, char *argv) 
{
	while(1) {
		scanf("%s", cmd);
		if (!fork()) {
			exec(cmd);
			wait();
		}
	}
}
```

<img src="https://img-blog.csdnimg.cn/20200214090535932.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MTczNjMx,size_16,color_FFFFFF,t_70" alt="在这里插入图片描述" style="zoom:50%;" />

​	**有关fork()的详细说明：fork()执行时，会产生一个子进程，该子进程与父进程除了进程号外，一模一样，fork()返回的值是子进程的号，而子进程返回的值是0。所以从if(!fork()){}；可以判断是否为子进程还是父进程，从而实现叉子的效果！**

<img src="https://img-blog.csdnimg.cn/20200214091608498.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MTczNjMx,size_16,color_FFFFFF,t_70" alt="在这里插入图片描述" style="zoom:50%;" />



**PCB**：用来记录进程信息的数据结构，切换进程改变PCB

**把进程按照状态进行分类：新建态，就绪态，运行态，阻塞态，终止态**

<img src="https://img-blog.csdnimg.cn/20200214091743143.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MTczNjMx,size_16,color_FFFFFF,t_70" alt="在这里插入图片描述" style="zoom:50%;" />





**多进程图像：多进程如何交替？**
pCur为当前进程

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200214092211750.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MTczNjMx,size_16,color_FFFFFF,t_70)

启动磁盘读写；开始写；（写磁盘比较慢）将该进程放在就绪态；schedule();从就绪状态中选择下一个进程开始工作。
switch_to(pCur,pNew);//把这个进程的状态用PCB保存起来，把接下来的PCB赋给各个寄存器。

<img src="https://img-blog.csdnimg.cn/2020021409305749.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MTczNjMx,size_16,color_FFFFFF,t_70" alt="在这里插入图片描述" style="zoom:67%;" />

寄存器、LDT、cs段号、pc指令等



**进程调度**

<img src="https://img-blog.csdnimg.cn/20200214092920694.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MTczNjMx,size_16,color_FFFFFF,t_70" alt="在这里插入图片描述" style="zoom: 33%;" />

#### 操作系统如何组织多进程图像和保存多进程图像

- 操作系统保存着各个进程的PCB信息放在不同的位置
- 操作系统维护着进程多个状态的队列：如阻塞队列、就绪队列、运行队列等

> 多进程图像：多进程如何影响？
>
> 内存解决方法：限制读写（映射表来杜绝）、多进程的地址空间分离：内存管理的主要内容
>
> 工作切换：当进程随机切换时，有时候，会影响操作的结果，需要在线程执行时候对某一段程序进行上锁操作！



<img src="https://img-blog.csdnimg.cn/20200214094848533.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MTczNjMx,size_16,color_FFFFFF,t_70" alt="在这里插入图片描述" style="zoom: 67%;" />



要修改counter，两个counter的话会由于进程的切换导致，结果就会出错。

<img src="https://img-blog.csdnimg.cn/20200214095221112.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MTczNjMx,size_16,color_FFFFFF,t_70" alt="在这里插入图片描述" style="zoom:67%;" />



怎么解决，给进程上锁

![在这里插入图片描述](https://img-blog.csdnimg.cn/202002140955259.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MTczNjMx,size_16,color_FFFFFF,t_70)



问题：操作系统如何对这种操作进行上锁，上锁的底层原理是什么？

<img src="C:\Users\10505\AppData\Roaming\Typora\typora-user-images\image-20211018112301323.png" alt="image-20211018112301323" style="zoom:33%;" />



### 用户级线程

**线程与进程之间的关系？**（分治策略）
进程切换 = 资源+指令执行序列

- 将资源和指令执行分开
- 一个资源　＋　多个指令执行序列

线程：**保留了并发的优点，避免了进程切换的代价**。
但是线程不像进程一样（进程需要切换资源+指令序列，而线程只需要执行指令序列）

**实质是：线程就是映射表不变而PC指针变（指令级别的切换）**



线程使用的实例：
一个网页的浏览器
一个线程用来从服务器接收数据
一个线程用来显示文本
一个线程用来处理图片
一个线程用来显示图片



**这些线程要共享资源吗？**
接收数据放在100处，显示时要读…
所有的文本，图片都显示在一个屏幕上
思想：有一个线程接收数据，接受文本之后，yield切换到另一个线程中去显示下载的文本，接着yield切换到另一个进程去下载图片~~



重点！！！yield与Create是怎样操作的呢？

<img src="https://img-blog.csdnimg.cn/20200214103053102.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MTczNjMx,size_16,color_FFFFFF,t_70" alt="在这里插入图片描述" style="zoom:50%;" />

**压栈（函数调用或yield）和弹栈（ret）**

**独立执行序列有独属于自己的栈，线程拥有属于自己的独立栈帧**，TCB（全局数据结构，thread control block）

**TCB和栈相互配合**



**以下是操作系统最最最重要的核心！**

<img src="https://img-blog.csdnimg.cn/20200214103636616.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MTczNjMx,size_16,color_FFFFFF,t_70" alt="在这里插入图片描述" style="zoom:50%;" />



上面的**右边yield**中？为**204**，而实际实现的操作，就会出错！！！两个线程切换，不可以使用一个栈，要使用两个栈。此时就用上了TCB

```c
void yield()
{
TCB.esp = esp;
esp = TCB1.esp;
}
```

<img src="https://img-blog.csdnimg.cn/20200214105316468.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MTczNjMx,size_16,color_FFFFFF,t_70" alt="在这里插入图片描述" style="zoom: 80%;" />



​	getData(),与show() ,都是用户级线程，切换时，内核根本就不知道
但是使用硬件资源时，就会进入内核，分配资源，转化为核心级线程。如下图中，GetData中等待网卡IO的操作。

#### 为什么内核级线程并发性更好？



![在这里插入图片描述](https://img-blog.csdnimg.cn/20200214105814764.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MTczNjMx,size_16,color_FFFFFF,t_70)



​	核心级线程,ThreadCreate是系统调用，会进入内核，内核知道TCB
核心线程会进入内核，当该线程阻塞时，会换至另一个核心级线程show，进行显示，**所以核心级线程并发性更好**！！
​	为了区分用户级线程与核心级线程的切换，用户级线程切换使用yield()，内核线程的切换为Schedule(),对用户来说，完全不可见。

​	可以内核级线程切换

> 用戶級线程就不管操作系统支持不支持了。



### 内核级线程

切用户级线程是切内核级线程的一部分



**多处理器和多核**

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201012163842795.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzU0MTA5NA==,size_16,color_FFFFFF,t_70#pic_center)

​	多处理器中，每一个处理器都有其对应的MMU(映射表)和Cache(缓存)，而多核处理器中，所有的核心共享一个Cache和MMU，核心级多线程在多核处理器中是**并行**执行的(OS感知不到用户级线程)



#### 用户级线程与核心级线程的不同

​	核心级线程中ThreadCreat是系统调用，内核管理TCB，内核负责切换线程，在用户态中线程由用户栈，到内核态中还需要有内核栈

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201012164900733.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzU0MTA5NA==,size_16,color_FFFFFF,t_70#pic_center)


用户级线程与核心级线程的不同：**从一个栈到一套栈，两个栈到两套栈**

> 用户级线程是TCB切换，根据TCB切换用户栈，核心级线程是TCB切换，根据TCB切换内核栈和用户栈

中断找到想相应的TCB，进行线程切换

switch_to：仍然是通过TCB找到内核栈指针，然后ret切到某个内核程序，最后再用CS：IP切换到用户程序，cur和next都是TCB

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201012170438542.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzU0MTA5NA==,size_16,color_FFFFFF,t_70#pic_center)



![在这里插入图片描述](https://img-blog.csdnimg.cn/20201012172811554.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzU0MTA5NA==,size_16,color_FFFFFF,t_70#pic_center)

就是没有地址映射表





### CPU 调度策略



如何做到合理？需要折中和综合

- 吞吐量和响应时间之间有矛盾

  响应时间少=>切换次数多=>系统内耗大=>吞吐量少

- 前台任务和后台任务的关注点

  前台任务关注响应时间，后台任务关注周转时间

- IO约束型和CPU约束型任务各有特点

![image-20211030093016264](C:\Users\10505\AppData\Roaming\Typora\typora-user-images\image-20211030093016264.png)

IO约束型多数指前台任务或者很像，前台任务的优先级较高

> 折中和综合让操作系统变得复杂，但有效的系统又要求尽量简单...



#### 常见CPU调度算法

##### FCFS （First Come, First Served）



##### SJF ：短作业优先

将短作业往前提，导致系统平均周转时间是最少的



响应时间该怎么办？



##### RR：按时间片来轮转调度

- 时间片大：响应时间太长；时间片小：吞吐量少
- 折中：时间片10~100ms，切换时间0.1~1ms（1%）

##### 例子

> ​	Word 很关心响应时间，而gcc更关心周转时间，两类任务同时存在怎么办？

- 一个调度算法让多种类型的任务同时都满意，怎么办？
- 直观想法：定义前台任务和后台任务两队列，前台RR，后台SJF，只有前台任务没有时才调度后台任务

![image-20211030094303876](C:\Users\10505\AppData\Roaming\Typora\typora-user-images\image-20211030094303876.png)

但是这会产生很多问题（优先级调度）

一个故事：1973年关闭的MIT的IBM 7094时，发现有一个进程在1967年提交但一直未运行

- 后台任务优先级动态升高，但后台任务（用SJF调度）一旦执行，前台的响应时间...
- 前后台任务都用时间片，但又退化为了RR，后台任务的SJF如何体现？前台任务如何照顾？



问题：分析到此出了，我们该怎么办？

- 我们怎么知道哪些是前台任务，哪些是后台任务，fork时告诉我们吗？
- gcc就一点不需要交互吗？Ctrl+C按键怎么工作？Word就不会执行一段批处理吗？Ctrl+F按键？
- SJF中的短作业优先如何体现？如何判断作业的长度？这是未来的信息...





### Counter

可代表时间片、也可代表优先级

io时间（阻塞队列呆的时间）越长，之后时间片或者说优先级会越高，照顾了IO约束型进程	

```c
(*p)->counter = ((*p)->counter>>1 + (*p)->priority)
```

- counter代表的优先级可以动态调整
- counter 保证了响应时间的界
- 经过IO以后，counter就会变大；IO时间越长，counter越大，照顾了IO进程，变相的照顾了前台进程
- 后台进程一直按照counter轮转，近似了SJF调度
- 每个进程只用维护一个counter 变量，简单、搞笑



> ​	CPU 调度：一个简单的算法这种了大多数任务的需求，这就是实际工作的schedule 函数



### 进程同步和通信

#### fork

​	“这都是历史遗留问题了，早期都是单线程的程序，一个`task_struct`就是一个进程，fork这样做是没有问题的，后来出现了多线程技术，一个`task_struct`实际上是一个线程了，多个`task_struct`通过共享地址空间，成为一个线程组，也就是进程，但fork仍然只复制当前的线程，就有了这个问题”

进程合作：多进程共同完成一个任务

实例一：

```
司机
while(true){
	启动车辆;
	正常运行;
	到站停车
}
售票员
while(true){
	关门;
	售票;
	开门;
}
```

正常情况下，进程之间互相不知道对方已存在

需合理有序向前推进

核心：需要等待阻塞，不能随意执行

- 需要让“进程走走停停”来保证多进程合作的合理有序

只发信号还不能解决全部问题



#### 信号量机制

sem

生产者消费者

- （1）缓冲区满，P1执行，P1sleep	sem=-1
- （2）P2执行，P2sleep，sem=-2
- （3）C执行1次循环，wakeup P1 sem=-1
- （4）C再执行1次循环，wakeup P2 sem=0
- （5）C再执行1次循环 sem=1
- （6）P3执行，P2继续执行 sem=0

producer、consumer

```c
struct semaphore
{
	int value;	//记录资源个数
	PCB* queue;	//记录等待在该信号量上的进程
}
P(semaphore s)
{
	s.value--;
	if (s.value < 0)
		sleep(s.queue);
}
V(semphore s)
{
    s.value++;
    if (s.value <= 0)
        wakeup(s.queue)
}
```

PV操作

P（semaphore s）(test)；//消费资源

V（semaphore s）(increment)；//产生资源

解决生产者消费者的问题



```c
semaphore full = 0;
semaphore empty = BUFFER_SIZE;
semaphore mutex = 1;
Producer(item) {
	P(empty);
	P(mutex);
	读入in;将item写入到in的位置上;
	V(mutex);
	V(full);
}
Consumer() {
	P(full);
	P(mutex);
	读入out;从文件中out位置读出到item，打印Item
	V(mutex);
	V(empty);
}
```



温故而知新：什么是信号量？通过对这个量的访问和修改，让大家有序推进



### 信号量临界区保护

> 软件的角度

共同修改信号量引出的问题

- 竞争条件：和调度有关的共享数据语义错误



- 错误由多个进程并发操作共享数据引起
- 错误和调度顺路有关，难以发现和调试







- 一个非常重要的工作：找出进程中的临界区代码



Peterson算法——两个进程

通过两个变量自旋实现互斥进入、有空让进、有限等待



多进程怎么办？——面包店算法

- 仍然是标记和轮转的结合
- 如何轮转：每个进程都获得一个序号，序号最小的进入
- 如何标记：进程离开时序号为0，不为0的序号即标记



- 面包店：每个进入商店的客户都获得一个号码，号码最小的先得到服务，号码相同，名字靠前的先服务

![image-20211116204618025](C:\Users\10505\AppData\Roaming\Typora\typora-user-images\image-20211116204618025.png)





临界区保护的另一类解法...

- 再想一下临界区：只允许一个进程进入，进入另一个进程以为着什么？
- 被调度：另一个进程只能被调度才能执行，才可能进入临界区，如何阻止调度？

关中断（单CPU）

```
cli();
临界区
sti();
剩余区
```

什么时候不好使？

多核CPU不好使



临界区保护的硬件原子指令法——通过硬件实现命令原子实现

CAS的本质



修改mutex用什么保护

- CAS——硬件原子指令法——spin锁

mutex是死锁信号量







### 信号量的代码实现



进程把自己睡眠，就是将PCB.current 改为睡眠状态，然后把自己放到阻塞队列中，然后CPU 切到其他进程执行



sleep_on 形成的队列



通过PCB 里的内核栈，tmp指向下一个执行的进程task_struct 并不断的套娃





### 死锁处理（Deadlock）

计算机核心工作：进程向前推进

- 互斥使用（资源的固有特性）
- 不可抢占（资源只能自愿放弃）
- 请求和保持（请求必须占有资源，再去申请）
- 循环等待（在资源分配图中存在一个环路）



死锁处理方法概述

- 死锁预防（破坏死锁出现的条件）
- 死锁避免（检测每个资源请求，如果造成死锁就拒绝）
- 死锁检测+恢复（检测到死锁出现时，让一些进程回滚，让出资源）
- 死锁忽略（在太阳上可以对火灾全然不顾，如果服务器经常不重启、不关机，这操作不可取）（死锁是否可复现，是否为大概率事件）



#### 死锁预防

- 在进程执行前，一次性申请所有需要的资源，不会占有资源再去申请其它资源

  - 缺点1：需要预支未来，编程困难

  - 缺点2：许多资源分配后很长时间才使用，资源利用率低

- 对资源类型进行排序，资源必须按序进行，不会出现环路等待

  - 缺点：仍然造成资源浪费



#### 死锁避免

判断此次请求是否引起死锁？

- 如果系统中的所有进程存在一个可完成的执行序列拍P1,...,Pn，则称系统处于安全状态
- 安全序列

|      | Allocation | Need  | Avaiable |
| ---- | ---------- | ----- | -------- |
|      | A B C      | A B C | A B C    |
| P0   | 0 1 0      | 7 4 3 | 2 3 0    |
| P1   | 3 0 2      | 0 2 0 |          |
| P2   | 3 0 2      | 6 0 0 |          |
| P3   | 2 1 1      | 0 1 0 |          |
| P4   | 0 0 2      | 4 3 1 |          |

找安全序列的银行家算法（Dijkstra提出）

```c
int Available[1..m];	//每种资源剩余数量
int Allocation[1..n, 1..m];	//已分配资源数量
int Need[1..n, 1..m];	//进程还需的各种资源数量
int Word[1..m];	//工作向量
bool Finish[1..n];	//进程是否结束
```



```
Work = Available;
Finsih[1..n] = false;
while (true) {
	for (i = 1;i <= n;i++) {
		if (Finish[i] == false && Need[i] <= Work) {
			Work = Work + Allocation[i];
			Finish[i] = true;
			break;
		} else {goto end;}
	}
}
End: for (i = 1;i <= n;i++)
		if (Finish[i] == false)return "deadlock";
时间复杂度T(n) = O(m*(n^2))
m是资源的个数，n是进程的个数
```



请求出现时：首先假装分配，然后调用银行家算法

时间复杂度高



#### 死锁检测 + 恢复

发现问题再处理

- 基本原因：每次申请都执行O(m * (n^2))，效率低。发现问题再处理

  - 定时检测或者是发现资源利用率低时检测

  ```c
  Finsih[1..n] = false;
  if (Allocation[i] == 0) Finish[i] = true;
  ...//和banker 算法完全一样
  for (i = 1;i <= n;i++) {
  	if (Finish[i] == false) {
  		deadlock = deadlock + {i};
  	}
  }
  ```

- 选择哪些进程回滚？优先级？占用资源多的？
- 如何实现回滚，那些已经修改的文件怎么办？



#### 死锁忽略（经常）

- 死锁出现不是确定的，又可以用重启动来处理死锁

> 许多通用操作系统，如PC机上安装Windows和Linux，都采用死锁忽略的方法



































