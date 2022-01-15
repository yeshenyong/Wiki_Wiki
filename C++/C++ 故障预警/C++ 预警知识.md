# C++ 预警知识

### sort

#### 故事一

《effective STL》

> ***第21条：总是让比较函数在等值情况下返回false***



strict weak ordering导致程序coredump，给公司造成数百万损失，最终评级故障为P0级



##### 背景

流量经过召回、过滤等一系列操作后，得到最终的广告候选集，需要根据相应的策略，进行排序，最终返回首位最优广告。

```
struct AdItem {
  std::string ad_id;
  int priority;
  int score;
};
```

现在有一个AdItem类型的verctor，要求对其排序，排序规则如下:

- 按照priority升序排列
- 如果priority一样大，则按照score降序排列

需求还是比较简单吧，当时线上代码如下：

```c++
void AdSort(std::vector<AdItem> &ad_items) {
 std::sort(ad_items.begin(), ad_items.end(), [](const AdItem &item1, const AdItem &item2) {
   if (item1.priority < item2.priority) {
      return true;
    } else if (item1.priority > item2.priority) {
      return false;
    }

    return item1.score >= item2.score;
 } );
}
```



测试环境构造测试case，符合预期，上线。

恐怖的事情来了，上线不久后，程序直接coredump，然后自动重启，接着有coredump，当时心情是这样的。

##### 定位

第一件事，登录线上服务器，通过gdb查看堆栈信息

由于线上是release版的，看不了堆栈信息，将其编译成debug版，**在某台线上进行灰度，不出意料，仍然崩溃，查看堆栈信息**。（我觉得是在一个程序上线之前必须要做充足的测试，而且是在2c的场景中，灰度分层实验场景测试比较好做的情况下，更是要着手去做的一件事情，不能充足相信人写的代码）

通过堆栈信息，这块的崩溃恰好是在AdSort函数执行完，析构std::vector的时候发生，看来就是因为此次上线导致，于是代码回滚，重新分析原因。





第一件事，登录线上服务器，通过gdb查看堆栈信息

由于线上是release版的，看不了堆栈信息，将其编译成debug版，在某台线上进行灰度，不出意料，仍然崩溃，查看堆栈信息。

通过堆栈信息，这块的崩溃恰好是在AdSort函数执行完，析构std::vector的时候发生，看来就是因为此次上线导致，于是代码回滚，重新分析原因。

运行正常，那么就是因为lambda比较函数有问题，那么为什么这样就没问题了呢？

想起之前在<Effective STL>中看到一句话***第21条：总是让比较函数在等值情况下返回false***。应该就是没有遵循这个原则，才导致的coredump。

总结下就是，存在两个变量x和y：

- x > y 等同于  y < x
- x == y 等同于 !(x < y) && !(x > y)

要想***严格弱序\***，就需要遵循如下规则：

- 对于所有的x：x < x永远不能为true，每个变量值必须等于其本身
- 如果x < y，那么y < x就不能为true
- 如果x < y 并且y < z，那么x < z,也就是说有序性必须可传递性
- 如果x == y并且y == z，那么x == z，也就是说值相同也必须具有可传递性

那么，为什么不遵循严格弱序的规则，就会导致coredump呢？

> ❝
>
> 对于`std::sort()`，当容器里面元素的个数大于`_S_threshold`的枚举常量值时，会使用快速排序，在STL中这个值的默认值是`16`
>
> ❞

我们先看下sort的函数调用链(去掉了不会导致coredump的部分)：

```
sort
-> __introsort_loop
--> __unguarded_partition
```

我们看下__unguarded_partition函数的定义：

```c++
template<typename _RandomAccessIterator, typename _Tp, typename _Compare>
     _RandomAccessIterator
     __unguarded_partition(_RandomAccessIterator __first,
               _RandomAccessIterator __last,
               _Tp __pivot, _Compare __comp)
     {
       while (true)
     {
       while (__comp(*__first, __pivot))
         ++__first;
       --__last;
       while (__comp(__pivot, *__last))
         --__last;
       if (!(__first < __last))
         return __first;
       std::iter_swap(__first, __last);
       ++__first;
     }
     }
```

在上面代码中，有下面一段：

```c++
while (__comp(*__first, __pivot))
         ++__first;
```

其中，__first为迭代器，__pivot为中间值，__comp为传入的比较函数。

如果传入的vector中，后面的元素完全相等，那么__comp比较函数一直是true，那么后面++__first，最终就会使得迭代器失效，从而导致coredump。

好了，截止到此，此次线上故障原因分析完毕。









摘自

1. https://mp.weixin.qq.com/s/xc1vfhTYq9EG-TQdyfkBQA