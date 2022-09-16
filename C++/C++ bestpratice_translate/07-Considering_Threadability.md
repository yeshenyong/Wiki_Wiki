# Considering Threadability（考虑线程能力）

## Avoid Global Data（避免全局数据）

全局数据会导致函数之间产生意想不到的副作用，并使代码难以或不可能并行化。即使当前的代码不打算进行并行化，也没有理由让它在未来不可能实现

### Statics

除了是全局数据之外，静态数据并不总是像您所期望的那样被构造和解构。在跨平台环境中尤其如此。例如，这个关于从动态模块加载的共享静态数据的销毁顺序的[g++错误](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=66830)

### Shared Pointers

`std::shared_ptr` 和 “全局变量一样”（http://stackoverflow.com/a/18803611/29975）因为它允许多段代码与相同的数据交互

### Singletons

单例通常使用静态和/或`shared_ptr`实现

## Avoid Heap Operations（避免堆操作）

在线程环境中要慢得多。在许多情况下，甚至在大多数情况下，复制数据更快。加上移动操作之类的东西

## Mutex and mutable go together (M&M rule, C++11)（互斥和可变是一起使用的）

对于成员变量，最好将互斥和可变结合使用。这适用于两个方面:

- 可变成员变量被假定为共享变量，因此它应该与互斥锁同步(或使其为原子变量)
- 如果成员变量本身是互斥对象，那么它应该是可变的。这是在const成员函数中使用它所必需的

欲了解更多信息，请参阅Herb Sutter的以下文章:http://herbsutter.com/2013/05/24/gotw-6a-const-correctness-part-1-3/

参见有关 `const &` return values值的[安全讨论](https://github.com/cpp-best-practices/cppbestpractices/blob/master/04-Considering_Safety.md#consider-return-by-value-for-mutable-data-const--for-immutable)