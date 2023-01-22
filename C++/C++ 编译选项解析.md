# C++ 编译选项解析

`-fvisibility-inlines-hidden`



`-fPIC`



`-L`、`-I`、`-l`



`-DNDEBUG`



`-DDEBUG`



`-g`



`-O2、O1、O3`



`-DCLOSE_PROFILING`



`-mavx`



`-msse4.2`



`-msse4.1`



`-fno-elide-constructors`

编译器默认开启 `RVO`（return valued optimize）优化，该编译选项可以关闭`RVO`



`-faligned-new`

启用内存对齐（会有对应`warning`），该标志位在 `std=c++17` 默认启用

> Enable support for C++17 `new` of types that require more alignment than `void* ::operator new(std::size_t)` provides. A numeric argument such as `-faligned-new=32` can be used to specify how much alignment (in bytes) is provided by that function, but few users will need to override the default of `alignof(std::max_align_t)`.

```cpp
struct alignas(8) Foo { int x; }
```



`-Wno-error=non-virtual-dtor`

> Warn when delete is used to destroy an instance of a class that has virtual functions and non-virtual destructor.  It is unsafe to delete an instance of a derived class through a pointer to a base class if the base class does not have a virtual destructor.  This warning is enabled by -Wall.

中文

> 如果delete 用于销毁具有虚函数并且不具有非虚析构函数类的实例时，发出警告。如果基类没有虚析构函数，则指向基类的指针删除派生类的实例是不安全的。此警告由 `-Wall` 启用

















