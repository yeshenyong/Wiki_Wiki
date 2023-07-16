# C++ 编译与链接

原文：https://zhuanlan.zhihu.com/p/357588753

参考：https://www.bilibili.com/video/BV1TN4y1375q

> 加以补充与修改



编译其实就是将程序运行的源代码翻译成CPU 能够直接运行的机器代码

> 编译永远都是以单个源文件为单位的
>
> > 如果单独修改了一个源文件，只需要单独编译单个源文件，而不需要浪费实践重新编译整个工程

### 单个源文件编译

```sh
gcc -c main.c # 编译成main.o (object 文件(ELF))
gcc -c math.c
ls *.o # main.o  math.o
file main.c
# main.c: C source, ASCII text
file main.o # ELF (Executable and Linkable Format) Linux 所有可执行文件的通用格式, 对二进制代码封装
# main.o: ELF 64-bit LSB relocatable, x86-64, version 1 (SYSV), not stripped
readelf -h main.o
# 可以在文件头找到可执行文件的基本信息, 如支持的操作系统、机器类型
# ELF Header:
#  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00 
#  Class:                             ELF64
#  Data:                              2's complement, little endian
#  Version:                           1 (current)
#  OS/ABI:                            UNIX - System V
#  ABI Version:                       0
#  Type:                              REL (Relocatable file)
#  Machine:                           Advanced Micro Devices X86-64
#  Version:                           0x1
#  Entry point address:               0x0
#  Start of program headers:          0 (bytes into file)
#  Start of section headers:          672 (bytes into file)
#  Flags:                             0x0
#  Size of this header:               64 (bytes)
#  Size of program headers:           0 (bytes)
#  Number of program headers:         0
#  Size of section headers:           64 (bytes)
#  Number of section headers:         14
#  Section header string table index: 13
readelf -S main.o # 区块/段(Sections 显示)
# 里面有机器代码还有程序的数据等
# .text 是代码区 (编译好的机器代码)
# .data 是数据区 (初始化的全局变量、局部静态变量)

There are 14 section headers, starting at offset 0x2a0:

Section Headers:
  [Nr] Name              Type             Address           Offset
       Size              EntSize          Flags  Link  Info  Align
  [ 0]                   NULL             0000000000000000  00000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .text             PROGBITS         0000000000000000  00000040
       0000000000000034  0000000000000000  AX       0     0     1
  [ 2] .rela.text        RELA             0000000000000000  000001c8
       0000000000000048  0000000000000018   I      11     1     8
  [ 3] .data             PROGBITS         0000000000000000  00000074
       0000000000000000  0000000000000000  WA       0     0     1
  [ 4] .bss              NOBITS           0000000000000000  00000074
       0000000000000000  0000000000000000  WA       0     0     1
  [ 5] .rodata           PROGBITS         0000000000000000  00000074
       000000000000000e  0000000000000000   A       0     0     1
  [ 6] .comment          PROGBITS         0000000000000000  00000082
       0000000000000027  0000000000000001  MS       0     0     1
  [ 7] .note.GNU-stack   PROGBITS         0000000000000000  000000a9
       0000000000000000  0000000000000000           0     0     1
  [ 8] .note.gnu.pr[...] NOTE             0000000000000000  000000b0
       0000000000000020  0000000000000000   A       0     0     8
  [ 9] .eh_frame         PROGBITS         0000000000000000  000000d0
       0000000000000038  0000000000000000   A       0     0     8
  [10] .rela.eh_frame    RELA             0000000000000000  00000210
       0000000000000018  0000000000000018   I      11     9     8
  [11] .symtab           SYMTAB           0000000000000000  00000108
       00000000000000a8  0000000000000018          12     4     8
  [12] .strtab           STRTAB           0000000000000000  000001b0
       0000000000000016  0000000000000000           0     0     1
  [13] .shstrtab         STRTAB           0000000000000000  00000228
       0000000000000074  0000000000000000           0     0     1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
  L (link order), O (extra OS processing required), G (group), T (TLS),
  C (compressed), x (unknown), o (OS specific), E (exclude),
  D (mbind), l (large), p (processor specific)
  
  # 目标文件虽然包含了编译之后的机器代码, 但它并不能够直接执行, os 也不允许执行
  # 原因
  # 1. 调用其它函数 (如add) 只是一句声明而已, 被定义在另一个模块math.c 中
  # 2. 调用基础库 (如printf) 在 #inclcude <stdio.h> 里也只是一个声明而已
  # 意味着在编译main.o 的过程中, 编译器是不知道add 和printf 的存在的, 此时编译器只能将这两个函数的跳转地址暂时先设为 0, 随后在链接的时候再去修正
chmod +x main.o
./main
# bash: ./main.o: cannot execute binary file: Exec format error
  
objdump -s -d main.o
  main.o:     file format elf64-x86-64

Contents of section .text:
 0000 f30f1efa 554889e5 4883ec10 488d0500  ....UH..H...H...
 0010 00000048 89c7e800 000000be 05000000  ...H............
 0020 bf030000 00e80000 00008945 fcb80000  ...........E....
 0030 0000c9c3                             ....            
Contents of section .rodata:
 0000 48656c6c 6f2c2057 6f726c64 2100      Hello, World!.  
Contents of section .comment:
 0000 00474343 3a202855 62756e74 75203132  .GCC: (Ubuntu 12
 0010 2e322e30 2d313775 62756e74 75312920  .2.0-17ubuntu1) 
 0020 31322e32 2e3000                      12.2.0.         
Contents of section .note.gnu.property:
 0000 04000000 10000000 05000000 474e5500  ............GNU.
 0010 020000c0 04000000 03000000 00000000  ................
Contents of section .eh_frame:
 0000 14000000 00000000 017a5200 01781001  .........zR..x..
 0010 1b0c0708 90010000 1c000000 1c000000  ................
 0020 00000000 34000000 00450e10 8602430d  ....4....E....C.
 0030 066b0c07 08000000                    .k......        

Disassembly of section .text:

0000000000000000 <main>: # 编译过后的主函数代码, 左边是机器代码, 后边是对应的反汇编
# 下面调用两个call 指令, 分别对应之前调用的printf 和add 函数 (但是它们的跳转地址都被设置成了 0, 后面会进行修正)
# 为了链接器能够定位到这些需要被修正的地址, 在代码块还能看到一个重定位表 (Relocation Table)
   0:   f3 0f 1e fa             endbr64
   4:   55                      push   %rbp
   5:   48 89 e5                mov    %rsp,%rbp
   8:   48 83 ec 10             sub    $0x10,%rsp
   c:   48 8d 05 00 00 00 00    lea    0x0(%rip),%rax        # 13 <main+0x13>
  13:   48 89 c7                mov    %rax,%rdi
  16:   e8 00 00 00 00          call   1b <main+0x1b>
  1b:   be 05 00 00 00          mov    $0x5,%esi
  20:   bf 03 00 00 00          mov    $0x3,%edi
  25:   e8 00 00 00 00          call   2a <main+0x2a>
  2a:   89 45 fc                mov    %eax,-0x4(%rbp)
  2d:   b8 00 00 00 00          mov    $0x0,%eax
  32:   c9                      leave
  33:   c3                      ret
  
objdump -r main.o

main.o:     file format elf64-x86-64

RELOCATION RECORDS FOR [.text]: # .text 区块中需要被重定位两个函数printf 和add 位于偏移量17 和26的位置, 后面是地址的类型和长度, 这和之前的编译机器代码是一一对应的
OFFSET           TYPE              VALUE
000000000000000f R_X86_64_PC32     .rodata-0x0000000000000004
0000000000000017 R_X86_64_PLT32    puts-0x0000000000000004 # printf 参数无需解析, 也即不含类似%s 之类format 时, 编译器将其转换为puts 调用, 进行简化
0000000000000026 R_X86_64_PLT32    add-0x0000000000000004


RELOCATION RECORDS FOR [.eh_frame]:
OFFSET           TYPE              VALUE
0000000000000020 R_X86_64_PC32     .text

gcc main.o math.o -o main
./main # Hello, World!
# header
# .text
# .data
# ...
# .reloc (重定位表 => .text)
# 这个时候编译器会根据目标文件或者静态库中的重定位表, 找到需要被重定位的函数、全局变量, 从而修正它们的地址

# 如果链接过程中, 没有链接定义的目标文件
gcc main.o -o main # 由于链接器找不到add 函数的实现, 于是报错 "引用未定义"或者"符号未定义"
/usr/bin/ld: main.o: in function `main':
main.c:(.text+0x26): undefined reference to `add'
collect2: error: ld returned 1 exit status
```

> makefile 的核心是对 "依赖(Dependency)" 的管理
>
> makefile 其实就是在定义一颗依赖树 (要构建最上方的这个目标就需要提供下面这些节点的文件)
>
> main => (main.o => main.c) + (math.o => math.c)



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



