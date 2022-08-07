# C++ typeid

写这篇文章的初衷：

1. 许多博客对`typeid ` 解释仅限于对 `RTTI` 运行时类型识别的作用（一度我以为只有这个作用）
2. `typeid` 是操作符，而不是函数，就跟 `sizeof` 类似

首先需要知道一件事情，C、C++ 不是动态语言

这就导致了我一开始的误区

- C、C++ 不是动态语言
- `typeid` 仅限于`RTTI`（我之前的误区）

错误的结论：那`typeid` 没啥用处

直到开始看到一些开源代码，里面用了`typeid` 操作符，咦？还可以这样用？不是`RTTI` 吗？导致了今天的这一篇文章

### 什么是typeid

查询某个类型的信息

前提：在必须知道多态对象的动态类型和静态类型标识下才可以使用

`typeid` 分两种识别途径

1. 编译时期识别（基本类型）
2. 引用类型/指针类型 运行时识别（多态与非多态）

### 为什么使用typeid

顾名思义，它的主要作用还是

> 获取一个表达式是类型，返回表达式的类型

但个人觉得它最大的作用还是辅助性功能，利用它编译期识别（基本类型）

1. 辅助`define`

```cpp
#define IF_CAST_TYPE(tp) \
if (type == typeid(tp) { \
return static_cast<T>(AnyCast<tp>(number)); \
}
IF_CAST_TYPE(uint8_t)
else
IF_CAST_TYPE(uint16_t)
...
```

根据传入 `tp`  的类型信息，结合`boost::any` 进行类型转换

### 怎么用 typeid

头文件

```cpp
#include <typeinfo>
```

例子

```c++
#include <iostream>
#include <typeinfo>

using namespace std;

class Base {
public:
    virtual ~Base() {}
};

class Child : public Base {
public:
    ~Child() {}
};

int main()
{
    // 基础数据类型
    short s1 = 5;
    unsigned ui1 = 20;
    int i1 = 50, i2 = 100;
    char c1 = 'a';
    wchar_t wch1 = L'b';
    float f1 = 3.14159265f;

    Base* base = new Base();
    Base* base_child = new Child();
    Child* child = new Child();
	// 编译器决定
    cout << typeid(s1).name() << endl;// s
    cout<<typeid(ui1).name()<<endl;   // j
    cout<<typeid(i1).name()<<endl;    // i
    cout<<typeid(c1).name()<<endl;   // c
    cout<<typeid(wch1).name()<<endl;  // w
    cout<<typeid(f1).name()<<endl;    // f
    // 多态下
    cout<<typeid(base).name()<<endl;    // P4Base
    cout<<typeid(base_child).name()<<endl;  // P4Base
    cout<<typeid(*base_child).name()<<endl; // 5Child
    cout<<typeid(child).name()<<endl;   // P5Child

    delete base;
    delete base_child;
    delete child;


    return 0;
}
```



### 总结

注意：

> 尽量不要以typeid.name() 返回的名称用判断，更应该用 typeid(int) == typeid(s) 这样来对比，name() 不具有平台统一性









