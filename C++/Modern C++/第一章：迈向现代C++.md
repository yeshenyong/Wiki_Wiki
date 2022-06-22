# 第一章

### 1.1 被弃用的特性



**注意：**弃用并非彻底不能用，只是用于暗示程序员这些特性将从未来的标准中消失，应该尽 量避免使用。但是，已弃用的特性依然是标准库的一部分，并且出于兼容性的考虑，大部分 特性其实会『永久』保留



- 不再允许字符串字面值常量赋值给一个`char *` 。如果需要用字符串字面值常量赋值和初始化一个`char *` ，应该使用`const char *` 或者`auto` 

```c
char *str = "hello world!"; // 将出现弃用警告
```

- C++98 异常说明、unexpected_handler、set_unexpected() 等相关特性被弃用，应该使用 noexcept（ps：C++的异常机制不建议使用，得不偿失）
- auto_ptr 被弃用，应使用 unique_ptr。
- **register 关键字被弃用，可以使用但不再具备任何实际含义**
-  bool 类型的 ++ 操作被弃用（PS：搞这些操作的程序员怕不是...）
- 如果一个类有析构函数，为其生成拷贝构造函数和拷贝赋值运算符的特性被弃用了（C++ 手写的乐趣）
- C 语言风格的类型转换被弃用（即在变量前使用 (convert_type)），应该使用 static_cast（正常类型转换）、 reinterpret_cast、const_cast（const转非const） 来进行类型转换
- 特别地，在最新的 C++17 标准中弃用了一些可以使用的 C 标准库，例如<ccomplex>、<cstdalign> 、 <cstdbool>与  <ctgmath>等



​	还有一些其他诸如参数绑定（C++11 提供了 std::bind 和 std::function）、export 等特性也均 被弃用。**前面提到的这些特性如果你从未使用或者听说过，也请不要尝试去了解他们，应该向新标准靠 拢**，直接学习**新特性**。毕竟，技术是向前发展的



### 1.2 与 C 的兼容性

> 出于一些不可抗力、历史原因，我们不得不在 C++ 中使用一些 C 语言代码（甚至古老的 C 语言代 码），例如 Linux 系统调用。



从现在开始，你的脑子里应该树立『C++ 不是 C 的一个超集』这个观念

​	在编写 C++ 时，**也应该尽可能 的避免使用诸如 void* 之类的程序风格。**而在不得不使用 C 时，**应该注意使用 extern "C" 这种特性**， 将 **C 语言的代码与 C++ 代码进行分离编译**，再**统一链接**这种做法，



`foo.h` 

```c++
#ifdef __cplusplus
extern "C" {
#endif
	int add(int x, int y);
#ifdef __cplusplus
}
#endif
```

`foo.c`

```c
int add(int x, int y) {
	return x+y;
}
```

`1.1 cpp`

```c++
#include "foo.h"
#include <iostream>
#include <functional>
int main() {
    [out = std::ref(std::cout << "Result from C code: " << add(1, 2))](){
    	out.get() << ".\n";
    }();
	return 0;
}
```

应先用gcc 编译C语言的代码

```sh
gcc -c foo.c
```



编译出 foo.o 文件，再使用 clang++ 将 C++ 代码和 .o 文件链接起来（或者都编译为 .o 再统一 链接）：



```sh
clang++ 1.1.cpp foo.o -std=c++2a -o 1.1 
```

当然，你可以使用 Makefile 来编译上面的代码：

```makefile
C = gcc
CXX = clang++

SOURCE_C = foo.c
OBJECTS_C = foo.o

SOURCE_CXX = 1.1.cpp

TARGET = 1.1
LDFLAGS_COMMON = -std=c++2a

all:
    $(C) -c $(SOURCE_C)
    $(CXX) $(SOURCE_CXX) $(OBJECTS_C) $(LDFLAGS_COMMON) -o $(TARGET)
clean:
	rm -rf *.o $(TARGET)
```



​	注意：Makefile 中的缩进是制表符而不是空格符，如果你直接复制这段代码到你的编辑器中， 制表符可能会被自动替换掉，请自行确保在 Makefile 中的缩进是由制表符完成的。 如果你还不知道 Makefile 的使用也没有关系，本教程中不会构建过于复杂的代码，简单的 在命令行中使用 clang++ -std=c++2a 也可以阅读本书。 如果你是首次接触现代 C++，那么你很可能还看不懂上面的那一小段代码，即：

```c++
[out = std::ref(std::cout << "Result from C code: " << add(1, 2))](){
	out.get() << ".\n";
}();
```



**进一步阅读的参考文献**

- C++ 语言导学. Bjarne Stroustrup 
- C++ 历史
- [C++ 特性在 GCC/Clang 等编译器中的支持情况](https://en.cppreference.com/w/cpp/compiler_support)
- C++98 与 C99 之间的区别





















