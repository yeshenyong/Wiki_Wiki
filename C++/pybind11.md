# pybind11

PyBind11是能够让C++和Python代码之间相互调用的轻量级头文件库。在这之前已经有了一个类似功能的库：Boost.Python。既然已经有了一个类似库，而且PyBind11的目的和语法都与Boost.Python相似，为什么还要重复造轮子？原因主要有以下亮点：

Boost.Python为了兼容大多数C++标准和编译器，它使用了很多可以说是魔法的操作去解决问题而变得非常的臃肿；
目前很多编译器对C++11已经有很好的支持，而且C++11应用也比较广泛。
因此，PyBind11应运而生，他能在抛弃Boost.Python的负担同时又具备Boost.Python的简单操作。

PyBind11的主要目的是将已有的C++代码接口暴露给Python去调用













### pybind11的返回值策略 return_value_policy

- **返回值策略的必要性：**

C++和python使用根本上就不一样的内存管理方法和对象生命周期管理方法。
  C++是对栈变量按照作用域来析构，对堆变量使用手动delete来析构。Python则是使用基于计数的垃圾回收机制。当提供C++函数接口给python时，如果返回类型不是基本类型，这种内存管理的差异会产生一些问题。
  只是看类型信息，看不出来Python是否应该接管返回值的所有权并最终回收它的资源，或是由C++端来处理。因为这个原因，pybind11提供了一些返回值策略标记传递给 module_::def()和class_::def()函数。
  默认的策略是return_value_policy::automatic（自动管理返回值）。



- 一个导致crash 的例子

 返回值策略非常容易犯错，所以有必要真正弄懂它们。考虑下面一个例子：

```c++
struct Person{
	int age;
	int gender;
};
Person* one_person = new Person();
/* Function declaration */
Person* get_one_person() { return one_person ; /* (pointer to a static data structure) */ }

/* Binding code */
m.def("get_one_person", &get_one_person); // <-- 严重危险行为，可能导致程序crashed
```

这里会发生什么？如果Python端调用了get_one_person()，那么返回类型被包装成一个python可用的类型。在这个场景，默认的返回值策略是return_value_policy::automatic（返回值自动管理），将会导致pybind11接管静态对象one_person的所有权。

  当python的垃圾回收器最终删除python包装时，pybind11也会尝试析构这个C++对象（使用delete操作符）。这时整个应用程序最终会crash，尽管原因非常隐蔽，其实是静态对象内存损坏。

  在上面这个例子中，应该使用return_value_policy::reference（引用返回值）策略。这种全局的静态对象one_person只是被python引用，而没有任何隐含的所有权转移。

```c++
m.def("get_one_person", &get_one_person, py::return_value_policy::reference);
```



- 所有的返回值策略探讨

很多时候如果没有正确使用返回值策略，会导致内存泄露。作为使用pybind11的开发者，必须熟悉所有的返回值策略，清楚知道什么情况该用什么。下面是所有的返回值策略。

https://blog.csdn.net/BlowfishKing/article/details/115765518































