# vector初始化与否导致的巨大性能差异

摘自：https://mp.weixin.qq.com/s/9JKG1ckbbG9Lc25uvCZapw

最近在优化引擎代码，在优化的过程中发现一个很奇怪的问题，一个简单的对象，存放在std::vector<> v中，如果v定义的时候为每个元素指定初值，那么后面对v中每个元素的写就飞快；相反的，如果v定义的时候，不指定初始值，那么后面对v中元素写操作的时候，就花费大约前一种2-3倍的时间。

今天，借助此文，分享下原因的排查过程。





## 问题

为了能够快速说明问题，直接上代码吧，如下：

```c++
#include <chrono>
#include <functional>
#include <iostream>
#include <vector>

int const num = 1000000000;
struct AdItem
{
     AdItem() {}

     AdItem(int x, int y, int z)
     {
         x_ = x; y_ = y; z_ = z;
     }

     int x_;
     int y_;
     int z_;
};


void time_report(const std::function<void()> &f1, const std::function<void()> &f2) {
   auto start = std::chrono::high_resolution_clock::now();
   f1();
   auto end = std::chrono::high_resolution_clock::now();
   std::cout << "allocation done in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

   start = std::chrono::high_resolution_clock::now();
   f2();
   end = std::chrono::high_resolution_clock::now();
   std::cout << "assignment done in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
}

void uninitialized_performance() {
  std::vector<AdItem> v;
  time_report([&v]() { v.reserve(num); },
          [&v]() {
            for (int i = 0; i < num; ++i) {
              v[i] = {i + 2, i + 4, i + 6};
            } });
}

void initialized_performance() {
  std::vector<AdItem> v;
  time_report([&v]() { v.assign(num, AdItem{0, 0, 0}); },
          [&v]() {
            for (int i = 0; i < num; ++i) {
              v[i] = {i + 2, i + 4, i + 6};
            } });

}

int main()
{
     uninitialized_performance();
     initialized_performance();
     return 0;
}
```



程序输出如下：

```sh
allocation done in 0ms
assignment done in 59ms
allocation done in 46ms
assignment done in 27ms
```

论使用`-g -O2 -O3`都是一样的效果，执行多次结果仍然一样。



## CPU Cache

> CPU缓存（CPU Cache）位于CPU与内存之间的临时存储器，它的容量比内存小但交换速度快。在缓存中的数据是内存中的一小部分，但这一小部分是短时间内CPU即将访问的，当CPU调用大量数据时，就可避开内存直接从缓存中调用，从而加快读取速度。
>
> 在CPU中加入缓存是一种高效的解决方案，这样整个内存储器（缓存+内存）就变成了既有缓存的高速度，又有内存的大容量的存储系统了。缓存对CPU的性能影响很大，主要是因为CPU的数据交换顺序和CPU与缓存间的带宽引起的。

看到结果以后，第一反应是CPU Cache导致，因为占用的内存大小一样，所以怀疑在第二次访问的时候，因为CPU Cache中已经有缓存，所以直接使用。

为了确认是否是因为CPU Cache而导致的性能差异，所以将测试分成两个独立的程序进行，即分别执行`uninitialized_performance()`和`initialized_performance()`函数，结果与之前的一致，所以**排除CPU Cache的原因**。



## perf工具

> perf是用来进行软件性能分析的工具,可以利用PMU、tracepoint和内核中的特殊计数器来进行性能统计，用来分析内核和应用程序的性能。

使用`perf stat uninitialized_performance`结果如下：

```sh
Performance counter stats for 'uninitialized_performance':

         47.923956      task-clock (msec)         #    0.985 CPUs utilized
                 6      context-switches          #    0.125 K/sec
                 0      cpu-migrations            #    0.000 K/sec
             1,007      page-faults               #    0.021 M/sec
        99,354,240      cycles                    #    2.073 GHz                      (83.33%)
        72,689,011      stalled-cycles-frontend   #   73.16% frontend cycles idle     (83.46%)
        48,738,086      stalled-cycles-backend    #   49.05% backend cycles idle      (66.66%)
        98,377,529      instructions              #    0.99  insn per cycle
                                                  #    0.74  stalled cycles per insn  (83.32%)
        11,399,060      branches                  #  237.857 M/sec                    (83.39%)
            19,617      branch-misses             #    0.17% of all branches          (83.16%)
```



使用`perf stat initialized_performance`结果如下：

```sh
76.092443      task-clock (msec)         #    0.991 CPUs utilized
                 6      context-switches          #    0.079 K/sec
                 0      cpu-migrations            #    0.000 K/sec
             1,007      page-faults               #    0.013 M/sec
       157,742,162      cycles                    #    2.073 GHz                      (82.94%)
       110,940,710      stalled-cycles-frontend   #   70.33% frontend cycles idle     (82.96%)
        72,531,622      stalled-cycles-backend    #   45.98% backend cycles idle      (65.92%)
       178,142,743      instructions              #    1.13  insn per cycle
                                                  #    0.62  stalled cycles per insn  (82.99%)
        31,488,875      branches                  #  413.824 M/sec                    (84.01%)
            24,309      branch-misses             #    0.08% of all branches          (84.17%)
```



因为perf只能得到整个程序执行期间的变化，而我们实际上需要的是核心部分，看来通过perf得到有用的信息，只能另寻他法。



## 源码跟踪

截止到现在，上述方案都没能解答本次问题的疑问，只能祭出屠龙刀，看源码了。

`未初始化版`调用信息如下：

```sh
> std::vector<AdItem, std::allocator<AdItem> >::vector
--> std::vector<AdItem, std::allocator<AdItem> >::_M_default_initialize
---> std::__uninitialized_default_n_a<AdItem*, unsigned long, AdItem>
----> std::__uninitialized_default_n<AdItem*, unsigned long>
-----> std::__uninitialized_default_n_1<false>::__uninit_default_n<AdItem*, unsigned long>
------> std::_Construct(std::__addressof(*__cur))
```



`初始化版`调用信息如下：

```sh
-> std::vector<AdItem, std::allocator<AdItem> >::vector
--> std::vector<AdItem, std::allocator<AdItem> >::_M_fill_initialize
---> std::__uninitialized_fill_n_a<AdItem*, unsigned long, AdItem, AdItem>
----> std::uninitialized_fill_n<AdItem*, unsigned long, AdItem>
-----> std::__uninitialized_fill_n<false>::__uninit_fill_n<AdItem*, unsigned long, AdItem>
------> std::_Construct(std::__addressof(*__cur), __x)
```

从上述函数调用信息可以看出，初始化版本和都调用了std::_Construct函数，而该函数通过其名称就能看到，是调用了构造函数对内存块进行构造(底层是用`placement new`实现)，得不到我们有用的信息。





## 汇编

下面我们从汇编角度来看下(为了篇幅，去掉了无关紧要的部分以及相同的部分)。

未初始化版本：

```sh
uninitialized_performance():
  mov QWORD PTR [rsp+104], OFFSET FLAT:std::_Function_handler<void (), uninitialized_performance()::{lambda()#2}>::_M_invoke(std::_Any_data const&)
  mov QWORD PTR [rsp+80], rax
  mov QWORD PTR [rsp+96], OFFSET FLAT:std::_Function_base::_Base_manager<uninitialized_performance()::{lambda()#2}>::_M_manager(std::_Any_data&, std::_Any_data const&, std::_Manager_operation)
  mov QWORD PTR [rsp+48], rax
  mov QWORD PTR [rsp+72], OFFSET FLAT:std::_Function_handler<void (), uninitialized_performance()::{lambda()#1}>::_M_invoke(std::_Any_data const&)
  mov QWORD PTR [rsp+64], OFFSET FLAT:std::_Function_base::_Base_manager<uninitialized_performance()::{lambda()#1}>::_M_manager(std::_Any_data&, std::_Any_data const&, std::_Manager_operation)
```



初始化版本：

```sh
initialized_performance():
  mov QWORD PTR [rsp+64], rsp
  mov QWORD PTR [rsp+88], OFFSET FLAT:std::_Function_handler<void (), initialized_performance()::{lambda()#2}>::_M_invoke(std::_Any_data const&)
  mov QWORD PTR [rsp+80], OFFSET FLAT:std::_Function_base::_Base_manager<initialized_performance()::{lambda()#2}>::_M_manager(std::_Any_data&, std::_Any_data const&, std::_Manager_operation)
  mov QWORD PTR [rsp+32], rsp
  mov QWORD PTR [rsp+56], OFFSET FLAT:std::_Function_handler<void (), initialized_performance()::{lambda()#1}>::_M_invoke(std::_Any_data const&)
  mov QWORD PTR [rsp+48], OFFSET FLAT:std::_Function_base::_Base_manager<initialized_performance()::{lambda()#1}>::_M_manager(std::_Any_data&, std::_Any_data const&, std::_Manager_operation)
```

从上述汇编结果没看到有什么区别，所以排除了语言层面的区别了，尝试从其它方面进行分析。



## lazy allocation

既然源码和汇编都看不出来区别，那么只能从另外一个方向来进行考虑，那就是`内存分配`，有没有可能虽然是都调用了new进行内存分配，但是实际上并没有分配呢？

记得之前看过一些文章，说是操作系统有个`lazy allocation（延迟分配）`机制，即用户调用API进行内存分配的时候，操作系统并不会直接分配给用户这么多内存，而是直到用户真的访问了申请的page时产生一个page falut，然后将这个page真的分配给用户，并重新执行产生page fault的语句。

我们使用以下例子进行验证：

```c++
#include <unistd.h>

struct X {
  int x_[1 << 24];
};

int main() {
    auto x = new X;
    sleep(100);
    return 0;
}
```



然后通过`htop -p xxx`命令查看信息，如下：

![图片](https://mmbiz.qpic.cn/mmbiz_png/p3sYCQXkuHgicKYkawpGRm5ImNWe1fxhm52j5nVhcJUHnBmhhPuTocLZno0QgiaUsYVYl8dpgnVW4JnN9v7e2yug/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

然后，我们尝试将代码进行修改如下：

```c++
#include <unistd.h>
#include <string.h>

struct X {
  int x_[1 << 24];
};

int main() {
    auto x = new X;
    memset(x->x_, 0, sizeof(x->x_)); // 增加了该句，仅仅为了访问
    sleep(100);
    return 0;
}
```

如下所示：

![图片](https://mmbiz.qpic.cn/mmbiz_png/p3sYCQXkuHgicKYkawpGRm5ImNWe1fxhmt0yOibHHjWmCDM5ejQwyCI39lbKeeq3C0XYUrILQzkUFbJqyAv2ADXA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)



通过上述htop的输出信息的`RES`块可以看出，第二次调用分配了足够的虚拟内存，这就验证了我们这次的结论，即使使用了new，在真正使用之前是`没有被真正的分配虚拟内存`。

> **Lazy allocation simply means not allocating a resource until it is actually needed.** This is common with singleton objects, but strictly speaking, any time a resource is allocated as late as possible, you have an example of lazy allocation.
>
> By delaying allocation of a resource until you actually need it, you can **decrease startup time, and even eliminate the allocation entirely if you never actually use the object**. In contrast, you could pre-allocate a resource you expect to need later, which can make later execution more efficient at the expense of startup time, and also avoids the possibility of the allocation failing later in program execution.



现在，我们从`lazy allocation`角度来分析该问题。对于未初始化的vector，该vector申请了一大块内存，operator new只是返回了虚拟地址(此时并未将物理地址与其关联)，当具体访问数据(for循环)的时候，操作系统才会将虚拟地址与物理地址进行关联。而在构造时给初值，算是访问大块连续逻辑空间，操作系统会将虚拟地址与物理地址相关联，这样在后面访问的时候，因为已经访问了一次了，所以会很快。

在源码一节中，我们提到未初始化版本和初始化版本最终分别调用了`std::_Construct(std::__addressof(*__cur))`和`_Construct(std::__addressof(*__cur), __x)`，下面是其具体实现。

未初始化版本：

```c++
template<typename _T1, typename... _Args>
   inline void
   _Construct(_T1* __p, _Args&&... __args) { 
    ::new(static_cast<void*>(__p)) _T1(std::forward<_Args>(__args)...); 
  }
```



初始化版本如下：

```c++
 inline void
   _Construct(_T1* __p, const _T2& __value) {
     ::new(static_cast<void*>(__p)) _T1(__value);
   }
```

std::_Construct是在std::vector<>定义中进行调用的，也就是说都访问了对应的地址，但是为什么区别这么大呢？后面，对示例代码进行了修改：

1、在默认的构造函数中对变量进行了赋值初始化操作

```c++
AdItem() {
         x_ = 0; y_ = 0; z_ = 0;
  }
```

2、对构造函数使用default关键字

```c++
 AdItem() = default;
```



修改之后，再次编译，运行，未初始化版本和初始化版本的性能结果基本一致，进一步验证了我们的猜想`lazy allocation`引起的性能差异。