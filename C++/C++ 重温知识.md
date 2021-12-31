# C++ 重温知识与编程要点

#### 头文件重复问题

头文件重复包含的影响

1. 使预处理的速度变慢了，要处理很多本来不需要处理的头文件。
2. 可能预处理器就陷入死循环了（其实编译器都会规定一个包含层数的上限）。例如C.h包含D.h，D.h又包含C.h的情况，如果不采用防止头文件的重复定义，那么预处理器就会进入死循环了。
3. 头文件里有些代码不允许重复出现。而重复定义头文件会重复出现一些代码。（虽然变量和函数允许多次声明（只要不是多次定义就行），但头文件里有些代码是不允许多次出现的）。例如：使用typedef类型定义和结构体Tag定义等，在一个程序文件中只允许出现一次。

##### 采用条件编译

```c++
#ifndef APP_H
#define APP_H
....
#endif //APP_H
```

##### 添加杂注

```c++
#pragma once
只要在头文件的最开始加入这条杂注，就能够保证头文件只被编译一次
```

​	\#ifndef，#define，#endif是C/C++语言中的宏定义，通过宏定义避免文件多次编译。所以在所有支持C++语言的编译器上都是有效的，如果写的程序要跨平台，最好使用这种方式。



#### #include "" 与 <>

 \#include <> 和 #include "" 都会在实现定义的位置查找文件，并将其包含

区别是若 #include "" 查找成功，则遮蔽 #include <> 所能找到的同名文件；否则再按照 #include <> 的方式查找文件。另外标准库头文件都放在 #include <> 所查找的位置。

一般来说 #include <> 的查找位置是标准库头文件所在目录， #include "" 的查找位置是当前源文件所在目录。不过这些都可由编译器调用参数等配置更改。

<>先去系统目录中找头文件，如果没有在到当前目录下找。所以像标准的头文件 stdio.h、stdlib.h等用这个方法。
而""首先在当前目录下寻找，如果找不到，再到系统目录中寻找。 这个用于include自定义的头文件，让系统优先使用当前目录中定义的。



#### 类的实现过程中探索编程习惯



类的设计习惯

- header头文件重复问题

- 数据放在private声明下，提供接口访问数据

- const返回不会改变（数据成员）的成员函数，加上const声明

  ```C++
  double real () `const` {return re;}
  double imag() `const` {return im;}
  而且，const对象才可以调用这些函数——const对象不能够调用非const成员函数。
  ```

- 使用构造函数初始值列表

```c++
class complex
{
    public:
        complex(double r = 0, double i =0)
            : re(r), im(i)  { }
    private:
        doubel re,im;
}
```

在初始值列表中，才是初始化。在构造函数体内的，叫做赋值

- 如果可以参数尽量使用reference to const

  ​	使用引用避免类对象构造与析构的开销，使用const确保参数不会被改变。内置类型的值传递与引用传递效率没有多大差别，甚至值传递效率会更高。

  ​	例如，传递`char`类型时，值传递只需传递一个字节；引用实际上是指针实现，需要四个字节（32位机）的传递开销。但是为了一致，不妨统一使用引用。

- 如果重载了操作符，就考虑是否需要多个重载

- 提供给外界使用的接口，放在类声明的最前面

```c++
class String
{
    public:
        String (const char * cstr = 0);
        String (const String & str);
        String & operator = (const String & str);
        ~String();
        char * get_c_str() const {return m_data};
    private:
        char * m_data;
}
```

- 析构函数释放动态分配的内存资源

- 赋值构造函数与复制构造函数负责进行深拷贝

① 处理自我赋值，如果不存在自我赋值问题，继续下列步骤：
② 释放自身已经申请的内存
③ 申请一块大小与目标字符串一样大的内存
④ 进行字符串的拷贝

同样的，复制构造函数也是一个深拷贝的过程：

```c++
inline String ::String(const String & str )
{
    m_data = new char[ strlen (str) +1];
    strcpy(m_data,str.m_data);
}
```

​	**一定要在operator = 中检查是否self assignment** 假设这时候确实执行了对象的自我赋值，左右pointers指向同一个内存块，前面的步骤②delete掉该内存块造成下面的结果。当企图对rhs的内存进行访问是，结果是未定义的。

- **不和对象直接相关的数据，声明为static**

  ​	存在`银行利率`这个成员变量，它不应该属于对象，而应该属于银行这个类，由所有的用户来共享。static修饰成员变量时，该成员变量放在程序的全局区中，整个程序运行过程中只有该成员变量的一份副本。而普通的成员变量存在每个对象的内存中，若把`银行利率`放在每个对象中，是浪费了内存。

- **static成员函数没有this指针**

  ​	static成员函数与普通函数一样，都是只有一份函数的副本，存储在进程的代码段上。不一样的是，static成员函数没有`this指针`，所以它不能够调用普通的成员变量，只能调用static成员变量。普通成员函数的调用需要通过对象来调用，编译器会把对象取地址，作为this指针的实参传递给成员函数：

  而static成员函数即可以通过对象来调用，也可以通过类名称来调用。

- **在类的外部定义static成员变量**

另一个问题是static成员变量的定义。static成员变量必须在类外部进行定义：

```c++
class A
{
    private:
        static int a; //①
}
int A::a = 10;  //②
```

注意①是声明，②才是定义，定义为变量分配了内存。跟进程分配的区域有关

- **static与类的一些小应用**

  ​	这些可以用来应付一下面试，在实现单例模式的时候，static成员函数与static成员变量得到了使用，下面是一种称为”饿汉式“的单例模式的实现：

  ```c++
  class A
  {
          public:
              static A& getInstance();
              setup(){...};
          private:
              A();
              A(const A & rhs);
              static A a;
  }
  ```

  这里把class A的构造函数都设置为私有，不允许用户代码创建对象。要获取对象实例需要通过接口`getInstance`。”饿汉式“缺点在于无论有没有代码需要`a`，`a`都被创建出来。下面是改进的单例模式，称为”懒汉式“：

  ```c++
  class A
  {
      public:
          static  A& getInstance();
          setup(){....};
      private:
          A();
          A(const A& rsh);
          ...
  };
  A& A::getInstance()
  {
          static A a;
          return a;
  }
  ```

  ​    	“懒汉式”只有在真正需要`a`时，调用`getInstance`才创建出唯一实例。这可以看成一个具有拖延症的单例模式，不到最后关头不干活。很多设计都体现了这种拖延的思想，比如string的写时复制，真正需要的时候才分配内存给string对象管理的字符串。









#### cout 和 printf之间的抉择

**个人看法是两个都不建议用。**

- **cout方式效率低**：每次cout << endl，都会刷新一遍输出缓冲区。

- **cout方式线程不安全**：

```c++
std::cout << "Hello " << "World ";
```

上面代码其实等同于：

```c++
std::operator<<(std::operator<<(std::cout, "Hello "), "World ");
```

相当于调用了两次operator<<，不保证调用两次operator<<是线程安全的。

- **cout方式输出复杂的格式很麻烦**



但printf需要开发者填入变量格式，如果填写有误，有可能打印错误，也有可能crash：



所以使用printf会给开发者带来负担，一定要在打印前确认变量的类型，并且一定要保证配置的输出格式正确，我想大多数人可能都会在print某个变量前搜索过打印某个类型需要使用哪种格式吧？



使用format可以把上面的问题全都解决，C++20标准已经全面支持使用std::format，如果不打算上C++20。可以考虑使用**spdlog**。



#### 定位crash问题（调试与排解）

​	关于crash问题我一般都是先看crash的报错，再查看堆栈信息，基本70%的crash问题都可以通过查看堆栈信息来定位，剩下的我一般都是结合log或者调试工具一起看。



- 调试逻辑问题

  ​	对于逻辑问题我一般都是打log，也建议大家平时开发过程中在一些关键的地方都打上log，开发过程中可能比较麻烦，但如果真的出了逻辑性的问题，而且又不是稳定复现，又没有对应的log信息，那真的相当麻烦。所以老铁们，多打log，不要嫌麻烦，log在关键时刻能起很大作用。

  ​	当然逻辑问题也可以使用gdb或者lldb打断点的方法来调试，在某些关键地方打上断点，然后查看某些变量的值，也可以使用某些IDE调试，我一般使用XCode来打断点进行调试。但这种方法有个缺点，如果程序Release出去后出现了逻辑问题，调试起来就比较麻烦，所以建议还是多打一些log。

  总结：log调试信息，bug排除gdb

- 内存问题

  ​	内存问题常见的就是内存泄漏啦，关于内存泄漏我一般都是先写个自己的内存检测的小工具用在项目中，C语言的话那就用宏覆盖malloc和free函数，C++的话那就重写operator new 和 operator delete函数，这里可以看我之前写过的文章[**我撸了个内存泄漏检测工具，只用了两招**](http://mp.weixin.qq.com/s?__biz=MzkyODE5NjU2Mw==&mid=2247485736&idx=1&sn=14f102f6499f9c6fa6ce2d7fc3c6a181&chksm=c21d3b94f56ab282d8ab121a6c8b979414b6f91686d0cf0ac1e15aa616db8266ed70d59a4223&scene=21#wechat_redirect)

- 调试死锁问题

  ​	如果**合理使用C++11的RAII**风格的锁或者**更现代的std::scope_lock**，那**基本不会发生死锁**。但如果真发生了死锁，怎么调试呢？可以使用IDE直接查看当前各个线程的堆栈信息，看哪两个线程处在wait状态，那基本上是这两个线程发生了死锁。

  ​	还可以使用gdb+deadlock.py的方式，deadlock.py是个检测死锁的python脚本，可以在gdb中执行，如果真的发生了死锁，直接执行脚本就可以检测出来，代码太长不好贴出来，deadlock.py的代码和使用方式，可以后台回复**deadlock**获取。

- 性能分析

  ​	关于性能分析我其实更多的还是在代码中打印函数耗时，关于怎么优雅的打印函数耗时可以移步这里[**RAII妙用之计算函数耗时**](http://mp.weixin.qq.com/s?__biz=MzkyODE5NjU2Mw==&mid=2247484795&idx=1&sn=ba7f3ec2787246d1162f0d9565d122b6&chksm=c21d37c7f56abed15d1e843b2da8ff593512bed78d3f83cc6ef6decf86d2b253e5443d91b199&scene=21#wechat_redirect)。再推荐个性能分析的工具，gperftools，这个工具是Google出品，可以提供整个程序的热点分布图，方便我们找到性能的瓶颈。

![图片](https://mmbiz.qpic.cn/mmbiz_png/JeibBY5FJRBEuwrCJUMO5g7sfIfgHJMPWg3oreQeww0J4VuTRSm4Iup1VGSlzL652LBdv2QdmVD0oKrwhrNibbhA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)















































