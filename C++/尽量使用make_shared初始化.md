# 尽量使用make_shared初始化

make_shared的使用:

```c++
shared_ptr<string> p1 = make_shared<string>(10, '9');  
 
shared_ptr<string> p2 = make_shared<string>("hello");  
 
shared_ptr<string> p3 = make_shared<string>(); 
```

之前的使用

```
shared_ptr<Context> p1(new Context());
```

C++11 中引入了智能指针, 同时还有一个模板函数 std::make_shared 可以返回一个指定类型的 std::shared_ptr, 那与 std::shared_ptr 的构造函数相比它能给我们带来什么好处呢 ?

​	内存分配的动作, 可以一次性完成. 这减少了内存分配的次数, 而内存分配是代价很高的操作.

 	make_shared函数的主要功能是在动态内存中分配一个对象并初始化它，返回指向此对象的shared_ptr;由于是通过shared_ptr管理内存，因此一种安全分配和使用动态内存的方法。



std::make_shared（比起直接使用new）的一个特性是能提升效率。使用std::make_shared允许编译器产生更小，更快的代码，产生的代码使用更简洁的数据结构。考虑下面直接使用new的代码：



### 缺点

### 构造函数是保护或私有时,无法使用 make_shared

`make_shared` 虽好, 但也存在一些问题, 比如, 当我想要创建的对象没有公有的构造函数时, `make_shared` 就无法使用了, 当然我们可以使用一些小技巧来解决这个问题, 比如这里 [How do I call ::std::make_shared on a class with only protected or private constructors?](http://stackoverflow.com/questions/8147027/how-do-i-call-stdmake-shared-on-a-class-with-only-protected-or-private-const?rq=1)

### 对象的内存可能无法及时回收

`make_shared` 只分配一次内存, 这看起来很好. 减少了内存分配的开销. 问题来了, `weak_ptr` 会保持控制块(强引用, 以及弱引用的信息)的生命周期, 而因此连带着保持了对象分配的内存, 只有最后一个 `weak_ptr` 离开作用域时, 内存才会被释放. 原本强引用减为 0 时就可以释放的内存, 现在变为了强引用, 弱引用都减为 0 时才能释放, 意外的延迟了内存释放的时间. 这对于内存要求高的场景来说, 是一个需要注意的问题. 关于这个问题可以看这里 [make_shared, almost a silver bullet](http://lanzkron.wordpress.com/2012/04/22/make_shared-almost-a-silver-bullet/)



## 总结

1.内存只需要分配一次

2.异常安全





