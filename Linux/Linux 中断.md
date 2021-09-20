# Linux 中断





> 上下文环境，就是描述当前正在执行的程序相关的静态信息，比如：有哪些代码段，栈空间在哪里，进程描述信息在什么位置，当前执行到哪一条指令等等。



> ​	实模式和保护模式



那么，中断向量就是工作在实模式下的，处理器通过中断号和中断向量，来定位到相应的中断处理程序。

而中断描述符呢，就是工作在保护模式下，处理器通过中断号和中断描述符，来定位到相应的中断处理程序。

也就是说：中断向量和中断描述符，它俩的根本作用是一样的。



## **中断的分类**



![图片](https://mmbiz.qpic.cn/mmbiz_png/WC13ibsIvG3adXgDp6bxSJDmv4oS0NYxmtYYibUPvTPeAq9zib1DA0ZnhE1NV1uUvFzticTccWbSTIq7ImBuzliaoLw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)



#### 内部中断

所谓的内部中断，是在 `CPU` 内部产生并进行处理的。比如：

1. CPU 遇到一条除以 0 的指令时，将产生 0 号中断，并调用相应的中断处理程序;
2. CPU 遇到一条不存在的非法指令时，将产生 6 号中断，并调用相应的中断处理程序;

对于内部中断，有时候也称之为异常。

**软中断**也属于内部中断，是非常有用的，它是由 `int` 指令触发的。比如 `int3` 这条指令，`gdb` 就是利用它来实现对应用程序的调试。

​	描述了 `gdb` 是如何通过插入一个 `int` 指令，来替换被调试程序的指令码，从而实现断点调试功能的。

![图片](https://mmbiz.qpic.cn/mmbiz_png/WC13ibsIvG3adXgDp6bxSJDmv4oS0NYxmlibpYDKamexSHYYHFXtib1vxaj9cCCDZTLZsibGCxk0qVsrqefZqC48dw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)





#### 外部中断

`x86` `CPU` 上有 `2` 个中断引脚：`INT` 和 `INTR`，分别对应：不可屏蔽中断和可屏蔽中断。

所谓不可屏蔽，就是说：中断不可以被忽视，`CPU` 必须处理这个中断。

如果不处理，程序就没法继续执行。

而对于可屏蔽中断，`CPU` 可以忽略它不执行，因为这类中断不会对系统的执行造成致命的影响。



​	对于外部的可屏蔽中断，`CPU` 上只有一根 `INTR` 引脚，但是需要产生中断信号的设备那么多，如何对众多的中断信号进行区分呢？

一般都是通过可编程中断控制器(`Programmable Interrupt Controller, PIC`)，在计算机中使用最多的就是 `8259a` 芯片。

虽然现代计算机都已经是 `APIC`(高级可编程中断控制器) 了，但是由于 `8259a` 芯片是那么的经典，大部分描述外部中断的文章都会用它来举例。

每一片 `8259a` 可以提供 `8` 个中断输入引脚，两片芯片级联在一起，就可以提供 `15` 个中断信号：

![图片](https://mmbiz.qpic.cn/mmbiz_png/WC13ibsIvG3adXgDp6bxSJDmv4oS0NYxmZxzqiaRNmUdPapkW53ZosNTSTFeDadDfQaKoJWnaLKicuic5zerBz8Kdg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

## **中断号**

在 `x86` 处理器中，一共支持 `256` 个中断，每一个中断都分配了一个中断号，从 `0` 到 `255`。

其中，`0 ~ 31` 号中断向量被保留，用来处理异常和非屏蔽中断(其中只有 `2` 号向量用于非屏蔽中断，其余全部是异常)。

当 `BIOS` 或者操作系统提供了异常处理程序之后，当一个异常产生时，就会通过中断向量表找到响应的异常处理程序，查找的过程马上就会介绍到。

从中断号 `32` 开始，全部分配给外部中断。

比如：

1. 系统定时器中断 IRQ0，分配的就是 32 号中断;

2. Linux 的系统调用，分配的就是 128 号中断;

我们来分别看一下内部中断和外部中断相关的中断号：

![图片](https://mmbiz.qpic.cn/mmbiz_png/WC13ibsIvG3adXgDp6bxSJDmv4oS0NYxmhtN1r75B1xM86Hp7sZXd5AmZ95hH4jxRUP2qOOE3CWV6iawTdrpKBmA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)



