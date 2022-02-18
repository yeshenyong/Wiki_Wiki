

### 保护模式下分段机制转换

![图片](https://mmbiz.qpic.cn/mmbiz_png/GLeh42uInXSQzoHRAf3PqxWI3MhZhAdX2dAhd1eRCZtMnib7bibIJMth2KiaCMKQb9Iheib0l1asWZasZlB4qwZCxA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)	这是在没有开启分页机制的时候，只需要经过这一步转换即可得到最终的物理地址了，但是在开启了分页机制后，又会**多一步转换**。



虚拟内存

![图片](https://mmbiz.qpic.cn/mmbiz_png/GLeh42uInXSQzoHRAf3PqxWI3MhZhAdXMmaEo1mxwqQF8ibvKK8OzaA47z3JgDO6kKLffIoKmuDXqOHiaBtayw5g/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

 

也就是说，CPU 在看到我们给出的内存地址后，首先把线性地址被拆分成



高 10 位：中间 10 位：后 12 位



高 10 位负责在**页目录表**中找到一个**页目录项**，这个页目录项的值加上中间 10 位拼接后的地址去**页表**中去寻找一个**页表项**，这个页表项的值，再加上后 12 位偏移地址，就是最终的物理地址。

 

而这一切的操作，都由计算机的一个硬件叫 **MMU**，中文名字叫**内存管理单元**，有时也叫 PMMU，分页内存管理单元。由这个部件来负责将虚拟地址转换为物理地址。



之后再开启分页机制的开关。其实就是更改 **cr0** 寄存器中的一位即可（31 位），还记得我们开启保护模式么，也是改这个寄存器中的一位的值。

 

![图片](https://mmbiz.qpic.cn/mmbiz_png/GLeh42uInXSQzoHRAf3PqxWI3MhZhAdXKz6I57yxNmia3zvaEFqW3icAyoialasB4OxVnlkNKn6dgic8hXr05x66xA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

 

然后，MMU 就可以帮我们进行分页的转换了。此后指令中的内存地址（就是程序员提供的逻辑地址），就统统要先经过分段机制的转换，再通过分页机制的转换，才能最终变成物理地址。

 

所以这段代码，就是帮我们把页表和页目录表在内存中写好，之后开启 cr0 寄存器的分页开关，仅此而已，我们再把代码贴上来。

**分段机制**在之前几回已经讨论过多次了，其目的是为了为每个程序或任务提供单独的代码段（cs）、数据段（ds）、栈段（ss），使其不会相互干扰。

 

**分页机制**是本回讲的内容，开机后分页机制默认是关闭状态，需要我们手动开启，并且设置好页目录表（PDE）和页表（PTE）。其目的在于可以按需使用物理内存，同时也可以在多任务时起到隔离的作用，这个在后面将多任务时将会有所体会。

 

在 Intel 的保护模式下，分段机制是没有开启和关闭一说的，它必须存在，而分页机制是可以选择开启或关闭的。所以如果有人和你说，它实现了一个没有分段机制的操作系统，那一定是个外行。

 

再说说那些地址：

 

**逻辑地址**：我们程序员写代码时给出的地址叫逻辑地址，其中包含段选择子和偏移地址两部分。

 

**线性地址**：通过分段机制，将逻辑地址转换后的地址，叫做线性地址。而这个线性地址是有个范围的，这个范围就叫做线性地址空间，32 位模式下，线性地址空间就是 4G。

 

**物理地址**：就是真正在内存中的地址，它也是有范围的，叫做物理地址空间。那这个范围的大小，就取决于你的内存有多大了。

 

**虚拟地址**：如果没有开启分页机制，那么线性地址就和物理地址是一一对应的，可以理解为相等。如果开启了分页机制，那么线性地址将被视为虚拟地址，这个虚拟地址将会通过分页机制的转换，最终转换成物理地址



![图片](https://mmbiz.qpic.cn/mmbiz_png/GLeh42uInXTKgeIUk0LzibxicZpkMjaibvPpeqVNqRxMB1Rl72zuqV2NHFTSow38RoQUEtj5pp6GdV9qTAP9ZdbVw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)



我把这些称为**进入内核前的苦力活**，经过这样的流程，内存被搞成了这个样子。

 

![图片](https://mmbiz.qpic.cn/mmbiz_png/GLeh42uInXTKgeIUk0LzibxicZpkMjaibvPLtdFhIBr7wlQnFRFQD4LO1o4h22ictTF3zkQZA917U7Obia8jszKian3g/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)



**分段**和**分页**，就是 Intel 管理内存的两大利器，也是内存管理最最最最底层的支撑。



而 Intel 本身对于访问内存就分成三类：

**代码**

**数据**

**栈**



而 Intel 也提供了三个段寄存器来分别对应着三类内存：



**代码段寄存器（cs）**

**数据段寄存器（ds）**

**栈段寄存器（ss）**



具体来说：



**cs:eip** 表示了我们要执行哪里的代码。

**ds:xxx** 表示了我们要访问哪里的数据。

**ss:esp** 表示了我们的栈顶地址在哪里。





