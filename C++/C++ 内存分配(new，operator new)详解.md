# C++ 内存分配(new，operator new)详解

转自：https://blog.csdn.net/wudaijun/article/details/9273339



好文

摘要

1. new operator 和 operator new

   1. new operator：指我们在C++里通常用到的关键字，比如A* a = new A;

   2. operator new：它是一个操作符，并且可被重载(类似加减乘除的操作符重载)

      1. 调用operator new (sizeof(A))
      2. 调用A:A()
      3. 返回指针

      operator new有三种形式：

      ```cpp
      throwing (1)    
      void* operator new (std::size_t size) throw (std::bad_alloc);
      nothrow (2) 
      void* operator new (std::size_t size, const std::nothrow_t& nothrow_value) throw();
      placement (3)   
      void* operator new (std::size_t size, void* ptr) throw();
      ```

      ```cpp
      // (1)(2)的区别仅是是否抛出异常，当分配失败时，前者会抛出bad_alloc异常，后者返回null，不会抛出异常。它们都分配一个固定大小的连续内存。
      A* a = new A; //调用throwing(1)
      A* a = new(std::nothrow) A; //调用nothrow(2)
      ```

      

