# GDB 技巧



gdb 基本上是每个 unix 环境开发者都会使用的调试工具，使用 gdb 基本上可以按照自己的需求随心所欲的运行程序， 可让被调试的程序在自己所指定的断点处停住，同时可以检查程序当前的状态，检查各种变量及寄存器的值，也可以 动态的改变程序的执行环境。



### gdb 调试的三种方法

1. `gdb filename`：run 开始、quit 结束
2. `gdb attach pid`：程序已经运行后，可使用 attach 跟踪进程，attach 目标进程后，调试器会暂停下来，需要 continue 才继续， 停止调试后使用 detach 命令分离调试器，quit 结束
3. `gdb filename corename` ：默认不开启 core，需要设置 core 文件大小，使用 `ulimit -a` 可查看 core 文件大小，使用 `ulimit -c [n]` 设置 core 文件最大 n 字节，使用 `ulimit -c unlimited` 表示修改为不限制大小。



**自定义 core 文件名称**

core 文件的默认路径和名称为当前工作目录下生成的"core"，通过如下方式可修改 core 的目录和文件名称：

```sh
echo "/root/test/core-%e-%p-%t" > /proc/sys/kernel/core_pattern
%p->pid 
%u->uid 
%g->gid 
%s->signal 
%t->timestamp
%h->hostname 
%e->exectable nam
```



### 常用命令

看本子笔记吧























































