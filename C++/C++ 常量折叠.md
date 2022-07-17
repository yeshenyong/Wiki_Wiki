# C++ 常量折叠

转自：https://mp.weixin.qq.com/s/zk3U9WaXUoNRO78eke4p0g

```cpp
#include <iostream>

int main() {
  const int a = 0;
  int *b = const_cast<int *>(&a);
  *b = 1;

  std::cout << "a = " << a << " b = " << *b << std::endl;
  return 0;
}
```

代码很简单，主要做了以下几件事：

声明一个const 常量a，赋值为0 声明一个int型指针b，指向a的地址 修改b所指向的内存的值 输出a 和 b 第一印象，a和b都是1，因为修改了a的内存，但是

答案却是 0 1 这是为什么呢？我们尝试下输出a和b的地址

```cpp
std::cout << &a << " " << b << std::endl;
```

0x7ffe1b01ae54 0x7ffe1b01ae54 很奇怪，地址一样，值却不一样。

这就是我们这篇文章的主题**常量折叠**。 `常量折叠`是在编译器进行语法分析的时候，将常量表达式计算求值，并用求得的值来替换表达式，**放入常量表**。可以算作一种编译优化。

我只是改了这个地址内容,但是a还是0,

原因：

> 编译器在优化的过程中，会把碰见的const全部以内容替换掉，这个出现在预编译阶段；但是在运行阶段，它的内存里存的东西确实改变了