# IPC——进程间通信底层

### 管道



`管道` 一般用于父子进程之间相互通信，一般的用法如下：

- 父进程使用 `pipe` 系统调用创建一个管道。
- 然后父进程使用 `fork` 系统调用创建一个子进程。
- 由于子进程会继承父进程打开的文件句柄，所以父子进程可以通过新创建的管道进行通信。

![图片](https://mmbiz.qpic.cn/mmbiz_png/ciab8jTiab9J6BZXhG7ibPdIvLnOZzmSHUINC0dmrvoTLyZfxjrialsDM0lLKtYGZzHFzBCxiajo531Q5KIcIhfwiawQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

#### Linux 内核实现

##### 环形缓冲区

![图片](https://mmbiz.qpic.cn/mmbiz_png/ciab8jTiab9J6BZXhG7ibPdIvLnOZzmSHUIzZNsOSQDhmdLmDTJkJ6rkSkzicfNxYoRECaapW2yYJpFT56aicQoktFg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

在 Linux 内核中，使用了 16 个内存页作为环形缓冲区，所以这个环形缓冲区的大小为 64KB（16 * 4KB）。

##### pipe_inode_info 对象

![图片](https://mmbiz.qpic.cn/mmbiz_jpg/ciab8jTiab9J6BZXhG7ibPdIvLnOZzmSHUIAK4JQUrI4AykUlx1RyPLbr6QYmFQicezZuibGx1EWY6fmY6Zib9ZrNdIQ/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)



#### 1. 为什么父子进程可以通过管道来通信？

这是因为父子进程通过 `pipe` 系统调用打开的管道，在内核空间中指向同一个管道对象（`pipe_inode_info`）。所以父子进程共享着同一个管道对象，那么就可以通过这个共享的管道对象进行通信。共享管道对象.

#### 2. 为什么内核要使用 16 个内存页进行数据存储？

这是为了减少内存使用。

因为使用 `pipe` 系统调用打开管道时，并没有立刻申请内存页，而是当有进程向管道写入数据时，才会按需申请内存页。当内存页的数据被读取完后，内核会将此内存页回收，来减少管道对内存的使用。

























