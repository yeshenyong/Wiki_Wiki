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















