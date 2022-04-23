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

  ​	关于性能分析我其实更多的还是在代码中打印函数耗时，关于怎么优雅的打印函数耗时可以移步这里[**RAII妙用之计算函数耗时**](http://mp.weixin.qq.com/s?__biz=MzkyODE5NjU2Mw==&mid=2247484795&idx=1&sn=ba7f3ec2787246d1162f0d9565d122b6&chksm=c21d37c7f56abed15d1e843b2da8ff593512bed78d3f83cc6ef6decf86d2b253e5443d91b199&scene=21#wechat_redirect)。再推荐个性能分析的工具，**gperftools**，这个工具是Google出品，可以提供整个程序的热点分布图，方便我们找到性能的瓶颈。

![图片](https://mmbiz.qpic.cn/mmbiz_png/JeibBY5FJRBEuwrCJUMO5g7sfIfgHJMPWg3oreQeww0J4VuTRSm4Iup1VGSlzL652LBdv2QdmVD0oKrwhrNibbhA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)





#### new[] 和 delete[] 为何配对

​	因为 new[]会创建一个数组，一个对象数组需要一定的空间大小，假设一个对象需要 N 字节大小，K 个对象的数组就需 要 K*N 个空间来构造对象数组，但是在 delete[]时候，如何知道数组的长度呢？ 

​	所以 new[]会在 K*N 个空间的基础上，头部多申请 4 个字节，用于存储数组长度，这样 delete[]时候才知道对象数组的 大小，才会相应调用 K 次析构函数，并且释放 K*N+4 大小的内存。 

这是我们平时编程中经常配对使用的情况，如果不配对使用呢

```c++
int main(int argc, char *argv[]) {
    inner *p = new inner[2];
    delete p;
    return 0;
} 
```

```sh
16. 程序输出：
17. Constructing
18. Constructing
19. Destructing
20. munmap_chunk(): invalid pointer
21. Aborted (core dumped)
```

这里发现：程序挂掉了。

并且，只调用了一次析构函数，为什么呢？

因为我们使用了 delete，delete 不同于 delete[]，它认为这只是一个对象占用的空间，不是对象数组，不会访问前 4 个 字节获取长度，所以只调用了一次析构函数。

而且，最后释放内存的时候只释放了起始地址为 A 的内存。**然而这不是 这一整块内存的起始地址**，整块内存的起始地址应该是 A-4，**释放内存如果不从内存起始地址操作就会出现断错误**，所 以**导致程序挂掉**。



#### C++数组长度可以为变量吗

 C++是支持变量长度的数组的，说不支持的那是很古老的编译器

备注：尽管 C++目前支持变量长度的数组，但是不建议使用，因为数组使用的是栈内存，栈内存是有大小限制的，一般是 8192 字节，既然长度是变量，那就可能是任何值，就有可能超过 8192，**这样就会 stack overflow**，所以动态内存最好使用堆内存。

C++中数组长度可以是变量，但是不建议使用，因为数组使用的是栈内存，变量可以是个比较大的数，这样会导致 stack overflow， 建议使用堆内存。

 操作超过数组长度的内存可以编译通过且表面上看不出来问题，但是会导致栈内存出现脏写，最终可能会引发难以排查的 bug，

建议数组使用 std::array，操作超过长度的下标会抛异常有利于开发者及时发现错误。





#### 条件变量

【时间管理的本质是目的管理，如果我们想要更好的利用时间，最先要做的是找到最值得我们花费时间的事情，自我学习和精 进才是最值得我们做的有意义的事。】



**什么是条件变量**

条件变量是多线程程序中用来实现等待和唤醒逻辑常用的方法。通常有 wait 和 notify 两个动作，wait 用于阻塞挂起线程 A，直 到另一个线程 B 通过通过 notify 唤醒线程 A，唤醒后线程 A 会继续运行。 

条件变量在多线程中很常用，在有名的生产者和消费者问题中，消费者如何知道生成者是否生产出了可以消费的产品，通过 while 循环不停的去判断是否有可消费的产品？众所周知，死循环极其消耗 CPU 性能，所以需要使用条件变量来阻塞线程，降 低 CPU 占用率。

**条件变量的使用**

拿生产者和消费者问题举例，看下面这段代码：

```c++
#include <mutex>
#include <condition_variable>
#include <vector>
#include <iostream>
#include <thread>

std::mutex mutex;
std::condition_variable cv;
std::vector<int> vec;

void Consume() {
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock);
    std::cout << "consume " << vec.size() << "\n";
}
void Produce() {
    std::unique_lock<std::mutex> lock(mutex);
    vec.push_back(1);
    cv.notify_all();
    std::cout << "produce \n";
}
int main() {
    std::thread t(Consume);
    t.detach();
    Produce();
    return 0;
}
```



​	本意是消费者线程阻塞，等待生产者生产数据后去通知消费者线程，这样消费者线程就可以拿到数据去消费。 但这里有个问题：如果先执行的 Produce()，后执行的 Consume()，生产者提前生产出了数据，去通知消费者，但是此时消费 者线程如果还没有执行到 wait 语句，即线程还没有处于挂起等待状态，线程没有等待此条件变量上，那通知的信号就丢失了，后面 Consume()中才执行 wait 处于等待状态，但此时生产者已经不会再触发 notify，那消费者线程就会始终阻塞下去，出现 bug。 如何解决这个问题呢？可以附加一个判断条件，就可以解决这种信号丢失问题，见代码

通过增加附加条件可以解决信号丢失的问题，但这里还有个地方需要注意，消费者线程处于 wait 阻塞状态时，即使没有调用 notify，操作系统也会有一些概率会唤醒处于阻塞的线程，使其继续执行下去，这就是**虚假唤醒**问题，当出现了虚假唤醒后， 消费者线程继续执行，还是没有可以消费的数据，出现了 bug。 

那怎么解决虚假唤醒的问题呢，可以在线程由阻塞状态被唤醒后继续判断附加条件，看是否满足唤醒的条件，如果满足则继续 执行，如果不满足，则继续去等待，体现在代码中，即将 if 判断改为 while 循环判断，见代码

```c++
void Consume() {
    std::unique_lock<std::mutex> lock(mutex);
    while (vec.empty()) {
        cv.wait(lock);
    }
    std::cout << "consume " << vec.size() << "\n";
}
```

难道我们每次都需要使用 while 循环和附加条件来操作条件变量吗，这岂不是很麻烦，在 C++中其实有更好的封装，只需要调 用 wait 函数时在参数中直接添加附加条件即可，内部已经做好了 while 循环判断，直接使用即可，见代码

```c++
void Consume() {
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock, [&]() { return !vec.empty(); });	// C++ 封装了
    std::cout << "consume " << vec.size() << "\n";
}
```



**为什么条件变量需要和锁配合使用？**

为什么叫条件变量呢？因为内部是通过判断及修改某个全局变量来决定线程的阻塞与唤醒，多线程操作同一个变量肯定需要加 锁来使得线程安全。

同时一个简单的 wait 函数调用内部会很复杂的，有可能线程 A 调用了 wait 函数但是还没有进入到 wait 阻 塞等待前，另一个线程 B 在此时却调用了 notify 函数，此时 nofity 的信号就丢失啦，如果加了锁，线程 B 必须等待线程 A 释放 了锁并进入了等待状态后才可以调用 notify，继而防止信号丢失。



#### 虚函数的开销

虚函数的开销真的那么慢吗？开销究竟在哪里？

想必很多人都听说过虚函数开销大，貌似很多答案都说是因为虚函数表导致的那一次间接调用，真的如此吗？

```c++
#include <cmath>
#include "../time_log/TimeLog.h"

struct Base {
    public:
        virtual int f(double i1, int i2) {
            return static_cast<int>(i1 * log(i1)) * i2;
        }
};

int main() {
    ysy::timer::TimerLog t("timer");
    Base *a = new Base();
    int ai = 0;
    for (int i = 0; i < 1000000000; i++)
    {
        ai += a->f(i, 10);
    }
    std::cout << ai << std::endl;
    return 0;
}
```

执行时间：12.895s

```c++
#include <cmath>
#include "../time_log/TimeLog.h"

struct Base {
    public:
        int f(double i1, int i2) {
            return static_cast<int>(i1 * log(i1)) * i2;
        }
};

int main() {
    ysy::timer::TimerLog t("timer");
    Base *a = new Base();
    int ai = 0;
    for (int i = 0; i < 1000000000; i++)
    {
        ai += a->f(i, 10);
    }
    std::cout << ai << std::endl;
    return 0;
}
```

执行时间：12.706s

这两段代码的执行时间几乎没有区别，可见虚函数表导致的那一次函数间接调用并不浪费时间，所以虚函数的开销并不在重定 向上，这一次重定向基本上不影响程序性能。

那它的开销究竟在哪里呢？看下面两段代码，这两段代码和上面相比只改动了一行：

```c++
#include <cmath>
#include "../time_log/TimeLog.h"

struct Base {
    public:
        virtual int f(double i1, int i2) {
            return static_cast<int>(i1 * log(i1)) * i2;
        }
};

int main() {
    ysy::timer::TimerLog t("timer");
    Base *a = new Base();
    int ai = 0;
    for (int i = 0; i < 1000000000; i++)
    {
        ai += a->f(10, i);
    }
    std::cout << ai << std::endl;
    return 0;
}
```

执行时间：436ms

```c++
struct Base {
    public:
        virtual int f(double i1, int i2) {
            return static_cast<int>(i1 * log(i1)) * i2;
        }
};
```

执行时间 154ms

这里看到，仅仅改变了一行代码，虚函数调用就比普通函数慢了几倍，为什么？

虚函数其实最主要的性能开销在于**它阻碍了编译器内联函数和各种函数级别的优化**，导致性能开销较大，在 普通函数中 log(10)会被优化掉，它就只会被计算一次，而如果使用虚函数，log(10)不会被编译器优化，它就 会被计算多次。如果代码中使用了更多的虚函数，编译器能优化的代码就越少，性能就越低。

虚函数通常通过虚函数表来实现，在虚表中存储函数指针，实际调用时需要间接访问，这需要多一点时间。

然而这并不是虚函数速度慢的主要原因，

真正原因是编译器在**编译时通常并不知道它将要调用哪个函数**，**所 以它不能被内联优化和其它很多优化**，因此就会**增加很多无意义的指令**（准备寄存器、调用函数、保存状态 等），而且如果虚函数有很多实现方法，那**分支预测的成功率也会降低很多**，**分支预测错误也会导致程序性 能下降**。

如果你想要写出高性能代码并频繁的调用虚函数，注意如果用其它的方式（例如 if-else、switch、函数指针 等）来替换虚函数调用并不能根本解决问题，它还有可能会更慢，真正的问题不是虚函数，而是那些不必要的间接调用

正常的函数调用：

1. 复制栈上的一些寄存器，以允许被调用的函数使用这些寄存器；
2.  将参数复制到预定义的位置，这样被调用的函数可以找到对应参数；
3. 入栈返回地址；
4. 跳转到函数的代码，这是一个编译时地址，因为编译器/链接器硬编码为二进制；
5. 从预定义的位置获取返回值，并恢复想要使用的寄存器。

而虚函数调用与此完全相同，唯一的区别就是编译时不知道函数的地址，而是：

1. 从对象中获取虚表指针，该指针指向一个函数指针数组，每个指针对应一个虚函数；
2. 从虚表中获取正确的函数地址，放到寄存器中；
3. 跳转到该寄存器中的地址，而不是跳转到一个硬编码的地址

通常，使用虚函数没问题，它的性能开销也不大，而且虚函数在面向对象代码中有强大的作用

但是不能无脑使用虚函数，特别是在性能至关重要的或者底层代码中，而且大项目中使用多态也会导致继承层次很混乱

那么有什么好方法替代虚函数呢？这里提供几个思路，读者请持续关注，后续会具体讲解

1. 使用访问者模式来使类层次结构可扩展；
2. 使用普通模板替代继承和虚函数；
3. C++20 中的 concepts 用来替代面向对象代码；
4. 使用 variants 替代虚函数或模板方法



#### C++ 的对象布局（干货）

- 普通类对象是什么布局
- 带虚函数的类对象是什么布局
- 单继承下不含有覆盖函数的类对象是什么布局
- 单继承下含有覆盖函数的类对象是什么布局
- 多继承下不含有覆盖函数的类对象是什么布局
- 多继承下含有覆盖函数的类对象是什么布局
- 虚继承的类对象是什么布局
- 菱形继承下类对象是什么布局
- 为什么引入虚集成
- 为什么虚函数表中有两个虚构函数
- 为什么构造函数不能是虚函数
- 为什么基类析构函数需要是虚函数？



**多态**

多态可以分为编译时多态和运行时多态

- 编译时多态：基于模板和函数重载方式，在编译时就已经确定对象的行为，也称为静态绑定
- 运行时多态：面向对象的一大特色，通过继承方式使得程序在运行时才会确定相应调用的方法，也称为动态绑定，它的实 现主要是依赖于传说中的虚函数表



**如何查看对象的布局**

在 gcc 中可以使用如下命令查看对象布局：

```sh
g++ -fdump-class-hierarchy model.cc 后查看生成的文件
```

在 clang 中可以使用如下命令：

```sh
clang -Xclang -fdump-record-layouts -stdlib=libc++ -c model.cc// 查看对象布局
clang -Xclang -fdump-vtable-layouts -stdlib=libc++ -c model.cc// 查看虚函数表布局
```

上面两种方式其实足够了，也可以使用 gdb 来查看内存布局，这里可以看文末相关参考资料。本文都是使用 clang 来查看的对 象布局。

接下来让我们一起来探秘下各种继承条件下类对象的布局情况吧~

**普通类对象的布局**

```c
#include <stdio.h>
struct Base {
    Base() = default;
    ~Base() = default;
    void Func() {}
    int a;
    int b;
};
int main() {
    Base a;
    return 0;
}
```

```sh
clang -Xclang -fdump-record-layouts -stdlib=libc++ -c model.cc
```

```sh
*** Dumping AST Record Layout
    0 | struct Base
    0 | int a
    4 | int b
    | [sizeof=8, dsize=8, align=4,
    | nvsize=8, nvalign=4]
*** Dumping IRgen Record Layout
```

从结果中可以看见，这个普通结构体 Base 的大小为 8 字节，a 占 4 个字节，b 占 4 个字节。



**带虚函数的类对象的布局**

```c++
struct Base {
    Base() = default;
    virtual ~Base() = default;
    virtual void FuncB() {
    	printf("FuncB\n");
    }
    int a;
    int b;
};
```

```sh
clang -Xclang -fdump-record-layouts -stdlib=libc++ -c model.cc
clang -Xclang -fdump-vtable-layouts -stdlib=libc++ -c model.cc
```

```sh
** Dumping AST Record Layout
    0 | struct Base
    0 | (Base vtable pointer)
    8 | int a
    12 | int b
    | [sizeof=16, dsize=16, align=8,
    | nvsize=16, nvalign=8]
*** Dumping IRgen Record Layout
```

这个含有虚函数的结构体大小为 16，在对象的头部，前 8 个字节是虚函数表的指针，指向虚函数的相应函数指针地址，a 占 4 个字节，b 占 4 个字节，总大小为 16。

虚函数表布局：

```sh
Vtable for 'Base' (5 entries).
    0 | offset_to_top (0)
    1 | Base RTTI
    -- (Base, 0) vtable address --
    2 | Base::~Base() [complete]
    3 | Base::~Base() [deleting]
    4 | void
```

画出对象布局图如下：

![img](https://imgconvert.csdnimg.cn/aHR0cHM6Ly9tbWJpei5xcGljLmNuL21tYml6X2pwZy85bEZGRmlhS3BFcmlieVZraWNTRkRLaWNiZHRNOFA5NWljMHc4RXdjaWJ0Sm9CdVBwWTRva0JObjhYblNLVkluZU85dkM0TEJkc2RzbDlYSGliUGM5ZDEzQ2VNTmcvNjQw?x-oss-process=image/format,png)

我们来探秘下传说中的虚函数表：

**offset_to_top(0)：** 表示当前这个虚函数表地址距离对象顶部地址的偏移量，因为对象的头部就是虚函数表的指针，所以偏移量 为 0

**RTTI 指针：**指向存储运行时类型信息(type_info)的地址，用于运行时类型识别，用于 typeid 和 dynamic_cast

RTTI 下面就是虚函数表指针真正指向的地址啦，存储了类里面所有的虚函数，至于这里为什么会有两个析构函数，大家可以先 关注对象的布局，最下面会介绍。



**单继承下不含有覆盖函数的类对象的布局**

```c++
struct Base {
    Base() = default;
    virtual ~Base() = default;
    void FuncA() {}
    virtual void FuncB() {
        printf("Base FuncB\n");
    }
    int a;
    int b;
};
struct Derive : public Base{};
int main() {
    Base a;
    Derive d;
    printf("%d\n", sizeof(a));
    printf("%d\n", sizeof(d));
    return 0;
}
```

```sh
*** Dumping AST Record Layout
    0 | struct Derive
    0 | struct Base (primary base)
    0 | (Base vtable pointer)
    8 | int a
    12 | int b
    | [sizeof=16, dsize=16, align=8,
    | nvsize=16, nvalign=8]
*** Dumping IRgen Record Layou
```

和上面相同，这个含有虚函数的结构体大小为 16，在对象的头部，前 8 个字节是虚函数表的指针，指向虚函数的相应函数指针 地址，a 占 4 个字节，b 占 4 个字节，总大小为 16。 

子类虚函数表布局：

```sh
vtable for 'Derive' (5 entries).
    0 | offset_to_top (0)
    1 | Derive RTTI
    -- (Base, 0) vtable address --
    -- (Derive, 0) vtable address --
    2 | Derive::~Derive() [complete]
    3 | Derive::~Derive() [deleting]
    4 | 
```

画图如下

![img](https://imgconvert.csdnimg.cn/aHR0cHM6Ly9tbWJpei5xcGljLmNuL21tYml6X2pwZy85bEZGRmlhS3BFcmlieVZraWNTRkRLaWNiZHRNOFA5NWljMHc4S2ljbHlpYlhQRmlidjcxOFYwUWVpY3FpY2t6bzF4RFV3QnRlMWliYXZqZHlPbWpsMFVZMlIxZm5mUjJBLzY0MA?x-oss-process=image/format,png)

这个和上面也是相同的，注意下虚函数表这里 的 FuncB 函数，还是 Base 类中的 FuncB，因为在子类中没有重写这个函数，那么如果子类重写这个函数后对象布局是什么样 的，请继续往下看哈。



**单继承下含有覆盖函数的类对象的布局**

注意这里虚函数表中的 FuncB 函数已经是 Derive 中的 FuncB 啦，因为在子类中重写了父类的这个函数

再注意这里的 RTTI 中有了两项，表示 Base 和 Derive 的虚表地址是相同的，Base 类里的虚函数和 Derive 类里的虚函数都在这 个链条下，这里可以继续关注下面多继承的情况，看看有何不同

![img](https://imgconvert.csdnimg.cn/aHR0cHM6Ly9tbWJpei5xcGljLmNuL21tYml6X3BuZy85bEZGRmlhS3BFcmlieVZraWNTRkRLaWNiZHRNOFA5NWljMHc4V2liaWNkcVFDZUlZcmliSW16eEpnQllwMHVyZDRVSTZSdG1wd29hWmljUzR3ZFdmVTRoMkc4YUlldy82NDA?x-oss-process=image/format,png)



**多继承下不含有覆盖函数的类对象的布局**

类对象布局

```sh
*** Dumping AST Record Layout
    0 | struct Derive
        0 | struct BaseA (primary base)
            0 | (BaseA vtable pointer)
            8 | int a
            12 | int b
        16 | struct BaseB (base)
            16 | (BaseB vtable pointer)
            24 | int a
            28 | int b
    | [sizeof=32, dsize=32, align=8,
    | nvsize=32, nvalign=8
```

Derive 大小为 32，注意这里有了两个虚表指针，因为 Derive 是多继承，一般情况下继承了几个带有虚函数的类，对象布局中 就有几个虚表指针，并且子类也会继承基类的数据，一般来说，不考虑内存对齐的话，子类（继承父类）的大小=子类（不继 承父类）的大小+所有父类的大小。

 虚函数表布局：

```sh
Vtable for 'Derive' (10 entries).
0 | offset_to_top (0)
1 | Derive RTTI
    -- (BaseA, 0) vtable address --
    -- (Derive, 0) vtable address --
2 | Derive::~Derive() [complete]
3 | Derive::~Derive() [deleting]
4 | void BaseA::FuncB()
5 | offset_to_top (-16)
6 | Derive RTTI
-- (BaseB, 16) vtable address --
7 | Derive::~Derive() [complete]
	[this adjustment: -16 non-virtual]
8 | Derive::~Derive() [deleting]
	[this adjustment: -16 non-virtual]
9 | void BaseB::FuncC()
```

可画出对象布局图如下：

![img](https://imgconvert.csdnimg.cn/aHR0cHM6Ly9tbWJpei5xcGljLmNuL21tYml6X3BuZy85bEZGRmlhS3BFcmlieVZraWNTRkRLaWNiZHRNOFA5NWljMHc4bDRjcTVHS3ZzdTY3U1EwUWN0aFpEMGpoWTFsUVphYndQS0ozaEtCTjhvb3lFQ0pGbzBhbnJnLzY0MA?x-oss-process=image/format,png)

**offset_to_top(0)：**表示当前这个虚函数表（BaseA，Derive）地址距离对象顶部地址的偏移量，因为对象的头部就是虚函数表的 指针，所以偏移量为 0

再注意这里的 RTTI 中有了两项，表示 BaseA 和 Derive 的虚表地址是相同的，BaseA 类里的虚函数和 Derive 类里的虚函数都在 这个链条下，截至到 offset_to_top(-16)之前都是 BaseA 和 Derive 的虚函数表

**offset_to_top(-16)：**表示当前这个虚函数表（BaseB）地址距离对象顶部地址的偏移量，因为对象的头部就是虚函数表的指针， 所以偏移量为-16，这里用于 this 指针偏移，下一小节会介绍

注意下后面的这个 RTTI：只有一项，表示 BaseB 的虚函数表，后面也有两个虚析构函数，为什么有四个 Derive 类的析构函数 呢，又是怎么调用呢，请继续往下看~



**多继承下含有覆盖函数的类对象的布局**

```C++
struct BaseA {
   BaseA() = default;
   virtual ~BaseA() = default;
   
   void FuncA() {}
 
 
   virtual void FuncB() {
       printf("BaseA FuncB\n");
  }
    
   int a;
   int b;
};
 
 
struct BaseB {
   BaseB() = default;
   virtual ~BaseB() = default;
   
   void FuncA() {}
 
 
   virtual void FuncC() {
       printf("BaseB FuncC\n");
  }
 
 
   int a;
   int b;
};
 
 
struct Derive : public BaseB, public BaseA{
   void FuncB() override {
       printf("Derive FuncB \n");
  }
 
 
   void FuncC() override {
       printf("Derive FuncC \n");
  }
};
 
 
int main() {
   BaseA a;
   Derive d;
   return 0;
}
```

对象布局

```sh
*** Dumping AST Record Layout
0 | struct Derive
0 | 	struct BaseA (primary base)
        0 | (BaseA vtable pointer)
        8 | int a
        12 | int b
16 | 	struct BaseB (base)
        16 | (BaseB vtable pointer)
        24 | int a
        28 | int b
| [sizeof=32, dsize=32, align=8,
| nvsize=32, nvalign=8]
*** Dumping IRgen Record Layout
```

类大小仍然是 32，和上面一样。

 虚函数表布局：

```sh
Vtable for 'Derive' (11 entries).
  0 | offset_to_top (0)
  1 | Derive RTTI
      -- (BaseB, 0) vtable address --
      -- (Derive, 0) vtable address --
  2 | Derive::~Derive() [complete]
  3 | Derive::~Derive() [deleting]
  4 | void Derive::FuncC()
  5 | void Derive::FuncB()
  6 | offset_to_top (-16)
  7 | Derive RTTI
      -- (BaseA, 16) vtable address --
  8 | Derive::~Derive() [complete]
      [this adjustment: -16 non-virtual]
  9 | Derive::~Derive() [deleting]
      [this adjustment: -16 non-virtual]
 10 | void Derive::FuncB()
      [this adjustment: -16 non-virtual]
```

对象布局图如下：

![img](https://imgconvert.csdnimg.cn/aHR0cHM6Ly9tbWJpei5xcGljLmNuL21tYml6X3BuZy85bEZGRmlhS3BFcmlieVZraWNTRkRLaWNiZHRNOFA5NWljMHc4bDRjcTVHS3ZzdTY3U1EwUWN0aFpEMGpoWTFsUVphYndQS0ozaEtCTjhvb3lFQ0pGbzBhbnJnLzY0MA?x-oss-process=image/format,png)

虚函数表的布局也有所不同，BaseB和Derive共用一个虚表地址，在整个虚表布局的上方，而布局的下半部分是BaseA的虚表，可见继承顺序不同，子类的虚表布局也有所不同。

**虚集成的布局**

```c++
struct Base {
   Base() = default;
   virtual ~Base() = default;
   
   void FuncA() {}
 
 
   virtual void FuncB() {
       printf("BaseA FuncB\n");
  }
 
 
   int a;
   int b;
};
 
 
struct Derive : virtual public Base{
   void FuncB() override {
       printf("Derive FuncB \n");
  }
};
 
 
int main() {
   Base a;
   Derive d;
   return 0;
}
```

对象布局：

```
*** Dumping AST Record Layout
        0 | struct Derive
        0 |   (Derive vtable pointer)
        8 |   struct Base (virtual base)
        8 |     (Base vtable pointer)
       16 |     int a
       20 |     int b
          | [sizeof=24, dsize=24, align=8,
          |  nvsize=8, nvalign=8]
 
 
*** Dumping IRgen Record Layout
```

虚继承下，这里的对象布局和普通单继承有所不同，普通单继承下子类和基类共用一个虚表地址，而在虚继承下，子类和**虚基类分别有一个虚表地址的指针**，两个指针大小总和为16，再加上a和b的大小8，为24。

**虚函数表**

```sh
Vtable for 'Derive' (13 entries).
  0 | vbase_offset (8)
  1 | offset_to_top (0)
  2 | Derive RTTI
      -- (Derive, 0) vtable address --
  3 | void Derive::FuncB()
  4 | Derive::~Derive() [complete]
  5 | Derive::~Derive() [deleting]
  6 | vcall_offset (-8)
  7 | vcall_offset (-8)
  8 | offset_to_top (-8)
  9 | Derive RTTI
      -- (Base, 8) vtable address --
 10 | Derive::~Derive() [complete]
      [this adjustment: 0 non-virtual, -24 vcall offset offset]
 11 | Derive::~Derive() [deleting]
      [this adjustment: 0 non-virtual, -24 vcall offset offset]
 12 | void Derive::FuncB()
      [this adjustment: 0 non-virtual, -32 vcall offset offset]
```

对象布局图如下：

![img](https://imgconvert.csdnimg.cn/aHR0cHM6Ly9tbWJpei5xcGljLmNuL21tYml6X3BuZy85bEZGRmlhS3BFcmlieVZraWNTRkRLaWNiZHRNOFA5NWljMHc4VFJkWVdtYVJpYnZpY3lhSVNqR0ZhWk5pYlMwaWNLMHJZU043a2licWwzZXl3WEJvNFhkUTQyV3pQaWFRLzY0MA?x-oss-process=image/format,png)

**vbase_offset(8)**：对象在对象布局中与指向虚基类虚函数表的指针地址的偏移量

**vcall_offset(-8)**：当**虚基类**Base的引用或指针base实际接受的是Derive类型的对象，执行base->FuncB()时候，由于FuncB()已经被重写，而此时的this指针指向的是Base类型的对象，需要对this指针进行调整，就是vcall_offset(-8)，所以this指针向上调整了8字节，之后调用FuncB()，就调用到了被重写后的FuncB()函数。



**虚继承带未覆盖函数的对象布局**

```C++
struct Base {
   Base() = default;
   virtual ~Base() = default;
   void FuncA() {}
   virtual void FuncB() {
       printf("Base FuncB\n");
  }
   virtual void FuncC() {
       printf("Base FuncC\n");
  }
   int a;
   int b;
};
 
struct Derive : virtual public Base{
   void FuncB() override {
       printf("Derive FuncB \n");
  }
};
 
int main() {
   Base a;
   Derive d;
   return 0;
}
```

对象布局：

```sh
*** Dumping AST Record Layout
        0 | struct Derive
        0 |   (Derive vtable pointer)
        8 |   struct Base (virtual base)
        8 |     (Base vtable pointer)
       16 |     int a
       20 |     int b
          | [sizeof=24, dsize=24, align=8,
          |  nvsize=8, nvalign=8]
 
 
*** Dumping IRgen Record Layout
```

和上面虚继承情况下相同，普通单继承下子类和基类共用一个虚表地址，而在虚继承下，子类和虚基类分别有一个虚表地址的指针，两个指针大小总和为16，再加上a和b的大小8，为24。

虚函数表布局：

```sh
Vtable for 'Derive' (15 entries).
  0 | vbase_offset (8)
  1 | offset_to_top (0)
  2 | Derive RTTI
      -- (Derive, 0) vtable address --
  3 | void Derive::FuncB()
  4 | Derive::~Derive() [complete]
  5 | Derive::~Derive() [deleting]
  6 | vcall_offset (0)
  7 | vcall_offset (-8)
  8 | vcall_offset (-8)
  9 | offset_to_top (-8)
 10 | Derive RTTI
      -- (Base, 8) vtable address --
 11 | Derive::~Derive() [complete]
      [this adjustment: 0 non-virtual, -24 vcall offset offset]
 12 | Derive::~Derive() [deleting]
      [this adjustment: 0 non-virtual, -24 vcall offset offset]
 13 | void Derive::FuncB()
      [this adjustment: 0 non-virtual, -32 vcall offset offset]
 14 | void Base::FuncC()
```

对象布局图如下：

![img](https://imgconvert.csdnimg.cn/aHR0cHM6Ly9tbWJpei5xcGljLmNuL21tYml6X3BuZy85bEZGRmlhS3BFcmlieVZraWNTRkRLaWNiZHRNOFA5NWljMHc4SXF5d1ZJRTFjVlZTUXJLNzRWTjVJZHViZUFBUHdXRGdudjUzcTRuVWI2OFBSbTE1aWFQaWN1RmcvNjQw?x-oss-process=image/format,png)

**vbase_offset(8)**：对象在对象布局中与指向虚基类虚函数表的指针地址的偏移量

**vcall_offset(-8)**：当**虚基类**Base的引用或指针base实际接受的是Derive类型的对象，执行base->FuncB()时候，由于FuncB()已经被重写，而此时的this指针指向的是Base类型的对象，需要对this指针进行调整，就是vcall_offset(-8)，所以this指针向上调整了8字节，之后调用FuncB()，就调用到了被重写后的FuncB()函数。

**vcall_offset(0)**：当Base的引用或指针base实际接受的是Derive类型的对象，执行base->FuncC()时候，由于FuncC()没有被重写，所以不需要对this指针进行调整，就是vcall_offset(0)，之后调用FuncC()。



**菱形继承下类对象的布局**

```c++
struct Base {
   Base() = default;
   virtual ~Base() = default;
   
   void FuncA() {}
   virtual void FuncB() {
       printf("BaseA FuncB\n");
  }
   int a;
   int b;
};
 
 
struct BaseA : virtual public Base {
   BaseA() = default;
   virtual ~BaseA() = default;
   void FuncA() {}
 
   virtual void FuncB() {
       printf("BaseA FuncB\n");
  }
 
 
   int a;
   int b;
};
 
 
struct BaseB : virtual public Base {
   BaseB() = default;
   virtual ~BaseB() = default;
   
   void FuncA() {}
 
   virtual void FuncC() {
       printf("BaseB FuncC\n");
  }

   int a;
   int b;
};

struct Derive : public BaseB, public BaseA{
   void FuncB() override {
       printf("Derive FuncB \n");
  }

   void FuncC() override {
       printf("Derive FuncC \n");
  }
};

int main() {
   BaseA a;
   Derive d;
   return 0;
}
```

类对象布局：

```sh
*** Dumping AST Record Layout
        0 | struct Derive
        0 |   struct BaseB (primary base)
        0 |     (BaseB vtable pointer)
        8 |     int a
       12 |     int b
       16 |   struct BaseA (base)
       16 |     (BaseA vtable pointer)
       24 |     int a
       28 |     int b
       32 |   struct Base (virtual base)
       32 |     (Base vtable pointer)
       40 |     int a
       44 |     int b
          | [sizeof=48, dsize=48, align=8,
          |  nvsize=32, nvalign=8]
 
 
*** Dumping IRgen Record Layout
```

虚函数表：

```sh
Vtable for 'Derive' (20 entries).
  0 | vbase_offset (32)
  1 | offset_to_top (0)
  2 | Derive RTTI
      -- (BaseB, 0) vtable address --
      -- (Derive, 0) vtable address --
  3 | Derive::~Derive() [complete]
  4 | Derive::~Derive() [deleting]
  5 | void Derive::FuncC()
  6 | void Derive::FuncB()
  7 | vbase_offset (16)
  8 | offset_to_top (-16)
  9 | Derive RTTI
      -- (BaseA, 16) vtable address --
 10 | Derive::~Derive() [complete]
      [this adjustment: -16 non-virtual]
 11 | Derive::~Derive() [deleting]
      [this adjustment: -16 non-virtual]
 12 | void Derive::FuncB()
      [this adjustment: -16 non-virtual]
 13 | vcall_offset (-32)
 14 | vcall_offset (-32)
 15 | offset_to_top (-32)
 16 | Derive RTTI
      -- (Base, 32) vtable address --
 17 | Derive::~Derive() [complete]
      [this adjustment: 0 non-virtual, -24 vcall offset offset]
 18 | Derive::~Derive() [deleting]
      [this adjustment: 0 non-virtual, -24 vcall offset offset]
 19 | void Derive::FuncB()
      [this adjustment: 0 non-virtual, -32 vcall offset offset]
```

对象布局图如下：

![img](https://imgconvert.csdnimg.cn/aHR0cHM6Ly9tbWJpei5xcGljLmNuL21tYml6X3BuZy85bEZGRmlhS3BFcmlieVZraWNTRkRLaWNiZHRNOFA5NWljMHc4Ykg3WjBtUThWVDRISlFuZWliajFGMWxXV2JUZWh2UzJ0SkEwdjFpY21xUHBIZUtvaWNwSWtQd3ZRLzY0MA?x-oss-process=image/format,png)



**vbase_offset (32)**

**vbase_offset (16)**：对象在对象布局中与指向虚基类虚函数表的指针地址的偏移量

**offset_to_top (0)**

**offset_to_top (-16)**

**offset_to_top (-32)**：指向虚函数表的地址与对象顶部地址的偏移量。

**vcall_offset(-32)**：当**虚基类**Base的引用或指针base实际接受的是Derive类型的对象，执行base->FuncB()时候，由于FuncB()已经被重写，而此时的this指针指向的是Base类型的对象，需要对this指针进行调整，就是vcall_offset(-32)，所以this指针向上调整了32字节，之后调用FuncB()，就调用到了被重写后的FuncB()函数。





**为什么要虚继承？**

如图：

![img](https://imgconvert.csdnimg.cn/aHR0cHM6Ly9tbWJpei5xcGljLmNuL21tYml6X2pwZy85bEZGRmlhS3BFcmlieVZraWNTRkRLaWNiZHRNOFA5NWljMHc4QlNTcXBrbE1jUk14UjFTaDJlbjc4cWlhc3A0VmFTZ2pqYkNwRjhiMUxDTUFtSHJyVW9YSURjdy82NDA?x-oss-process=image/format,png)

非虚继承时，显然D会继承两次A，内部就会存储两份A的数据浪费空间，而且还有二义性，D调用A的方法时，由于有两个A，究竟时调用哪个A的方法呢，编译器也不知道，就会报错，所以有了虚继承，解决了空间浪费以及二义性问题。在虚拟继承下，只有一个共享的基类子对象被继承，而无论该基类在派生层次中出现多少次。共享的基类子对象被称为虚基类。在虚继承下，基类子对象的复制及由此而引起的二义性都被消除了。



**为什么虚函数表中有两个析构函数？**

前面的代码输出中我们可以看到虚函数表中有两个析构函数，**一个标志为deleting，一个标志为complete**，因为对象有两种构造方式，栈构造和堆构造，所以对应的实现上，对象也有两种析构方式，其中堆上对象的析构和栈上对象的析构不同之处在于，栈内存的析构不需要执行 delete 函数，会自动被回收。





**为什么构造函数不能是虚函数？**

构造函数就是为了在编译阶段确定对象的类型以及为对象分配空间，如果类中有虚函数，那就会在构造函数中初始化虚函数表，虚函数的执行却需要依赖虚函数表。如果构造函数是虚函数，那它就需要依赖虚函数表才可执行，而只有在构造函数中才会初始化虚函数表，鸡生蛋蛋生鸡的问题，很矛盾，所以构造函数不能是虚函数。



**为什么基类析构函数要是虚函数？**

一般基类的析构函数都要设置成虚函数，因为如果不设置成虚函数，在析构的过程中只会调用到基类的析构函数而不会调用到子类的析构函数，可能会产生内存泄漏。





**总结**

**offset_to_top**：对象在对象布局中与对象顶部地址的偏移量。

**RTTI指针**：指向存储运行时类型信息(type_info)的地址，用于运行时类型识别，用于typeid和dynamic_cast。

**vbase_offset**：对象在对象布局中与指向虚基类虚函数表的指针地址的偏移量。

**vcall_offset**：父类引用或指针指向子类对象，调用被子类重写的方法时，用于对虚函数执行指针地址调整，方便成功调用被重写的方法。

**thunk**: 表示上面虚函数表中带有adjustment字段的函数调用需要先进行this指针调整，才可以调用到被子类重写的函数。

最后通过两张图总结一下对象在Linux中的布局：

```c++
A *a = new Derive(); // A为Derive的基类
```

如图：

![img](https://imgconvert.csdnimg.cn/aHR0cHM6Ly9tbWJpei5xcGljLmNuL21tYml6X3BuZy85bEZGRmlhS3BFcmlieVZraWNTRkRLaWNiZHRNOFA5NWljMHc4VVAwSVRmNEpvQnQ4MmJENnJMb2lhMzdWVzN6UTE3Q3o1QnNocFBpY3RMankzY0ZjOFE0V2xtZWcvNjQw?x-oss-process=image/format,png)

**a作为对象指针存储在栈中，指向在堆中的类A的实例内存，其中实例内存布局中有虚函数表指针，指针指向的虚函数表存放在数据段中，虚函数表中的各个函数指针指向的函数在代码段中。**

![img](https://imgconvert.csdnimg.cn/aHR0cHM6Ly9tbWJpei5xcGljLmNuL21tYml6X3BuZy85bEZGRmlhS3BFcmlieVZraWNTRkRLaWNiZHRNOFA5NWljMHc4QU02blJQbHcwWEhERUI5THFyYTVRWVlWVXVrWndFRmd3M2JpY2lhamV4NGxDSEMxNjJxaWFPR1hRLzY0MA?x-oss-process=image/format,png)

虚表结构大体如上图，正常的虚表结构中都含有后三项，当有虚继承情况下会有前两个表项。





















