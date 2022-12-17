# Union 和 enum



## Union

Union，也就是联合体。在C++中可以从来处理 n选1 的变量占用同样[内存](https://so.csdn.net/so/search?q=内存&spm=1001.2101.3001.7020)，从而达到节省资源的目的。

例如看下面一个例子：

```cpp
union demo{
	int a;
	char b;
}c;
cout<< sizeof(c)<<endl;// 4 bytes
```

也就是说，变量 a和b 都是占用着同样的内存空间。

### 大小端测试

那怎么利用它来测试大端小端呢？看下面的代码。

```cpp
c.a = 1;
return 1 == c.b;//返回1则为小端法，否则为大端法
```

看地址，假设地址为 0x01 0x02 0x03 0x04
则 小端法中 c.a 赋值为 01 00 00 00
此时 c.b的情况是： 01 那么输出就是1.

若是大端法： c.a 为 00 00 00 01
则c.b为： 00
所以此时b的值为0.

下面可以补充一下Union的东西
1、Union里面不能放 static 变量（因为共享内存）static存在静态储存区
2、Union里面不能放 有构造函数和析构函数的类对象

## enum

枚举是用户定义的数据类型，由整数常量组成。可以使用关键字**enum**，定义枚举





### 为什么在C ++编程中使用枚举？

枚举变量仅取许多可能值中的一个值。如下示例来演示它：

```cpp
#include <iostream>
using namespace std;

enum suit {
    club = 0,
    diamonds = 10,
    hearts = 20,
    spades = 3
} card;

int main() 
{
    card = club;
    cout << "enum变量的大小 " << sizeof(card) << " bytes.";   
    return 0;
}
```

**输出结果**

```text
enum变量的大小 4 bytes.
```

这是因为整数的大小为4个字节。

这使得enum成为使用标志的一个很好的选择。

您可以使用[C ++结构](https://link.zhihu.com/?target=https%3A//www.nhooo.com/cpp/cpp-structure.html)完成相同的任务。但是，使用枚举可以提高效率和灵活性。

您无需使用枚举就可以在C ++编程中完成几乎所有任务。但是，它们在某些情况下可能非常方便。这也或许是优秀程序员与更优秀程序员的不同之处



### [C++ enum枚举类型详解](http://c.biancheng.net/view/1411.html)





### 枚举类

`enum class` 和 `enum`  的区别

前者为`C++ 11` 引入特性

众所周知在C++中，变量名字仅仅在一个作用域内生效，出了大括号作用域，那么变量名就不再生效了。但是传统C++的enum却特殊，只要有作用域包含这个枚举类型，那么在这个作用域内这个枚举的变量名就生效了。即枚举量的名字泄露到了包含这个枚举类型的作用域内。在这个作用域内就不能有其他实体取相同的名字。所以在C++98中这个称之为不限范围的枚举型别

```cpp
enum Color{black,white,red};	//black、white、red作用域和color作用域相同

auto white = false;	//错误，white已经被声明过了
```



**枚举类优势——降低命名空间污染**

```cpp
enum class Color{black,white,red}; //black、white、red作用域仅在大括号内生效

auto white = false;		//正确，这个white并不是Color中的white

Color c = white;	//错误，在作用域范围内没有white这个枚举量

Color c = Color::white;	//正确

auto c = Color::white;	//正确
```

**枚举类优势——避免发生隐式转换**

传统的不限范围的枚举类是可以发生隐式转换的：

```cpp
enum Color{black,white,red};
std::vector<std::size_t> primeFactors(std::size_t x);	//函数返回x的质因数

Color c = red;

if(c < 14.5)	//将color型别和double型别比较，发生隐式转换
{
	auto factors = primeFactors(c);  //计算一个color型别的质因数，发生隐式转换
}
```

限定作用域的枚举型别不允许发生任何隐式转换。如果非要转换，按就只能使用static_cast进行强制转换。

```cpp
enum class Color{black,white,red};
Color c = Color::red;

if(c < 14.5)	//错误，不能将枚举类和double进行比较
{
	auto factors = primeFactors(c); //错误，Color不能转化为size_t型别
}
```

**枚举类优势——可以前置声明**

枚举类可以进行前置声明，即型别名字可以比其中的枚举量先声明：

```cpp
enum Color;			//错误
enum class Color;	//正确
```

传统的不限作用域的枚举也可以前置声明，但是需要先做其他的工作。因为一切枚举型别在C++里都需要由编译器来选择一个整型型别作为其底层型别。为了节约使用内存，编译器通常会为枚举型别选用足够表示枚举量取值的最小底层型别。但某些情况下，编译器会用空间换取时间，他们可能不选择只具备最小可容尺寸的型别（当然同时要具备优化空间的能力）。在为了使这种设计成为可能，C++98就只提供了枚举型别的定义（即列出所有的枚举量），而不支持声明。这样编译器就可能在枚举类型使用前逐个的确定其底层型别选择哪一种。

  这种前置声明的缺失会造成一些弊端。假设我们有一个枚举型别，如果我们在这个枚举中新增了一个变量，那么整个用到枚举型别的系统都会因此重新编译，哪怕整个系统只有一个函数用到了这个枚举型别中新的枚举量。而使用C++11的枚举类就可以避免:

```cpp
enum Color:std:uint8_t;	//提前指定底层型别，可以进行前置声明

enum class Status:std::uint32_t{		//在定义中设置底层型别
	good = 0,
	failed = 1,
	incomplate = 100
};
```





传统的enum并非被完全取代了，在一种情况下它还是具有优势，即需要引用C++11中的std::tuple型别的各个域时。

```cpp
//********文件A********//
using UserInfo = 
std::tuple<std::string, //名字
			std::string //邮件
			std::size_t>; //声望值

//********文件B*******//
UserInfo uInfo;

auto val = std::get<1>(uInfo);  //取用域1的值
```

在上述代码中，在文件B中我们要取tuple中第二个值，但是如果第一次接触这段代码，很难知道这第2个值到底是什么意思。使用不限范围的枚举型别和域序数关联就可以消除这种问题：

```cpp
eunm UserInfoFields{uiName,uiEmail,uiReputation};

UserInfo uInfo;

auto val = std::get<uiEmail>(uInfo);  //一目了然，要去邮件这个值
```

以上代码能够运行的原理就是不限范围的枚举类型可以隐式转换。而使用enum class来定义的话，由于限定作用域的枚举类型不接受隐式转换，就要使用static_cast进行强转，整个代码会变得很啰嗦。当然，这种啰嗦的写法也确实能避免由于不限定作用域而带来的技术缺陷。

