# C++ glibc

glibc 是 gnu 发布的 libc 库，即 c 运行库。glibc 是 linux 系统中最底层的 api，几乎其他任何的运行库都会依赖 glibc. glibc 除了封装 linux 操作系统所提供的系统服务外，它本身提供了许多其它一些必要服务的实现，主要有：

- string：字符串处理
- signal：信号处理
- dlfcn：管理共享库的动态加载
- direct：文件目录操作
- elf：共享库的动态加载器，即 interpreter
- iconv：不同字符集的编码转换
- inet：socket 接口的实现
- intl：gettext 的实现
- io
- linuxthreads
- locale：本地化
- login：虚拟终端设备的管理，及系统的安全访问
- malloc：动态内存管理的分配与管理
- nis
- stdlib
- math

## gcc 和 glibc 的关系

首先，gcc 是编译器，基本上 linux 下所有程序（包括内核）都是 gcc 编译的，libc 也是。但 gcc 和 libc 又是相互依赖的，什么意思呢？就是在编译 c/cpp 代码时，既需要 gcc，也需要 libc.

## gcc 发行版本

截止2021年8月，最新版本已经到 gcc11.2 了。

## glibc 发行版本

截止2021年8月，最新版本已经到 glibc2.34 了，历史版本有 glibc2.29、2.30、2.31、2.32、2.33 等。