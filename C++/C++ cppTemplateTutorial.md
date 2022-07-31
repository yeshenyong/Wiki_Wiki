# cppTemplateTutorial

## 1. Template的基本语法

### 1.1 Template Class基本语法

#### 1.1.2 模板的使用

此时我们的程序需要一个整型和一个浮点型的列表，那么便可以通过以下代码获得两个变量：

```cpp
vector<int> intArray;
vector<float> floatArray;
```

此时我们就可以执行以下的操作，获得我们想要的结果：

```cpp
intArray.push_back(5);
floatArray.push_back(3.0f);
```

变量定义的过程可以分成两步来看：第一步，`vector<int>`将`int`绑定到模板类`vector`上，获得了一个“普通的类`vector<int>`”；第二步通过“vector”定义了一个变量。 与“普通的类”不同，模板类是不能直接用来定义变量的。例如：

```cpp
vector unknownVector; // 错误示例
```

这样就是错误的。我们把通过类型绑定将模板类变成“普通的类”的过程，称之为模板实例化（Template Instantiate）。实例化的语法是：

```cpp
模板名 < 模板实参1 [，模板实参2，...] >
```

看几个例子：

```cpp
vector<int>
ClassA<double>

template <typename T0, typename T1> class ClassB
{
    // Class body ...
};

ClassB<int, float>
```

当然，在实例化过程中，被绑定到模板参数上的类型（即模板实参）需要与模板形参正确匹配。 就如同函数一样，如果没有提供足够并匹配的参数，模板便不能正确的实例化。

#### 1.1.3 模板类的成员函数定义

由于C++11正式废弃“模板导出”这一特性，因此在模板类的变量在调用成员函数的时候，需要看到完整的成员函数定义。因此现在的模板类中的成员函数，通常都是以内联的方式实现。 例如：

```cpp
template <typename T>
class vector
{
public:
    void clear()
    {
        // Function body
    }
	
private:
    T* elements;
};
```

当然，我们也可以将`vector<T>::clear`的定义部分放在类型之外，只不过这个时候的语法就显得蹩脚许多：

```cpp
template <typename T>
class vector
{
public:
    void clear();  // 注意这里只有声明
private:
    T* elements;
};

template <typename T>
void vector<T>::clear()  // 函数的实现放在这里
{
    // Function body
}
```

函数的实现部分看起来略微拗口。我第一次学到的时候，觉得

```cpp
void vector::clear()
{
    // Function body
}
```

这样不就行了吗？但是简单想就会知道，`clear`里面是找不到泛型类型`T`的符号的。

因此，在成员函数实现的时候，必须要提供模板参数。此外，为什么类型名不是`vector`而是`vector<T>`呢？ 如果你了解过模板的偏特化与特化的语法，应该能看出，这里的vector在语法上类似于特化/偏特化。实际上，这里的函数定义也确实是成员函数的偏特化。特化和偏特化的概念，本文会在第二部分详细介绍。

综上，正确的成员函数实现如下所示：

```cpp
template <typename T> // 模板参数
void vector<T> /*看起来像偏特化*/ ::clear() // 函数的实现放在这里
{
    // Function body
}
```

### 1.2 Template Function的基本语法

#### 1.2.1 Template Function的声明和定义

模板函数的语法与模板类基本相同，也是以关键字`template`和模板参数列表作为声明与定义的开始。模板参数列表中的类型，可以出现在参数、返回值以及函数体中。比方说下面几个例子

```cpp
template <typename T> void foo(T const& v);

template <typename T> T foo();

template <typename T, typename U> U foo(T const&);

template <typename T> void foo()
{
    T var;
    // ...
}
```

无论是函数模板还是类模板，在实际代码中看起来都是“千变万化”的。这些“变化”，主要是因为类型被当做了参数，导致代码中可以变化的部分更多了。

归根结底，模板无外乎两点：

1. 函数或者类里面，有一些类型我们希望它能变化一下，我们用标识符来代替它，这就是“模板参数”；
2. 在需要这些类型的地方，写上相对应的标识符（“模板参数”）。

当然，这里的“可变”实际上在代码编译好后就固定下来了，可以称之为编译期的可变性。

这里多啰嗦一点，主要也是想告诉大家，模板其实是个很简单的东西。

下面这个例子，或许可以帮助大家解决以下两个问题：

1. 什么样的需求会使用模板来解决？
2. 怎样把脑海中的“泛型”变成真正“泛型”的代码？

```cpp
举个例子：generic typed function ‘add’
```

在我遇到的朋友中，即便如此对他解释了模板，即便他了解了模板，也仍然会对模板产生畏难情绪。毕竟从形式上来说，模板类和模板函数都要较非模板的版本更加复杂，阅读代码所需要理解的内容也有所增多。

如何才能克服这一问题，最终视模板如平坦代码呢？

答案只有一个：**无他，唯手熟尔**。

在学习模板的时候，要反复做以下的思考和练习：

1. 提出问题：我的需求能不能用模板来解决？
2. 怎么解决？
3. 把解决方案用代码写出来。
4. 如果失败了，找到原因。是知识有盲点（例如不知道怎么将 `T&` 转化成 `T`），还是不可行（比如试图利用浮点常量特化模板类，但实际上这样做是不可行的）？

#### 1.2.2 模板函数的使用

我们先来看一个简单的函数模板，两个数相加：

```cpp
template <typename T> T Add(T a, T b)
{
    return a + b;
}
```

函数模板的调用格式是：

```cpp
函数模板名 < 模板参数列表 > ( 参数 )
```

例如，我们想对两个 `int` 求和，那么套用类的模板实例化方法，我们可以这么写：

```cpp
int a = 5;
int b = 3;
int result = Add<int>(a, b);
```

这时我们等于拥有了一个新函数：

```cpp
int Add<int>(int a, int b) { return a + b; }
```

这时在另外一个偏远的程序角落，你也需要求和。而此时你的参数类型是 `float` ，于是你写下：

```cpp
Add<float>(a, b);
```

一切看起来都很完美。但如果你具备程序员的最佳美德——懒惰——的话，你肯定会这样想，我在调用 `Add<int>(a, b)` 的时候， `a` 和 `b` 匹配的都是那个 `T`。编译器就应该知道那个 `T` 实际上是 `int` 呀？为什么还要我多此一举写 `Add<int>` 呢？ 唔，我想说的是，编译器的作者也是这么想的。所以实际上你在编译器里面写下以下片段：

```cpp
int a = 5;
int b = 3;
int result = Add(a, b);
```

编译器会心领神会地将 `Add` 变成 `Add<int>`。但是编译器不能面对模棱两可的答案。比如你这么写的话呢？

```cpp
int  a = 5;
char b = 3;
int  result = Add(a, b);
```

第一个参数 `a` 告诉编译器，这个 `T` 是 `int`。编译器点点头说，好。但是第二个参数 `b` 不高兴了，告诉编译器说，你这个 `T`，其实是 `char`。 两个参数各自指导 `T` 的类型，编译器就不知道怎么做了。在Visual Studio 2012下，会有这样的提示：

```cpp
error C2782: 'T _1_2_2::Add(T,T)' : template parameter 'T' is ambiguous
```

好吧，"ambiguous"，这个提示再明确不过了。

不过，只要你别逼得编译器精神分裂的话，编译器其实是非常聪明的，它可以从很多的蛛丝马迹中，猜测到你真正的意图，有如下面的例子：

```cpp
template <typename T> class A {};

template <typename T> T foo( A<T> v );

A<int> v;
foo(v);	// 它能准确地猜到 T 是 int.
```

咦，编译器居然绕过了A这个外套，猜到了 `T` 匹配的是 `int`。编译器是怎么完成这一“魔法”的，我们暂且不表，2.2节时再和盘托出。

下面轮到你的练习时间了。你试着写了很多的例子，但是其中一个你还是犯了疑惑：

```cpp
float data[1024];

template <typename T> T GetValue(int i)
{
    return static_cast<T>(data[i]);
}

float a = GetValue(0);	// 出错了！
int b = GetValue(1);	// 也出错了！
```

为什么会出错呢？你仔细想了想，原来编译器是没办法去根据返回值推断类型的。函数调用的时候，返回值被谁接受还不知道呢。如下修改后，就一切正常了：

```cpp
float a = GetValue<float>(0);
int b = GetValue<int>(1);
```

嗯，是不是so easy啊？嗯，你又信心满满的做了一个练习：

你要写一个模板函数叫 `c_style_cast`，顾名思义，执行的是C风格的转换。然后出于方便起见，你希望它能和 `static_cast` 这样的内置转换有同样的写法。于是你写了一个use case。

```cpp
DstT dest = c_style_cast<DstT>(src);
```

根据调用形式你知道了，有 `DstT` 和 `SrcT` 两个模板参数。参数只有一个， `src`，所以函数的形参当然是这么写了： `(SrcT src)`。实现也很简单， `(DstT)v`。

我们把手上得到的信息来拼一拼，就可以编写自己的函数模板了：

```cpp
template <typename SrcT, typename DstT> DstT c_style_cast(SrcT v)
{
    return (DstT)(v);
}

int v = 0;
float i = c_style_cast<float>(v);
```

嗯，很Easy嘛！我们F6一下…咦！这是什么意思！

```cpp
error C2783: 'DstT _1_2_2::c_style_cast(SrcT)' : could not deduce template argument for 'DstT'
```

然后你仔细的比较了一下，然后发现 … 模板参数有两个，而参数里面能得到的只有 `SrcT` 一个。结合出错信息看来关键在那个 `DstT` 上。这个时候，你死马当活马医，把模板参数写完整了：

```cpp
float i = c_style_cast<int, float>(v);
```

嗯，很顺利的通过了。难道C++不能支持让参数推导一部分模板参数吗？

当然是可以的。只不过在部分推导、部分指定的情况下，编译器对模板参数的顺序是有限制的：**先写需要指定的模板参数，再把能推导出来的模板参数放在后面**。

在这个例子中，能推导出来的是 `SrcT`，需要指定的是 `DstT`。把函数模板写成下面这样就可以了：

```cpp
template <typename DstT, typename SrcT> DstT c_style_cast(SrcT v)	// 模板参数 DstT 需要人肉指定，放前面。
{
    return (DstT)(v);
}

int v = 0;
float i = c_style_cast<float>(v);  // 形象地说，DstT会先把你指定的参数吃掉，剩下的就交给编译器从函数参数列表中推导啦。
```



### 1.3 整型也可是Template参数

模板参数除了类型外（包括基本类型、结构、类类型等），也可以是一个整型数（Integral Number）。这里的整型数比较宽泛，包括布尔型，不同位数、有无符号的整型，甚至包括指针。我们将整型的模板参数和类型作为模板参数来做一个对比：

```cpp
template <typename T> class TemplateWithType;
template <int      V> class TemplateWithValue;
```

我想这个时候你也更能理解 `typename` 的意思了：它相当于是模板参数的“类型”，告诉你 `T` 是一个 `typename`。

按照C++ Template最初的想法，模板不就是为了提供一个类型安全、易于调试的宏吗？有类型就够了，为什么要引入整型参数呢？考虑宏，它除了代码替换，还有一个作用是作为常数出现。所以整型模板参数最基本的用途，也是定义一个常数。例如这段代码的作用：

```cpp
template <typename T, int Size> struct Array
{
    T data[Size];
};

Array<int, 16> arr;
```

便相当于下面这段代码：

```cpp
class IntArrayWithSize16
{
    int data[16]; // int 替换了 T, 16 替换了 Size
};

IntArrayWithSize16 arr;
```

其中有一点需要注意，因为模板的匹配是在编译的时候完成的，所以实例化模板的时候所使用的参数，也必须要在编译期就能确定。例如以下的例子编译器就会报错：

```cpp
template <int i> class A {};

void foo()
{
    int x = 3;
    A<5> a; // 正确！
    A<x> b; // error C2971: '_1_3::A' : template parameter 'i' : 'x' : a local variable cannot be used as a non-type argument
}
```

因为x不是一个编译期常量，所以 `A<x>` 就会告诉你，x是一个局部变量，不能作为一个模板参数出现。

嗯，这里我们再来写几个相对复杂的例子：

```cpp
template <int i> class A 
{
public:
    void foo(int)
    {
    }
};
template <uint8_t a, typename b, void* c> class B {};
template <bool, void (*a)()> class C {};
template <void (A<3>::*a)(int)> class D {};

template <int i> int Add(int a)	// 当然也能用于函数模板
{
    return a + i;
}

void foo()
{
    A<5> a;
    B<7, A<5>, nullptr>	b; // 模板参数可以是一个无符号八位整数，可以是模板生成的类；可以是一个指针。
    C<false, &foo> c;      // 模板参数可以是一个bool类型的常量，甚至可以是一个函数指针。
    D<&A<3>::foo> d;       // 丧心病狂啊！它还能是一个成员函数指针！
    int x = Add<3>(5);     // x == 8。因为整型模板参数无法从函数参数获得，所以只能是手工指定啦。
}

template <float a> class E {}; // ERROR: 别闹！早说过只能是整数类型的啦！
```

当然，除了单纯的用作常数之外，整型参数还有一些其它的用途。这些“其它”用途最重要的一点是让类型也可以像整数一样运算。《Modern C++ Design》给我们展示了很多这方面的例子。不过你不用急着去阅读那本天书，我们会在做好足够的知识铺垫后，让你轻松学会这些招数。

### 1.4 模板形式与功能是统一的

第一章走马观花的带着大家复习了一下C++ Template的基本语法形式，也解释了包括 `typename` 在内，类/函数模板写法中各个语法元素的含义。形式是功能的外在体现，介绍它们也是为了让大家能理解到，模板之所以写成这种形式是有必要的，而不是语言的垃圾成分。

从下一章开始，我们便进入了更加复杂和丰富的世界：讨论模板的匹配规则。其中有令人望而生畏的特化与偏特化。但是，请相信我们在序言中所提到的：将模板作为一门语言来看待，它会变得有趣而简单。





































































