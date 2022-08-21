# c++filt

c++filt 命令可用于解析 C++ 和 Java 中被修饰的符号，比如变量与函数名称。 

```sh
c++filt _ZL5dTest dTest

c++filt _Z5printRKSs print(std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)
```



**解析编译器 g++ 修饰的函数名称，但不显示函数参数类型**

```sh
c++filt -p _Z5printRKSs
```

