# Linux 共享库如何进行版本控制

大家平时使用Linux系统过程中可能都见过文件系统里有好多带版本号的共享库，如下：

```sh
lrwxrwxrwx 1 root root     21 Mar 25 18:33 libDeployPkg.so.0 -> libDeployPkg.so.0.0.0
-rw-r--r-- 1 root root  31304 Mar 25 18:33 libDeployPkg.so.0.0.0
lrwxrwxrwx 1 root root     20 Mar 25 18:33 libguestlib.so.0 -> libguestlib.so.0.0.0
-rw-r--r-- 1 root root  26752 Mar 25 18:33 libguestlib.so.0.0.0
lrwxrwxrwx 1 root root     16 Mar 25 18:33 libhgfs.so.0 -> libhgfs.so.0.0.0
-rw-r--r-- 1 root root 167248 Mar 25 18:33 libhgfs.so.0.0.0
lrwxrwxrwx 1 root root     18 Mar 25 18:33 libvgauth.so.0 -> libvgauth.so.0.0.0
-rw-r--r-- 1 root root  85144 Mar 25 18:33 libvgauth.so.0.0.0
lrwxrwxrwx 1 root root     19 Mar 25 18:33 libvmtools.so.0 -> libvmtools.so.0.0.0
-rw-r--r-- 1 root root 631480 Mar 25 18:33 libvmtools.so.0.0.0
```

大家平时关注过这些共享库的版本号是以什么规则制定的呢？

以"libname.so.x.y.z"来说：

- 最前面使用"lib"前缀，中间是库的名字和后缀".so"，后面跟着三个数字组成版本号
- x为主版本号，表示库的重大升级，不同主版本号的库之间是不兼容的，依赖于旧版本号的程序要修改相应的代码来适应新版本的库，并重新编译才可以链接新版本库运行。
- y为次版本号，表示库的增量升级，相对于旧版本只是增加了一些新的接口，并保持原有的接口符号不变，完全兼容旧版本的库，即一个依赖于旧版本号的程序可以直接链接新版本库来正常运行。
- z为发布版本号，表示库的一些错误修正、性能优化等，不会增加新的接口，只是在旧版本库的基础上做一些优化。



### **如何创建共享库**

首先介绍一些SO-NAME，每一个共享库都有一个SO-NAME，即共享库的文件名去掉次版本号和发布版本号，比如"libname.so.x.y.z"的SO_NAME就是"libname.so.x"。在Linux系统中，系统会为每个共享库在所在的目录中创建一个与SO-NAME同名并且指向实际共享库的软链接。例如"libc.so.1"会指向"libc.so.1.2.3"，当目录中有"libc.so.1.2.4"时，"libc.so.1"这个软链就会指向"libc.so.1.2.4"，达到升级的目的。那系统是如何更新这个软链的呢，例如我们经常使用apt-get install更新程序时，动态链接库更换了一个新的版本，那同时也需要更新一些软链指向的位置，有一个程序叫**ldconfig**，每次升级后执行一下ldconfig，就会自动遍历所有的默认共享库目录，更新软链



如下代码

```cpp
// libc.c
#include <stdio.h>

void func(int i) {
    printf("func %d \n", i);
}
```



在gcc中通过“-Wl,-soname”参数告诉链接器，用于制定共享库的SO-NAME。

```sh
gcc -fPIC -shared -Wl,-soname,libc.so.1 -o libc.so.1.2.3 lib.c
```



再看program.c

```cpp
// program.c
#include <stdio.h>

int main() {
    func(1);
    return 0;
}
```



编译链接运行：

```sh
$ gcc -o ttt program.c ./libc.so.1.2.3
$ ./ttt
./ttt: error while loading shared libraries: libc.so.1: cannot open shared object file: No such file or directory    
```

上面可见，程序并没有运行成功，因为没有创建SO-NAME的相应软链

```sh
$ ln -s libc.so.1.2.3 libc.so.1
再次运行
$ ./ttt
./ttt: error while loading shared libraries: libc.so.1: cannot open shared object file: No such file or directory    
```

再次运行发现还是没有运行成功，为什么呢？因为程序运行时不知道去哪里找这个动态链接库，所有需要指定一下查找库的路径：

```sh
$ LD_LIBRARY_PATH=. ./ttt
func 1
```



为什么运行"LD_LIBRARY_PATH=."后程序就可以运行成功了呢？这里介绍下共享库的路径查找相关知识点。

### **共享库系统路径**

一般有三个：

- /lib：主要存放系统最关键和基础的共享库，比如动态链接器、C语言运行库、数学库等，/bin和/sbin下的程序需要的共享库和系统启动需要的库一般放在这里。
- /usr/lib：主要存放一些非系统运行时所需要的关键性的共享库，一般是用户开发过程中用到的共享库。
- /usr/local/lib：一般存放一些非系统所需要的第三方库，例如装一个Python环境依赖的库都可以放在这里。

总结：/lib和/usr/lib存放一些常用成熟的系统本身需要的库，/usr/local/lib存放一些非系统所需要的第三方库。

简单讲：/lib是内核级的，/usr/lib是系统级的：/usr/local/lib是用户级的

两个在程序运行时与共享库搜索路径相关的环境变量：

- LD_LIBRARY_PATH：通过此环境变量，可以临时改变某个程序的共享库查找路径，而不会影响系统中的其它程序，LD_LIBRARY_PATH默认为空，如果某个进程设置了此环境变量，动态链接器在查找共享库时，会首先查找LD_LIBRARY_PATH的指定目录，通过这个环境变量可以测试新的共享库，因为链接器链接时会链接最先找到的共享库。

- LD_PRELOAD：与LD_LIBRARY_PATH类似，它比LD_LIBRARY_PATH里面的目录优先级还高，LD_PRELOAD里面指定的共享库和目标文件中的全局符号会覆盖后面出现的同名全局符号，使得我们可以很方便的改写标准库里的某个函数而不影响其它函数，对于程序调试和测试非常有用。



### **为什么要extern C？**

前面已经介绍了共享库的版本升级机制，在C语言中可能升级比较方便简单，不会遇到太多问题，在C++中就比较繁琐了，因为C++为了支持重载和namespace等，编译出来的函数符号相对于函数名字来说有很多前后缀修饰，而且不同厂家的编译器或者不同版本的编译器可能符号修饰规则都不同，在更新时可能也会因为这种原因导致不兼容，所以函数导出时需要使用extern C修饰，让导出函数的符号遵守C语言的规范