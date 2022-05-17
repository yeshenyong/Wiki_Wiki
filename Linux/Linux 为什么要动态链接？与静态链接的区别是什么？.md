# Linux 为什么要动态链接？与静态链接的区别是什么？

摘自：程序喵学习宝典

先抛出几个问题：

-  为什么要进行动态链接？
- 如何进行动态链接？
- 什么是地址无关代码技术？
- 什么是延迟绑定技术？
- 如果在程序运行过程中进行显式链接？



### 为什么要进行动态链接

因为静态链接有缺点：

1. 浪费内存和磁盘空间：如下图

![img](https://pic3.zhimg.com/80/v2-4c0aaad8f34d29aab34303895a93c67e_720w.jpg)

Program1和Program2分别包含Program1.o和Program2.o两个模块，他们都需要Lib.o模块。静态链接情况下，两个目标文件都用到Lib.o这个模块，所以它们同时在链接输出的可执行文件Program1和program2中有副本，同时运行时，Lib.o在磁盘和内存中有两份副本，当系统中有大量类似Lib.o的多个程序共享目标文件时，就会浪费很大空间。

1. 静态链接对程序的更新部署和发布很不友好：假如一个模块依赖20个模块，当20个模块其中有一个模块需要更新时，需要将所有的模块都找出来重新编译出一个可执行程序才可以更新成功，每次更新任何一个模块，用户就需要重新获得一个非常大的程序，程序如果使用静态链接，那么通过网络来更新程序也会非常不便，一旦程序任何位置有一个小改动，都会导致整个程序重新下载

为了解决静态链接的缺点，所以引入了动态链接，动态链接的内存分布如图

![img](https://pic4.zhimg.com/80/v2-057b9c9350627e409b4fdbb783c33f13_720w.jpg)

多个程序依赖同一个共享目标文件，这个共享目标文件在磁盘和内存中仅有一份，不会产生副本，简单来讲就是不像静态链接一样对那些组成程序的目标文件进行链接，等到程序要运行时才进行链接，把链接这个过程推迟到运行时才执行。动态链接的方式使得开发过程中各个模块更加独立，耦合度更小，便于不同的开发者和开发组织之间独立的进行开发和测试

### 如何进行动态链接

看如下代码：

```c++
// lib.c
#include <stdio.h>

void func(int i) {
    printf("func %d \n", i);
}
// Program.c
void func(int i);

int main() {
    func(1);
    return 0;
}
```

编译运行过程如下：

```shell
$ gcc -fPIC -shared -o lib.so lib.c
$ gcc -o test Program.c ./lib.so
$ ./test
$ func 1
```

通过-fPIC和-shared可以生成一个动态链接库，再链接到可执行程序就可以正常运行。

通过readelf命令可以查看动态链接库的segment信息：

```shell
~/test$ readelf -l lib.so

Elf file type is DYN (Shared object file)
Entry point 0x530
There are 7 program headers, starting at offset 64

Program Headers:
  Type           Offset             VirtAddr           PhysAddr
                 FileSiz            MemSiz              Flags  Align
  LOAD           0x0000000000000000 0x0000000000000000 0x0000000000000000
                 0x00000000000006e4 0x00000000000006e4  R E    0x200000
  LOAD           0x0000000000000e10 0x0000000000200e10 0x0000000000200e10
                 0x0000000000000218 0x0000000000000220  RW     0x200000
  DYNAMIC        0x0000000000000e20 0x0000000000200e20 0x0000000000200e20
                 0x00000000000001c0 0x00000000000001c0  RW     0x8
  NOTE           0x00000000000001c8 0x00000000000001c8 0x00000000000001c8
                 0x0000000000000024 0x0000000000000024  R      0x4
  GNU_EH_FRAME   0x0000000000000644 0x0000000000000644 0x0000000000000644
                 0x0000000000000024 0x0000000000000024  R      0x4
  GNU_STACK      0x0000000000000000 0x0000000000000000 0x0000000000000000
                 0x0000000000000000 0x0000000000000000  RW     0x10
  GNU_RELRO      0x0000000000000e10 0x0000000000200e10 0x0000000000200e10
                 0x00000000000001f0 0x00000000000001f0  R      0x1

 Section to Segment mapping:
  Segment Sections...
   00     .note.gnu.build-id .gnu.hash .dynsym .dynstr .gnu.version .gnu.version_r .rela.dyn .rela.plt .init .plt .plt.got .text .fini .rodata .eh_frame_hdr .eh_frame
   01     .init_array .fini_array .dynamic .got .got.plt .data .bss
   02     .dynamic
   03     .note.gnu.build-id
   04     .eh_frame_hdr
   05
   06     .init_array .fini_array .dynamic .got
```

可以看见动态链接模块的装载地址从0开始，0是无效地址，它的装载地址会在程序运行时再确定，在编译时是不确定的

改一下程序：

```c++
// Program.c
#include <stdio.h>
void func(int i);

int main() {
    func(1);
    sleep(-1);
    return 0;
}
```

运行读取maps信息：

```shell
~/test$ ./test &
[1] 126
~/test$ func 1
cat /proc/126/maps
7ff2c59f0000-7ff2c5bd7000 r-xp 00000000 00:00 516391             /lib/x86_64-linux-gnu/libc-2.27.so
7ff2c5bd7000-7ff2c5be0000 ---p 001e7000 00:00 516391             /lib/x86_64-linux-gnu/libc-2.27.so
7ff2c5be0000-7ff2c5dd7000 ---p 000001f0 00:00 516391             /lib/x86_64-linux-gnu/libc-2.27.so
7ff2c5dd7000-7ff2c5ddb000 r--p 001e7000 00:00 516391             /lib/x86_64-linux-gnu/libc-2.27.so
7ff2c5ddb000-7ff2c5ddd000 rw-p 001eb000 00:00 516391             /lib/x86_64-linux-gnu/libc-2.27.so
7ff2c5ddd000-7ff2c5de1000 rw-p 00000000 00:00 0
7ff2c5df0000-7ff2c5df1000 r-xp 00000000 00:00 189022             /mnt/d/wzq/wzq/util/test/lib.so
7ff2c5df1000-7ff2c5df2000 ---p 00001000 00:00 189022             /mnt/d/wzq/wzq/util/test/lib.so
7ff2c5df2000-7ff2c5ff0000 ---p 00000002 00:00 189022             /mnt/d/wzq/wzq/util/test/lib.so
7ff2c5ff0000-7ff2c5ff1000 r--p 00000000 00:00 189022             /mnt/d/wzq/wzq/util/test/lib.so
7ff2c5ff1000-7ff2c5ff2000 rw-p 00001000 00:00 189022             /mnt/d/wzq/wzq/util/test/lib.so
7ff2c6000000-7ff2c6026000 r-xp 00000000 00:00 516353             /lib/x86_64-linux-gnu/ld-2.27.so
7ff2c6026000-7ff2c6027000 r-xp 00026000 00:00 516353             /lib/x86_64-linux-gnu/ld-2.27.so
7ff2c6227000-7ff2c6228000 r--p 00027000 00:00 516353             /lib/x86_64-linux-gnu/ld-2.27.so
7ff2c6228000-7ff2c6229000 rw-p 00028000 00:00 516353             /lib/x86_64-linux-gnu/ld-2.27.so
7ff2c6229000-7ff2c622a000 rw-p 00000000 00:00 0
7ff2c62e0000-7ff2c62e3000 rw-p 00000000 00:00 0
7ff2c62f0000-7ff2c62f2000 rw-p 00000000 00:00 0
7ff2c6400000-7ff2c6401000 r-xp 00000000 00:00 189023             /mnt/d/wzq/wzq/util/test/test
7ff2c6600000-7ff2c6601000 r--p 00000000 00:00 189023             /mnt/d/wzq/wzq/util/test/test
7ff2c6601000-7ff2c6602000 rw-p 00001000 00:00 189023             /mnt/d/wzq/wzq/util/test/test
7fffee96f000-7fffee990000 rw-p 00000000 00:00 0                  [heap]
7ffff6417000-7ffff6c17000 rw-p 00000000 00:00 0                  [stack]
7ffff729d000-7ffff729e000 r-xp 00000000 00:00 0                  [vdso]
```

可以看到，整个进程虚拟地址空间中，多出了几个文件的映射，lib.so和test一样，它们都是被操作系统用同样的方法映射到进程的虚拟地址空间，只是它们占据的虚拟地址和长度不同，从maps里可以看见里面还有libc-2.27.so，这是C语言运行库，还有一个ld-2.27.so，这是Linux下的动态链接器，动态链接器和普通共享对象一样被映射到进程的地址空间，在系统开始运行test前，会先把控制权交给动态链接器，动态链接器完成所有的动态链接工作后会把控制权交给test，然后执行test程序。

当链接器将Program.o链接成可执行文件时，这时候链接器必须确定目标文件中所引用的func函数的性质，如果是一个定义于其它静态目标文件中的函数，那么链接器将会按照静态链接的规则，将Program.o的func函数地址进行重定位，如果func是一个定义在某个动态链接共享对象中的函数，那么链接器将会将这个符号的引用标记为一个动态链接的符号，不对它进行地址重定位，将这个过程留在装载时再进行。



### 动态链接的方式

动态链接有两种方式：装载时重定位和地址无关代码技术

装载时重定位：在链接时对所有绝对地址的引用不作重定位，而把这一步推迟到装载时完成，也叫基址重置，每个指令和数据相当于模块装载地址是固定的，系统会分配足够大的空间给装载模块，当装载地址确定后，那指令和数据地址自然也就确定了。然而动态链接模块被装载映射到虚拟空间，指令被重定位后对于每个进程来讲是不同的，没有办法做到同一份指令被多个进程共享，所以指令对不同的进程来说有不同的副本，还是空间浪费，怎么解决这个问题？**使用fPIC方法。**

地址无关代码：指令部分无法在多个进程之间共享，不能节省内存，所以引入了地址无关代码的技术。我们平时编程过程中可能都见过-fPIC的编译选项，这个就代表使用了地址无关代码技术来实现真正的动态链接。基本思想就是使用GOT（全局偏移表），这是一个指向变量或函数地址的指针数组，当指令要访问变量或者调用函数时，会去GOT中找到相应的地址进行间接跳转访问，每个变量或函数都对应一个地址，链接器在装载模块的时候会查找每个变量和函数的地址，然后填充GOT中的各个项，确保每个指针指向的地址正确。GOT放在数据段，所以它可以在模块装载时被修改，并且每个进程都可以有独立的副本，相互不受影响。

**tips：**

-fpic和-fPIC的区别：它们都是地址无关代码技术，-fpic产生的代码相对较小较快，但是在某些平台会有些限制，所以大多数情况下都是用-fPIC来产生地址无关代码。

-fPIC和-fPIE的区别：一个作用于共享对象，一个作用于可执行文件，一个以地址无关方式编译的可执行文件被称作地址无关可执行文件。

-fpie和-fPIE的区别：类似于-fpic和-fPIC的区别

### 延迟绑定技术

在程序刚启动时动态链接器会寻找并装载所需要的共享对象，然后进行符号地址寻址重定位等工作，这些工作会减慢程序的启动速度，如果解决？

使用PLT延迟绑定技术，这里会单独有一个叫.PLT的段，ELF将 GOT拆分成两个表.GOT和.GOT.PLT，其中.GOT用来保存全局变量的引用地址，.GOT.PLT用来保存外部函数的地址，每个外部函数在PLT中都有一个对应项，在初始化时不会绑定，而是在函数第一次被用到时才进行绑定，将函数真实地址与对应表项进行绑定，之后就可以进行间接跳转过去。

### 显式运行时链接

支持动态链接的系统往往都支持显式运行时链接，也叫运行时加载，让程序自己在运行时控制加载的模块，在需要时加载需要的模块，在不需要时将其卸载。这种运行时加载方式使得程序的模块组织变得很灵活，可以用来实现一些诸如插件、驱动等功能。

通过这四个API可以进行显式运行时链接：

```cpp
dlopen()：打开动态链接库
dlsym()：查找符号
dlerror()：错误处理
dlclose()：关闭动态链接库
```

参考这段使用代码：

```cpp
#include <stdio.h>
#include <dlfcn.h>

int main() {

    void *handle;
    void (*f)(int);
    char *error;

    handle = dlopen("./lib.so", RTLD_NOW);
    if (handle == NULL) {
        printf("handle null \n");
        return -1;
    }
    f = dlsym(handle, "func");
    do {
        if ((error = dlerror()) != NULL) {
            printf("error\n");
            break;
        }
        f(100);
    } while (0);
    dlclose(handle);

    return 0;
}
```

编译运行：

```cpp
$ gcc -o test program.c -ldl
$ ./test
func 100
```

### 总结

为什么要进行动态链接？为了解决静态链接浪费空间和更新困难的缺点。

动态链接的方式？装载时重定位和地址无关代码技术。

地址无关代码技术原理？通过GOT段实现间接跳转。

延迟加载技术原理？对外部函数符号通过PLT段实现延迟绑定及间接跳转。

如果进行显式运行时链接？通过<dlfcn.h>头文件中的四个函数，代码如上。
