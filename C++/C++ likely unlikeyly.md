# C++关键字之likely和unlikely

https://developer.aliyun.com/article/951294

https://www.cnblogs.com/xudong-bupt/p/7337240.html

现代CPU为了提高执行指令执行的吞吐量，使用了流水线技术，它将每条指令分解为多步，让不同指令的各步操作重叠，从而实现若干条指令并行处理。在流水线中，一条指令的生命周期可能包括：

- 取指：将指令从存储器中读取出来，放入指令缓冲区中。

- 译码：对取出来的指令进行翻译

- 执行：知晓了指令内容，便可使用CPU中对应的计算单元执行该指令

- 访存：将数据从存储器读出，或写入存储器

- 写回：将指令的执行结果写回到通用寄存器组

流水线技术无法提升CPU执行单条指令的性能，但是可以通过相邻指令的并行化提高整体执行指令的吞吐量



## 分支预测

我们都知道，程序的控制流程基本可分为三种：顺序、分支和循环。对CPU流水线来说，顺序比较好处理，一条路往前趟就行了。但是当程序中有了分支结构之后，CPU无法确切知道到底应该取分支1中的D指令，还是分支二中的E指令。

此时CPU会根据指令执行的上下文，猜测那一路分支应该被执行。预测的结果有两个，命中或者命不中。

在前一种情况下，CPU流水线正常执行，不会被打断。在后一种情况下，需要CPU丢掉为跳转指令之后的所有指令所做的工作，再开始从正确位置处起始的指令去填充流水线，这会导致很严重的惩罚：大约20-40个时钟周期的浪费，导致程序性能的严重下降

![1446762f051f1cdf0e6686932350e117.png](https://ucc.alicdn.com/pic/developer-ecology/5da29759285648cb844a8e97e15b3a03.png)

既然程序是我们程序员所写，在一些明确的场景下，我们应该比CPU和编译器更了解哪个分支条件更有可能被满足。我们是否可将这一先验知识告知编译器和CPU, 提高分支预测的准确率，从而减少CPU流水线分支预测错误带来的性能损失呢？答案是可以！它便是`likely`和`unlikely`。在Linux内核代码中，这两个宏的应用比比皆是。下面是他们的

```sh
#define likely(x) __builtin_expect(!!(x), 1) 
#define unlikely(x) __builtin_expect(!!(x), 0)
```

`likely`，用于修饰if/else if分支，表示该分支的条件更有可能被满足。而`unlikely`与之相反

以下为示例。`unlikely`修饰`argc > 0`分支，表示该分支不太可能被满足。

```c++
#include <cstdio>
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)
int main(int argc, char *argv[])
{
    if (unlikely(argc > 0)) {
        puts ("Positive\n");
    } else
    {
        puts ("Zero or Negative\n");
    }
    return 0;
}
```

## likely/unlikely的原理

接下来，我们从汇编指令分析likely/unlikely到底是如何起作用的?

首先我们将上述代码中的`unlikely`去掉，然后反汇编，作为对照组

```cpp
#include <cstdio>
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)
int main(int argc, char *argv[])
{
    if (argc > 0) {
        puts ("Positive\n");
    } else
    {
        puts ("Zero or Negative\n");
    }
    return 0;
}
```

汇编如下，我们看到，if分支中的指令被编译器放置于分支跳转指令jle相邻的位置，即CPU流水线在遇到`jle`指令所代表的的'岔路口'时，更倾向于走if分支

```sh
.LC0:
        .string "Positive\n"
.LC1:
        .string "Zero or Negative\n"
main:
        sub     rsp, 8
        test    edi, edi                
        jle     .L2                     ; 如果argc <= 0, 跳转到L2
        mov     edi, OFFSET FLAT:.LC0   ; 如果argc > 0, 从这里执行
        call    puts
.L3:
        xor     eax, eax
        add     rsp, 8
        ret
.L2:
        mov     edi, OFFSET FLAT:.LC1
        call    puts
        jmp     .L3
```

接着我们在if分支中加上unlikely, 反汇编如下。这里的情况正好与对照组相反，if分支下的指令被编译器放置于远离跳转指令`jg`的位置。这意味着CPU此时更倾向于走else分支

```sh
.LC0:
        .string "Positive\n"
.LC1:
        .string "Zero or Negative\n"
main:
        sub     rsp, 8
        test    edi, edi
        jg      .L6
        mov     edi, OFFSET FLAT:.LC1
        call    puts
.L3:
        xor     eax, eax
        add     rsp, 8
        ret
.L6:
        mov     edi, OFFSET FLAT:.LC0
        call    puts
        jmp     .L3
```

因此，通过对分支条件使用`likely`和`unlikely`，我们可给编译器一种暗示，即该分支条件被满足的概率比较大或比较小。而编译器利用这一信息优化其机器指令，从而最大限度减少CPU分支预测失败带来的惩罚

## likely/unlikely的适用条件

CPU有自带的分支预测器，在大多数场景下效果不错。因此在分支发生概率严重倾斜、追求极致性能的场景下，使用`likely/unlikely`才具有较大意义