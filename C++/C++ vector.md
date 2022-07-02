![img](https://pic2.zhimg.com/80/v2-b70f3a45baacc2dcf3f8d69c049e2bb9_720w.jpg)

这是因为 vector 是C++标准库中的容器类，其可以理解为一个动态数组，其内部实现有三个指针：

- pointer _M_start;
- pointer _M_finish;
- pointer _M_end_of_storage;

分别代表其使用内存的开始、结束，以及分配的内存结尾，其中

- _M_finish - _M_start 对应 sizeof()；
- _M_end_of_storage - _M_start 对应 capacity() ；

> vector 通过配置比其所容纳的元素所需更多的内存，即先预留足够空间，避免二次分配，从而提高 vector 的性能。