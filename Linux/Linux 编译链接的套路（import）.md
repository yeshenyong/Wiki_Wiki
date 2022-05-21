# 原来编译链接还有这么多套路

摘自：程序喵学习宝典

如果一个程序引用了无数个动态链接库，那就有可能引入符号冲突的问题

问题如下：

![图片](https://img-blog.csdnimg.cn/img_convert/c2f5a7330fbc2b78f31c01158e3fff68.png)

实际上：

![图片](https://img-blog.csdnimg.cn/img_convert/7d8dcaba3c720b6e70abf1b477f8c0a7.png)



下面我们尝试解决它：

最开始介绍下g++基本命令参数：

```sh
g++
-c <file> 编译源文件，但是不进行链接
-o <file> 指定输出文件的名字
-s        strip，移除符号信息
-L <dir>  指令搜索链接库的路径
-l <lib>  指定要链接的链接库
-shared   产生动态目标文件
```

先来看一段代码：

```c++
#work.cc
#include  <stdio.h>

void DoThing() { printf("work.cc DoThing\n"); }
```

再定义一个简单的main.cc程序：

```c++
#main.cc
#include <stdio.h>
 
void DoThing();
 
int main() {
    printf("start \n");
    DoThing();
    printf("finished \n");
    return 0;
}
```

编译这两个文件，并分别打包成静态库：

```
g++ -c work.cc -o work.o
ar rc libwork.a work.o
g++ -c main.cc -o main.o
ar rc libmain.a main.o
```

现在将这两个静态库链接成一个可执行文件，注意链接器如果发现当前库中使用了没有被定义的符号，它只会向后查找，因此，最低级别没有其它依赖的库应该放在最右边，如果出现了符号冲突问题，链接器会使用最左边的符号。

如果这样进行链接：

```sh
$ g++ -s -L. -o main.exe -lwork -lmain
./libmain.a(main.o): In function `main':
main.cc:(.text+0x11): undefined reference to `DoThing()'
collect2: error: ld returned 1 exit status
```

链接失败，因为main库里的DoThing符号没有被定义，链接器向后查找，没有找到对应的符号定义，这里更改下链接库的顺序：

```sh
g++ -s -L. -o main.exe -lmain -lwork
$ ./main.exe
start
work.cc DoThing
finished
```

链接成功。

现在写一个简单的容易产生符号冲突的文件conflict.cc：

```c++
#include <stdio.h>

void DoThing() {printf("conflict.cc DoThing\n");}
```

编译并打包成静态库：

```sh
g++ -c conflict.cc -o conflict.o
ar rc libconflict.a conflict.o
```

如果按这样的顺序链接成一个可执行程序：

```sh
$ g++ -s -L. -o main.exe -lmain -lwork -lconflict
$ ./main.exe
start
work.cc DoThing
finished
```

如果稍微更改一下链接的顺序：

```sh
$ g++ -s -L. -o main.exe -lmain -lconflict -lwork
$ ./main.exe
start
conflict.cc DoThing
finished
```

这里发现顺序的不同导致了程序输出内容不同，究其原因就是那潜在的符号冲突。

现在再试试动态库，先介绍如何使用动态库：

```sh
$ rm libconflict.a
$ g++ -shared conflict.o -o libconflict.so
$ g++ -s -L. -o main.exe -lmain -lconflict
$ LD_LIBRARY_PATH=. ./main.exe
start
conflict.cc DoThing
finished
```

现在再引用一个中间层在动态链接库中调用conflict的文件layer.cc

```cpp
#include <stdio.h>

void DoThing();
void DoLayer() {
    printf("layer.cc DoLayer\n");
    DoThing();
}
```

并把layer和conflict打包成一个动态链接库：

```sh
$ g++ -c layer.cc -o layer.o
$ g++ -shared layer.o conflict.o -o libconflict.so
```

然后更新main.cc程序，main里面调用layer，layer里调用conflict：

```cpp
#main.cc
#include <stdio.h>

void DoThing();
void DoLayer();

int main() {
    printf("start \n");
    DoLayer();
    DoThing();
    printf("finish \n");
    return 0;
}
```

把work.o和main.o打包成一个库，之后和conflict链接成一个可执行程序，运行：

```sh
$ g++ -c main.cc -o main.o
$ ar rc libmain.a main.o work.o
$ g++ -s -L. -o main.exe -lmain -lconflict
$ LD_LIBRARY_PATH=. ./main.exe
start 
layer.cc DoLayer
work.cc DoThing
work.cc DoThing
finish
```

这里输出了两个work，正常情况下第二个work应该输出conflict，怎么解决呢？可以考虑使用-fvisibility=hidden来隐藏内部的符号，链接库内部使用的符号把它隐藏掉，不让它被导出，外部也不会改变它的调用路径。

先使用nm看一下libconflict.so里面的符号：

```sh
$ nm -CD libconflict.so
                 w _ITM_deregisterTMCloneTable
                 w _ITM_registerTMCloneTable
000000000000065a T DoLayer()
0000000000000672 T DoThing()
0000000000201030 B __bss_start
                 w __cxa_finalize
                 w __gmon_start__
0000000000201030 D _edata
0000000000201038 B _end
0000000000000688 T _fini
0000000000000528 T _init
                 U puts
```

如果把符号隐藏掉

```sh
$ g++ -fvisibility=hidden -c layer.cc -o layer.o
$ g++ -fvisibility=hidden -c conflict.cc -o conflict.o
$ g++ -shared layer.o conflict.o -o libconflict.so
再使用nm看一下libconflict.so里面的符号：
$ nm -CD libconflict.so
                 w _ITM_deregisterTMCloneTable
                 w _ITM_registerTMCloneTable
0000000000201028 B __bss_start
                 w __cxa_finalize
                 w __gmon_start__
0000000000201028 D _edata
0000000000201030 B _end
0000000000000618 T _fini
00000000000004c0 T _init
                 U puts
```

这样的话main函数肯定不能调用DoLayer啦，因为DoLayer符号没有暴露出来：

```sh
$ g++ -s -L. -o main.exe -lmain -lconflict
./libmain.a(main.o): In function `main':
main.cc:(.text+0x16): undefined reference to `DoLayer()'
collect2: error: ld returned 1 exit statu
```

那怎么暴露出来特定符号呢，直接看代码，改动了layer.cc：

```sh
#include <stdio.h>
void DoThing();
__attribute__ ((visibility ("default"))) void DoLayer() {
    printf("layer \n");
    DoThing();
}
```

再编译链接运行看看结果：

```sh
$ g++ -fvisibility=hidden -c layer.cxx -o layer.o
$ g++ -shared layer.o conflict.o -o libconflict.so
$ g++ -s -L. -o main.exe -lmain -lconflict
$ LD_LIBRARY_PATH=. ./main.exe
start 
layer.cc DoLayer
conflict.cc DoThing
work.cc DoThing
finish
```

发现已经是我们期待的结果啦，符号冲突的问题因此被解决。

是不是感觉很麻烦，难道每个要暴露的符号都要加上__attribute__这种修饰吗，这里其实可以写一个export文件，告诉编译器要导出的所有符号有哪些。

```sh
export.txt
{
	global: *DoLayer*;
	local: *;
};
g++ -Wl,--version-script=export.txt -s -shared layer.o conflict.o -o libconflict.so
```

但是这种方式只有在 gcc 中才可以被使用，我在 clang 中尝试使用但是失败啦，所以为了兼容性不建议使用这种方式，还是消停的使用__attribute__ 来解决符号冲突问题吧。

Tips:

**通过隐藏符号可以减小可执行程序的大小，还可以解决符号冲突问题，但是有个缺点，因为隐藏了符号，线上程序运行时如果 出现 crash，通过堆栈信息我们看不到具体函数调用路径，给定位问题带来了困难，所以需不需要使用这种办法，还需要根据实 际情况具体抉择**
