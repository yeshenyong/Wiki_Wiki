# Linux 中的零拷贝技术

### 什么是零拷贝

维基上是这么描述零拷贝的：零拷贝描述的是CPU不执行拷贝数据从一个存储区域到另一个存储区域的任务，这通常用于通过网络传输一个文件时以减少CPU周期和内存带宽。

### 传统I/O

在写一个服务端程序时（Web Server或者文件服务器），文件下载是一个基本功能。这时候服务端的任务是：**将服务端主机磁盘中的文件不做修改地从已连接的socket发出去**，我们通常用下面的代码完成：

```c++
while((n = read(diskfd, buf, BUF_SIZE)) > 0)
    write(sockfd, buf , n);
```

基本操作就是循环的从磁盘读入文件内容到缓冲区，再将缓冲区的内容发送到`socket`。但是由于Linux的`I/O`操作默认是缓冲`I/O`。这里面主要使用的也就是`read`和`write`两个系统调用，我们并不知道操作系统在其中做了什么。实际上在以上`I/O`操作中，发生了多次的数据拷贝。

​	当应用程序访问某块数据时，操作系统首先会检查，是不是最近访问过此文件，文件内容是否缓存在内核缓冲区，如果是，操作系统则直接根据`read`系统调用提供的`buf`地址，将内核缓冲区的内容拷贝到`buf`所指定的用户空间缓冲区中去。如果不是，操作系统则首先将磁盘上的数据拷贝的内核缓冲区，这一步目前主要依靠`DMA`来传输，然后再把内核缓冲区上的内容拷贝到用户缓冲区中。
 	接下来，`write`系统调用再把用户缓冲区的内容拷贝到网络堆栈相关的内核缓冲区中，最后`socket`再把内核缓冲区的内容发送到网卡上。
 说了这么多，不如看图清楚：

![img](https://upload-images.jianshu.io/upload_images/272719-9b800f62a9c0e47d.PNG?imageMogr2/auto-orient/strip|imageView2/2/w/544/format/webp)





从上图中可以看出，共产生了四次数据拷贝，即使使用了`DMA`来处理了与硬件的通讯，CPU仍然需要处理两次数据拷贝，与此同时，在用户态与内核态也发生了多次上下文切换，无疑也加重了CPU负担。

![img](https://upload-images.jianshu.io/upload_images/4235178-40631870dd4c58db.jpeg?imageMogr2/auto-orient/strip|imageView2/2/w/426/format/webp)

① 发出read系统调用：导致用户空间到内核空间的上下文切换(第一次上下文切换)。通过DMA引擎将文件中的数据从磁盘上读取到内核空间缓冲区(第一次拷贝: hard drive ——> kernel buffer)。
 ② 将内核空间缓冲区的数据拷贝到用户空间缓冲区(第二次拷贝: kernel buffer ——> user buffer)，然后read系统调用返回。而系统调用的返回又会导致一次内核空间到用户空间的上下文切换(第二次上下文切换)。
 ③ 发出write系统调用：导致用户空间到内核空间的上下文切换(第三次上下文切换)。将用户空间缓冲区中的数据拷贝到内核空间中与socket相关联的缓冲区中(即，第②步中从内核空间缓冲区拷贝而来的数据原封不动的再次拷贝到内核空间的socket缓冲区中。)(第三次拷贝: user buffer ——> socket buffer)。
 ④ write系统调用返回，导致内核空间到用户空间的再次上下文切换(第四次上下文切换)。通过DMA引擎将内核缓冲区中的数据传递到协议引擎(第四次拷贝: socket buffer ——> protocol engine)，这次拷贝是一个独立且异步的过程。

**总的来说，传统的I/O操作进行了4次用户空间与内核空间的上下文切换，以及4次数据拷贝。其中4次数据拷贝中包括了2次DMA拷贝和2次CPU拷贝。**

在此过程中，我们没有对文件内容做任何修改，那么在内核空间和用户空间来回拷贝数据无疑就是一种浪费，而零拷贝主要就是为了解决这种低效性。



## **DMA**

​	DMA(Direct Memory Access) ———— 直接内存访问 ：DMA是允许外设组件将I/O数据直接传送到主存储器中并且传输不需要CPU的参与，以此将CPU解放出来去完成其他的事情。
 而用户空间与内核空间之间的数据传输并没有类似DMA这种可以不需要CPU参与的传输工具，因此用户空间与内核空间之间的数据传输是需要CPU全程参与的。所有也就有了通过零拷贝技术来减少和避免不必要的CPU数据拷贝过程。



### 通过sendfile实现的零拷贝I/O

![img](https://upload-images.jianshu.io/upload_images/272719-5c49aebc85085726.PNG?imageMogr2/auto-orient/strip|imageView2/2/w/626/format/webp)

![img](https://upload-images.jianshu.io/upload_images/4235178-66c23adafbfbd47f.jpeg?imageMogr2/auto-orient/strip|imageView2/2/w/418/format/webp)

① 发出sendfile系统调用，导致用户空间到内核空间的上下文切换(第一次上下文切换)。通过DMA引擎将磁盘文件中的内容拷贝到内核空间缓冲区中(第一次拷贝: hard drive ——> kernel buffer)。然后再将数据从内核空间缓冲区拷贝到内核中与socket相关的缓冲区中(第二次拷贝: kernel buffer ——> socket buffer)。
 ② sendfile系统调用返回，导致内核空间到用户空间的上下文切换(第二次上下文切换)。通过DMA引擎将内核空间socket缓冲区中的数据传递到协议引擎(第三次拷贝: socket buffer ——> protocol engine)

**总的来说，通过sendfile实现的零拷贝I/O只使用了2次用户空间与内核空间的上下文切换，以及3次数据的拷贝。其中3次数据拷贝中包括了2次DMA拷贝和1次CPU拷贝。**



### 带有DMA收集拷贝功能的sendfile实现的I/O

从Linux 2.4版本开始，操作系统底层提供了带有scatter/gather的DMA来从内核空间缓冲区中将数据读取到协议引擎中。这样一来待传输的数据可以分散在存储的不同位置上，而不需要在连续存储中存放。那么从文件中读出的数据就根本不需要被拷贝到socket缓冲区中去，只是需要将缓冲区描述符添加到socket缓冲区中去，DMA收集操作会根据缓冲区描述符中的信息将内核空间中的数据直接拷贝到协议引擎中。

![img](https://upload-images.jianshu.io/upload_images/272719-8461cc4141c8dd45.PNG?imageMogr2/auto-orient/strip|imageView2/2/w/833/format/webp)



![img](https://upload-images.jianshu.io/upload_images/4235178-df9323d3ae59b8f8.jpeg?imageMogr2/auto-orient/strip|imageView2/2/w/432/format/webp)

① 发出sendfile系统调用，导致用户空间到内核空间的上下文切换(第一次上下文切换)。通过DMA引擎将磁盘文件中的内容拷贝到内核空间缓冲区中(第一次拷贝: hard drive ——> kernel buffer)。
 ② 没有数据拷贝到socket缓冲区。取而代之的是只有相应的描述符信息会被拷贝到相应的socket缓冲区当中。该描述符包含了两方面的信息：a)kernel buffer的内存地址；b)kernel buffer的偏移量。
 ③ sendfile系统调用返回，导致内核空间到用户空间的上下文切换(第二次上下文切换)。DMA gather copy根据socket缓冲区中描述符提供的位置和偏移量信息直接将内核空间缓冲区中的数据拷贝到协议引擎上(第二次拷贝: kernel buffer ——> protocol engine)，这样就避免了最后一次CPU数据拷贝。

**总的来说，带有DMA收集拷贝功能的sendfile实现的I/O只使用了2次用户空间与内核空间的上下文切换，以及2次数据的拷贝，而且这2次的数据拷贝都是非CPU拷贝。这样一来我们就实现了最理想的零拷贝I/O传输了，不需要任何一次的CPU拷贝，以及最少的上下文切换。**

**关于sendfile：**

```c
#include <sys/sendfile.h>
ssize_t sendfile(int out_fd, int in_fd, off_t *offset, size_t count);
```



#### "传统I/O” VS “sendfile零拷贝I/O”

- 传统I/O通过两条系统指令read、write来完成数据的读取和传输操作，以至于产生了4次用户空间与内核空间的上下文切换的开销；而sendfile只使用了一条指令就完成了数据的读写操作，所以只产生了2次用户空间与内核空间的上下文切换。
- 传统I/O产生了2次无用的CPU拷贝，即内核空间缓存中数据与用户空间缓冲区间数据的拷贝；而sendfile最多只产出了一次CPU拷贝，即内核空间内之间的数据拷贝，甚至在底层操作体系支持的情况下，sendfile可以实现零CPU拷贝的I/O。
- 因传统I/O用户空间缓冲区中存有数据，因此应用程序能够对此数据进行修改等操作；而sendfile零拷贝消除了所有内核空间缓冲区与用户空间缓冲区之间的数据拷贝过程，因此sendfile零拷贝I/O的实现是完成在内核空间中完成的，这对于应用程序来说就无法对数据进行操作了。

Q：对于上面的第三点，如果我们需要对数据进行操作该怎么办了？
 A：Linux提供了mmap零拷贝来实现我们的需求。



### 通过mmap实现的零拷贝I/O

mmap(内存映射)是一个比sendfile昂贵但优于传统I/O的方法。

我们减少拷贝次数的一种方法是调用mmap()来代替read调用：

```c
buf = mmap(diskfd, len);
write(sockfd, buf, len);
```

![img](https://upload-images.jianshu.io/upload_images/4235178-2700bead4cf14739.jpeg?imageMogr2/auto-orient/strip|imageView2/2/w/429/format/webp)

![img](https://upload-images.jianshu.io/upload_images/272719-c955c60095647d6e.PNG?imageMogr2/auto-orient/strip|imageView2/2/w/550/format/webp)

① 发出mmap系统调用，导致用户空间到内核空间的上下文切换(第一次上下文切换)。通过DMA引擎将磁盘文件中的内容拷贝到内核空间缓冲区中(第一次拷贝: hard drive ——> kernel buffer)。
 ② mmap系统调用返回，导致内核空间到用户空间的上下文切换(第二次上下文切换)。接着用户空间和内核空间共享这个缓冲区，而不需要将数据从内核空间拷贝到用户空间。因为用户空间和内核空间共享了这个缓冲区数据，所以用户空间就可以像在操作自己缓冲区中数据一般操作这个由内核空间共享的缓冲区数据。
 ③ 发出write系统调用，导致用户空间到内核空间的上下文切换(第三次上下文切换)。将数据从内核空间缓冲区拷贝到内核空间socket相关联的缓冲区(第二次拷贝: kernel buffer ——> socket buffer)。
 ④ write系统调用返回，导致内核空间到用户空间的上下文切换(第四次上下文切换)。通过DMA引擎将内核空间socket缓冲区中的数据传递到协议引擎(第三次拷贝: socket buffer ——> protocol engine)

**总的来说，通过mmap实现的零拷贝I/O进行了4次用户空间与内核空间的上下文切换，以及3次数据拷贝。其中3次数据拷贝中包括了2次DMA拷贝和1次CPU拷贝。**

#### mmap的代价

使用mmap替代read很明显减少了一次拷贝，当拷贝数据量很大时，无疑提升了效率。但是使用`mmap`是有代价的。当你使用`mmap`时，你可能会遇到一些隐藏的陷阱。例如，当你的程序`map`了一个文件，但是当这个文件被另一个进程截断(truncate)时, write系统调用会因为访问非法地址而被`SIGBUS`信号终止。`SIGBUS`信号默认会杀死你的进程并产生一个`coredump`,如果你的服务器这样被中止了，那会产生一笔损失。



通常我们使用以下解决方案避免这种问题：

1. **为SIGBUS信号建立信号处理程序**
    当遇到`SIGBUS`信号时，信号处理程序简单地返回，`write`系统调用在被中断之前会返回已经写入的字节数，并且`errno`会被设置成success,但是这是一种糟糕的处理办法，因为你并没有解决问题的实质核心。
2. **使用文件租借锁**
    通常我们使用这种方法，在文件描述符上使用租借锁，我们为文件向内核申请一个租借锁，当其它进程想要截断这个文件时，内核会向我们发送一个实时的`RT_SIGNAL_LEASE`信号，告诉我们内核正在破坏你加持在文件上的读写锁。这样在程序访问非法内存并且被`SIGBUS`杀死之前，你的`write`系统调用会被中断。`write`会返回已经写入的字节数，并且置`errno`为success。

我们应该在`mmap`文件之前加锁，并且在操作完文件后解锁：

```c++
if(fcntl(diskfd, F_SETSIG, RT_SIGNAL_LEASE) == -1) {
    perror("kernel lease set signal");
    return -1;
}
/* l_type can be F_RDLCK F_WRLCK  加锁*/
/* l_type can be  F_UNLCK 解锁*/
if(fcntl(diskfd, F_SETLEASE, l_type)){
    perror("kernel lease set type");
    return -1;
}
```



除此之外，还有一些零拷贝技术，比如传统的Linux I/O中加上`O_DIRECT`标记可以直接`I/O`，避免了自动缓存，还有尚未成熟的`fbufs`技术，本文尚未覆盖所有零拷贝技术，只是介绍常见的一些，如有兴趣，可以自行研究，一般成熟的服务端项目也会自己改造内核中有关I/O的部分，提高自己的数据传输速率



