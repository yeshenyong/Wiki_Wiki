# C++ 编译原理、链接原理

原文：https://zhuanlan.zhihu.com/p/357588753

> 加以补充与修改



### 单个源文件编译

单个 c/cpp 文件可以被 gcc 编译成目标文件（.o 文件），这部分就不过多赘述，大家应该都很熟悉了。二进制目标文件中的 section 有很多，详细内容可以打开汇编代码详细研究下，下图列出了其中比较常见的段。

![img](https://pic4.zhimg.com/80/v2-6cd623de1aa958a23642fd417969eb63_720w.webp)

**符号表的作用是什么？**

1. 记录该目标文件中定义的全局变量及函数；
2. 记录该目标文件中引用的全局变量及函数；

![img](https://pic3.zhimg.com/80/v2-de29a869e9a3e1887b6e555522b91966_720w.webp)

**.rela.\* 的作用是什么？**

全称 relocation（重定位），记录编译器在编译时不确定的符号地址——针对引用的外部符号。

dynamic 段中保存了可执行文件依赖哪些动态库。

GOT 段记录了需要引用的外部符号的地址



### 编译期连接

多个 .o 文件可以通过链接器（ld）被打包在一起，组合成库文件。

库文件又分为静态库（.a 文件）和共享库（.so 文件）。

什么是 ld 呢？它本身也是可执行文件，属于 GNU 的一部分，将一堆目标文件通过符号表链接成最终的目标文件、库文件和可执行文件。

**.a 文件如何生成？**

ld 直接将涉及的所有目标文件打包进静态库文件。

**.so 文件如何生成？**

在链接生成共享库文件的过程中，并不拷贝目标文件中涉及的代码段，只记录它需要引用的外部符号位置（在哪些目标文件中）。

所有的目标文件、库文件和可执行文件都有统一的格式，即 ELF，Executable and Linking Format（可执行链接格式）。

![img](https://pic1.zhimg.com/80/v2-e9dbb294b84b2df684e98a0dd126fbdc_720w.webp)

libstdc++.so 是标准库文件

上图中，多个 .o 文件链接在一起形成 .a 文件，多个 .o 和 .so 文件也可以链接形成 .so 文件，可执行文件也可以由 .a 文件、.so、.o 文件链接而成



### 动态库链接

### 动态加载

如果可执行文件没有使用共享库，那么该程序就可以独立运行，因为它内部所有的符号都有对应的二进制机器码。这种情况比较简单，我们这里主要讨论下面这种程序运行方式。

如果可执行文件要使用共享库，那么该程序就不能独立运行，它在运行时需要使用共享库的代码，且对应的两种使用方式，分别是**运行时动态链接**和**运行时动态加载**。

**可执行文件的组成**

![img](https://pic3.zhimg.com/80/v2-263fce42a0ecc75723366c9eaf3a54de_720w.webp)

ld-linux.so：不是一个可执行程序，只是一个 shell 脚本。作为解释器，写在 elf 文件（可执行文件）中，ld-linux.so 先于 main 函数工作，用于查找主程序所依赖的共享库，实际上可以直接执行 ld-linux.so. 还有另外一种比较常见的是 ld.so，它是个符号链接，指向 ld-linux.so.（通过命令 ln -s ld.so ld-linux.so 创建）。

![img](https://pic2.zhimg.com/80/v2-03c0e5a7b82d4c9429c2b058822592d5_720w.webp)

关键区别是：动态链接的流程是 OS 直接把共享库的代码拷贝到内存，而动态加载由人工指定（代码中的 dlopen() 接口）。

动态链接需要 OS 的特殊支持，通过动态链接方式拷贝到内存的库代码可以在各个进程之间共享。而对动态加载而言，可以在各自进程中打开共享库代码。



### 如何打包静态库动态库

生成静态库

首先，将代码文件编译成目标文件.o（StaticMath.o）

```sh
g++ -c StaticMath.cpp
```

注意带参数-c，否则直接编译为可执行文件

- 然后，通过ar工具将目标文件打包成.a静态库文件

```sh
ar -crv libstaticmath.a StaticMath.o
```

生成静态库libstaticmath.a

Linux下使用静态库，只需要在编译的时候，**指定静态库的搜索路径（-L选项）、指定静态库名（不需要lib前缀和.a后缀，-l选项）**

```sh
g++ TestStaticLibrary.cpp -L../StaticLibrary -lstaticmath
```



生成动态库

首先，生成目标文件，此时要加编译器选项-fpic

```sh
g++ -fPIC -c DynamicMath.cpp
```

-fPIC 创建与地址无关的编译程序（pic，position independent code），是为了能够在多个应用程序间共享

```sh
g++ -shared -o libdynmath.so DynamicMath.o
```

-shared指定生成动态链接库

其实上面两个步骤可以合并为一个命令：

```sh
g++ -fPIC -shared -o libdynmath.so DynamicMath.cpp
```

引用动态库编译成可执行文件（跟静态库方式一样）：

```sh
g++ TestDynamicLibrary.cpp -L../DynamicLibrary -ldynmath
```

> 然后运行：./a.out，发现竟然报错了！！！
>
>  
>
> 可能大家会猜测，是因为动态库跟测试程序不是一个目录，那我们验证下是否如此：
>
>  
>
> 发现还是报错！！！那么，在执行的时候是如何定位共享库文件的呢？
>
> 1)        当系统加载可执行代码时候，能够知道其所依赖的库的名字，但是还需要知道绝对路径。此时就需要系统动态载入器(dynamic linker/loader)。
>
> 2)        **对于elf格式的可执行程序，是由ld-linux.so*来完成的，它先后搜索elf文件的 DT_RPATH段—环境变量LD_LIBRARY_PATH—/etc/ld.so.cache文件列表—/lib/,/usr/lib 目录找到库文件后将其载入内存**。
>
> 如何让系统能够找到它：
>
> - 如果安装在/lib或者/usr/lib下，那么ld默认能够找到，无需其他操作。
> - 如果安装在其他目录，需要将其添加到/etc/ld.so.cache文件中，步骤如下：
> - 编辑/etc/ld.so.conf文件，加入库文件所在目录的路径
> - 运行ldconfig ，该命令会重建/etc/ld.so.cache文件
>
> 我们将创建的动态库复制到/usr/lib下面，然后运行测试程序
>
> 亦可通过 `echo $LD_LIBRARY_PATH` 在当前终端生效，关掉就不生效了







### 应用层

设置链接库路径

```sh
`echo $LD_LIBRARY_PATH`，可通过该环境变量直接设置。
或者，`gcc --print-search-dirs`
```

> ldconfig：这是个可执行程序，隶属于 GNU，作用是在默认搜寻目录（/lib和/usr/lib）以及共享库配置文件 /etc/ld.so.conf 内所列的目录下，搜索出共享库文件（lib*.so*），进而创建出 ld-linux.so 所需要的链接和缓存文件。缓存文件默认为 **/etc/ld.so.cache**，此文件保存已排好序的共享库名字列表。更新缓存使新添加的库生效，当然系统启动时会自动运行 ldconfig



ldd：这是 Linux 内核中自带的脚本，可以用来查看可执行文件链接了哪些共享库

```sh
ldd libc.so.6
        /lib64/ld-linux-x86-64.so.2 (0x00007f4948a68000)
        linux-vdso.so.1 (0x00007fffdce29000)
```



objdump：反汇编命令（用查看目标文件或者可执行的目标文件的构成的gcc工具）

对应使用

```sh
gcc -c -o main.o main.c
objdump -s -d main.o > main.o.txt
```



nm：nm 命令是name 的缩写，不是 nima的缩写（记住了）

显示**指定文件中的符号信息**，文件可以是对象文件、可执行文件或对象文件库。如果文件中没有包含符号信息，nm报告该情况，单不把他解释为出错。nm缺省情况下报告十进制符号表示法下的数字值



