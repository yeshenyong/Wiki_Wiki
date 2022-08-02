# cppTemplateTutorial

读书笔记自：https://github.com/wuye9036/CppTemplateTutorial#0-%e5%89%8d%e8%a8%80

> 把自己能做的事情做好

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



### 2.1 编程，元编程，模板元编程

程序最根本的目的是什么？复现真实世界或人所构想的规律，减少重复工作的成本，或通过提升规模完成人所不能及之事。但是世间之事万千，有限的程序如何重现复杂的世界呢？

答案是“抽象”。论及具体手段，无外乎“求同”与“存异”：概括一般规律，处理特殊情况。这也是软件工程所追求的目标。一般规律概括的越好，我们所付出的劳动也就越少。



元（meta）无论在中文还是英文里，都是个很“抽象（abstract）”的词。因为它的本意就是“抽象”。元编程，也可以说就是“编程的抽象”。用更好理解的说法，元编程意味着你撰写一段程序A，程序A会运行后生成另外一个程序B，程序B才是真正实现功能的程序。那么这个时候程序A可以称作程序B的元程序，撰写程序A的过程，就称之为“元编程”。



如果元编程中所有变化的量（或者说元编程的参数），都是类型，那么这样的编程，我们有个特定的称呼，叫“泛型”。



### 2.2 模板世界的If-Then-Else：类模板的特化与偏特化

#### 2.2.1 根据类型执行代码

前一节的示例提出了一个要求：需要做出根据类型执行不同代码。要达成这一目的，模板并不是唯一的途径。比如之前我们所说的重载。如果把眼界放宽一些，虚函数也是根据类型执行代码的例子。此外，在C语言时代，也会有一些技法来达到这个目的，比如下面这个例子，我们需要对两个浮点做加法， 或者对两个整数做乘法：

```c++
struct Variant
{
    union
    {
        int x;
        float y;
    } data;
    uint32 typeId;
};

Variant addFloatOrMulInt(Variant const* a, Variant const* b)
{
    Variant ret;
    assert(a->typeId == b->typeId);
    if (a->typeId == TYPE_INT)
    {
        ret.x = a->x * b->x;
    }
    else
    {
        ret.y = a->y + b->y;
    }
    return ret;
}
```

更常见的是 `void*`:

```c++
#define BIN_OP(type, a, op, b, result) (*(type *)(result)) = (*(type const *)(a)) op (*(type const*)(b))
void doDiv(void* out, void const* data0, void const* data1, DATA_TYPE type)
{
    if(type == TYPE_INT)
    {
        BIN_OP(int, data0, *, data1, out);
    }
    else
    {
        BIN_OP(float, data0, +, data1, out);
    }
}
```

在C++中比如在 `Boost.Any` 的实现中，运用了 `typeid` 来查询类型信息。和 `typeid` 同属于RTTI机制的 `dynamic_cast`，也经常会用来做类型判别的工作。我想你应该写过类似于下面的代码：

```c++
IAnimal* animal = GetAnimalFromSystem();

IDog* maybeDog = dynamic_cast<IDog*>(animal);
if(maybeDog)
{
    maybeDog->Wangwang();
}
ICat* maybeCat = dynamic_cast<ICat*>(animal);
if(maybeCat)
{
    maybeCat->Moemoe();
}
```

当然，在实际的工作中，我们建议把需要 `dynamic_cast` 后执行的代码，尽量变成虚函数。不过这个已经是另外一个问题了。我们看到，不管是哪种方法都很难避免 `if` 的存在。而且因为输入数据的类型是模糊的，经常需要强制地、没有任何检查的转换成某个类型，因此很容易出错。

但是模板与这些方法最大的区别并不在这里。模板无论其参数或者是类型，它都是一个编译期分派的办法。编译期就能确定的东西既可以做类型检查，编译器也能进行优化，砍掉任何不必要的代码执行路径。例如在上例中，

```c++
template <typename T> T addFloatOrMulInt(T a, T b);

// 迷之代码1：用于T是float的情况

// 迷之代码2：用于T是int时的情况
```



#### 2.2.2 特化

我的高中物理老师对我说过一句令我受用至今的话：把自己能做的事情做好。编写模板程序也是一样。当你试图用模板解决问题之前，先撇开那些复杂的语法要素，用最直观的方式表达你的需求：

```cpp
// 这里是伪代码，意思一下

int|float addFloatOrMulInt(a, b)
{
    if(type is Int)
    {
        return a * b;
    }
    else if (type is Float)
    {
        return a + b;
    }
}

void foo()
{
    float a, b, c;
    c = addFloatOrMulInt(a, b);		// c = a + b;
	
    int x, y, z;
    z = addFloatOrMulInt(x, y);		// z = x * y;
}
```

因为这一节是讲类模板有关的特化和偏特化机制，所以我们不用普通的函数，而是用类的静态成员函数来做这个事情（这就是典型的没事找抽型）：

```cpp
// 这里仍然是伪代码，意思一下，too。
class AddFloatOrMulInt
{
    static int|float Do(a, b)
    {
        if(type is Int)
        {
            return a * b;
        }
        else if (type is Float)
	{
	    return a + b;
        }
    }
};

void foo()
{
    float a, b, c;
    c = AddFloatOrMulInt::Do(a, b); // c = a + b;
	
    int x, y, z;
    z = AddFloatOrMulInt::Do(x, y); // z = x * y;
}
```

好，意思表达清楚了。我们先从调用方的角度，把这个形式改写一下：

```cpp
void foo()
{
    float a, b, c;
    c = AddFloatOrMulInt<float>::Do(a, b); // c = a + b;
	
    int x, y, z;
    z = AddFloatOrMulInt<int>::Do(x, y); // z = x * y;
}
```

也许你不明白为什么要改写成现在这个样子。看不懂不怪你，怪我讲得不好。但是你别急，先看看这样改写以后能不能跟我们的目标接近一点。如果我们把 `AddFloatOrMulInt<float>::Do` 看作一个普通的函数，那么我们可以写两个实现出来：

```cpp
float AddFloatOrMulInt<float>::Do(float a, float b)
{
    return a + b;
}

int AddFloatOrMulInt<int>::Do(int a, int b)
{
    return a * b;
}

void foo()
{
    float a, b, c;
    c = AddFloatOrMulInt<float>::Do(a, b); // c = a + b;
	
    int x, y, z;
    z = AddFloatOrMulInt<int>::Do(x, y); // z = x * y;
}
```

这样是不是就很开心了？我们更进一步，把 `AddFloatOrMulInt<int>::Do` 换成合法的类模板：

```cpp
// 这个是给float用的。
template <typename T> class AddFloatOrMulInt
{
    T Do(T a, T b)
    {
        return a + b;
    }
};

// 这个是给int用的。
template <typename T> class AddFloatOrMulInt
{
    T Do(T a, T b)
    {
        return a * b;
    }
};

void foo()
{
    float a, b, c;

    // 嗯，我们需要 c = a + b;
    c = AddFloatOrMulInt<float>::Do(a, b);
    // ... 觉得哪里不对劲 ...
    // ...
    // ...
    // ...
    // 啊！有两个AddFloatOrMulInt，class看起来一模一样，要怎么区分呢！
}
```

好吧，问题来了！如何要让两个内容不同，但是模板参数形式相同的类进行区分呢？特化！特化（specialization）是根据一个或多个特殊的整数或类型，给出模板实例化时的一个指定内容。我们先来看特化是怎么应用到这个问题上的。

```cpp
// 首先，要写出模板的一般形式（原型）
template <typename T> class AddFloatOrMulInt
{
    static T Do(T a, T b)
    {
        // 在这个例子里面一般形式里面是什么内容不重要，因为用不上
        // 这里就随便给个0吧。
        return T(0);
    }
};

// 其次，我们要指定T是int时候的代码，这就是特化：
template <> class AddFloatOrMulInt<int>
{
public:
    static int Do(int a, int b) // 
    {
        return a * b;
    }
};

// 再次，我们要指定T是float时候的代码：
template <> class AddFloatOrMulInt<float>
{
public:
    static float Do(float a, float b)
    {
        return a + b;
    }
};

void foo()
{
    // 这里面就不写了
}
```

我们再把特化的形式拿出来一瞧：这货有点怪啊： `template <> class AddFloatOrMulInt<int>`。别急，我给你解释一下。

```cpp
// 我们这个模板的基本形式是什么？
template <typename T> class AddFloatOrMulInt;

// 但是这个类，是给T是Int的时候用的，于是我们写作
class AddFloatOrMulInt<int>;
// 当然，这里编译是通不过的。

// 但是它又不是个普通类，而是类模板的一个特化（特例）。
// 所以前面要加模板关键字template，
// 以及模板参数列表
template </* 这里要填什么？ */> class AddFloatOrMulInt<int>;

// 最后，模板参数列表里面填什么？因为原型的T已经被int取代了。所以这里就不能也不需要放任何额外的参数了。
// 所以这里放空。
template <> class AddFloatOrMulInt<int>
{
    // ... 针对Int的实现 ... 
};

// Bingo!
```

哈，这样就好了。我们来做一个练习。我们有一些类型，然后你要用模板做一个对照表，让类型对应上一个数字。我先来做一个示范：

```cpp
template <typename T> class TypeToID
{
public:
    static int const ID = -1;
};

template <> class TypeToID<uint8_t>
{
public:
    static int const ID = 0;
};
```

然后呢，你的任务就是，要所有无符号的整数类型的特化（其实就是`uint8_t`到`uint64_t`啦），把所有的基本类型都赋予一个ID（当然是不一样的啦）。当你做完后呢，可以把类型所对应的ID打印出来，我仍然以 `uint8_t` 为例：

```cpp
void PrintID()
{
    cout << "ID of uint8_t: " << TypeToID<uint8_t>::ID << endl;
}
```

嗯，看起来挺简单的，是吧。但是这里透露出了一个非常重要的信号，我希望你已经能察觉出来了： `TypeToID` 如同是一个函数。这个函数只能在编译期间执行。它输入一个类型，输出一个ID。

如果你体味到了这一点，那么恭喜你，你的模板元编程已经开悟了。



#### 2.2.3 特化：一些其它问题

在上一节结束之后，你一定做了许多的练习。我们再来做三个练习。第一，给`float`一个ID；第二，给`void*`一个ID；第三，给任意类型的指针一个ID。先来做第一个:

```cpp
// ...
// TypeToID 的模板“原型”
// ...

template <> class TypeToID<float>
{
public:
    static int const ID = 0xF10A7;
};
```

嗯， 这个你已经了然于心了。那么`void*`呢？你想了想，这已经是一个复合类型了。不错你还是战战兢兢地写了下来：

```cpp
template <> class TypeToID<void*>
{
public:
    static int const ID = 0x401d;
};

void PrintID()
{
    cout << "ID of uint8_t: " << TypeToID<void*>::ID << endl;
}
```

遍译运行一下，对了。模板不过如此嘛。然后你觉得自己已经完全掌握了，并试图将所有C++类型都放到模板里面，开始了自我折磨的过程：

```cpp
class ClassB {};

template <> class TypeToID<void ()>;      // 函数的TypeID
template <> class TypeToID<int[3]>;       // 数组的TypeID
template <> class TypeToID<int (int[3])>; // 这是以数组为参数的函数的TypeID
template <> class TypeToID<int (ClassB::*[3])(void*, float[2])>; // 我也不知道这是什么了，自己看着办吧。
```

甚至连 `const` 和 `volatile` 都能装进去：

```cpp
template <> class TypeToID<int const * volatile * const volatile>;
```

此时就很明白了，只要 `<>` 内填进去的是一个C++能解析的合法类型，模板都能让你特化。不过这个时候如果你一点都没有写错的话， `PrintID` 中只打印了我们提供了特化的类型的ID。那如果我们没有为之提供特化的类型呢？比如说double？OK，实践出真知，我们来尝试着运行一下：

```cpp
void PrintID()
{
    cout << "ID of double: " << TypeToID<double>::ID << endl;
}
```

嗯，它输出的是-1。我们顺藤摸瓜会看到， `TypeToID`的类模板“原型”的ID是值就是-1。通过这个例子可以知道，当模板实例化时提供的模板参数不能匹配到任何的特化形式的时候，它就会去匹配类模板的“原型”形式。

不过这里有一个问题要理清一下。和继承不同，类模板的“原型”和它的特化类在实现上是没有关系的，并不是在类模板中写了 `ID` 这个Member，那所有的特化就必须要加入 `ID` 这个Member，或者特化就自动有了这个成员。完全没这回事。我们把类模板改成以下形式，或许能看的更清楚一点：

```cpp
template <typename T> class TypeToID
{
public:
    static int const NotID = -2;
};

template <> class TypeToID<float>
{
public:
    static int const ID = 1;
};

void PrintID()
{
    cout << "ID of float: " << TypeToID<float>::ID << endl;       // Print "1"
    cout << "NotID of float: " << TypeToID<float>::NotID << endl; // Error! TypeToID<float>使用的特化的类，这个类的实现没有NotID这个成员。
    cout << "ID of double: " << TypeToID<double>::ID << endl;     // Error! TypeToID<double>是由模板类实例化出来的，它只有NotID，没有ID这个成员。
}
```

这样就明白了。类模板和类模板的特化的作用，仅仅是指导编译器选择哪个编译，但是特化之间、特化和它原型的类模板之间，是分别独立实现的。所以如果多个特化、或者特化和对应的类模板有着类似的内容，很不好意思，你得写上若干遍了。

第三个问题，是写一个模板匹配任意类型的指针。对于C语言来说，因为没有泛型的概念，因此它提供了无类型的指针`void*`。它的优点是，所有指针都能转换成它。它的缺点是，一旦转换称它后，你就再也不知道这个指针到底是指向`float`或者是`int`或者是`struct`了。

比如说`copy`。

```cpp
void copy(void* dst, void const* src, size_t elemSize, size_t elemCount, void (*copyElem)(void* dstElem, void const* srcElem))
{
    void const* reader = src;
    void const* writer = dst;
    for(size_t i = 0; i < elemCount; ++i)
    {
        copyElem(writer, reader);
        advancePointer(reader, elemSize); // 把Reader指针往后移动一些字节
        advancePointer(writer, elemSize);
     }
}
```

为什么要提供copyElem，是因为可能有些struct需要深拷贝，所以得用特殊的copy函数。这个在C++98/03里面就体现为拷贝构造和赋值函数。

但是不管怎么搞，因为这个函数的参数只是`void*`而已，当你使用了错误的elemSize，或者传入了错误的copyElem，就必须要到运行的时候才有可能看出来。注意，这还只是有可能而已。

那么C++有了模板后，能否既能匹配任意类型的指针，同时又保留了类型信息呢？答案是显然的。至于怎么写，那就得充分发挥你的直觉了：

首先，我们需要一个`typename T`来指代“任意类型”这四个字：

```cpp
template <typename T>
```

接下来，我们要写函数原型：

```cpp
void copy(?? dest, ?? src, size_t elemCount);
```

这里的 `??` 要怎么写呢？既然我们有了模板类型参数T，那我们不如就按照经验，写 `T*` 看看。

```cpp
template <typename T>
void copy(T* dst, T const* src, size_t elemCount);
```

编译一下，咦，居然通过了。看来这里的语法与我们以前学到的知识并没有什么不同。这也是语言设计最重要的一点原则：一致性。它可以让你辛辛苦苦体验到的规律不至于白费。

最后就是实现：

```cpp
template <typename T>
void copy(T* dst, T const* src, size_t elemCount)
{
    for(size_t i = 0; i < elemCount; ++i)
    {
        dst[i] = src[i];
    }
}
```

是不是简洁了许多？你不需要再传入size；只要你有正确的赋值函数，也不需要提供定制的copy；也不用担心dst和src的类型不匹配了。

最后，我们把函数模板学到的东西，也应用到类模板里面：

```cpp
template <typename T> // 嗯，需要一个T
class TypeToID<T*> // 我要对所有的指针类型特化，所以这里就写T*
{
public:
    static int const ID = 0x80000000;	// 用最高位表示它是一个指针
};
```

最后写个例子来测试一下，看看我们的 `T*` 能不能搞定 `float*`：

```cpp
void PrintID()
{
    cout << "ID of float*: " << TypeToID<float*>::ID << endl;
}
```

哈哈，大功告成。嗯，别急着高兴。待我问一个问题：你知道 `TypeToID<float*>` 后，这里的T是什么吗？换句话说，你知道下面这段代码打印的是什么吗？

```cpp
// ...
// TypeToID 的其他代码，略过不表
// ...

template <typename T> // 嗯，需要一个T
class TypeToID<T*> // 我要对所有的指针类型特化，所以这里就写T*
{
public:
    typedef T		 SameAsT;
    static int const ID = 0x80000000; // 用最高位表示它是一个指针
};

void PrintID()
{
    cout << "ID of float*: " << TypeToID< TypeToID<float*>::SameAsT >::ID << endl;
}
```

别急着运行，你先猜。

------------------------- 这里是给勤于思考的码猴的分割线 -------------------------------

OK，猜出来了吗，T是`float`。为什么呢？因为你用 `float *` 匹配了 `T *`，所以 `T` 就对应 `float` 了。没想清楚的自己再多体会一下。

嗯，所以实际上，我们可以利用这个特性做一件事情：把指针类型的那个指针给“干掉”：

```cpp
template <typename T>
class RemovePointer
{
public:
    typedef T Result;  // 如果放进来的不是一个指针，那么它就是我们要的结果。
};

template <typename T>
class RemovePointer<T*>	// 祖传牛皮藓，专治各类指针
{
public:
    typedef T Result;  // 正如我们刚刚讲的，去掉一层指针，把 T* 这里的 T 取出来。
};

void Foo()
{
    RemovePointer<float*>::Result x = 5.0f; // 喏，用RemovePointer后，那个Result就是把float*的指针处理掉以后的结果：float啦。
    std::cout << x << std::endl;
}
```

当然啦，这里我们实现的不算是真正的 `RemovePointer`，因为我们只去掉了一层指针。而如果传进来的是类似 `RemovePointer<int**>` 这样的东西呢？是的没错，去掉一层之后还是一个指针。`RemovePointer<int**>::Result` 应该是一个 `int*`，要怎么才能实现我们想要的呢？聪明的你一定能想到：只要像剥洋葱一样，一层一层一层地剥开，不就好了吗！相应地我们应该怎么实现呢？可以把 `RemovePointer` 的特化版本改成这样（当然如果有一些不明白的地方你可以暂时跳过，接着往下看，很快就会明白的）：

```cpp
template <typename T>
class RemovePointer<T*>
{
public:
    // 如果是传进来的是一个指针，我们就剥夺一层，直到指针形式不存在为止。
    // 例如 RemovePointer<int**>，Result 是 RemovePointer<int*>::Result，
    // 而 RemovePointer<int*>::Result 又是 int，最终就变成了我们想要的 int，其它也是类似。
    typedef typename RemovePointer<T>::Result Result;
};
```

是的没错，这便是我们想要的 `RemovePointer` 的样子。类似的你还可以试着实现 `RemoveConst`, `AddPointer` 之类的东西。

OK，回到我们之前的话题，如果这个时候，我需要给 `int*` 提供一个更加特殊的特化，那么我还得多提供一个：

```cpp
// ...
// TypeToID 的其他代码，略过不表
// ...

template <typename T> // 嗯，需要一个T
class TypeToID<T*>    // 我要对所有的指针类型特化，所以这里就写T*
{
public:
    typedef T SameAsT;
    static int const ID = 0x80000000; // 用最高位表示它是一个指针
};

template <> // 嗯，int* 已经是个具体的不能再具体的类型了，所以模板不需要额外的类型参数了
class TypeToID<int*> // 嗯，对int*的特化。在这里呢，要把int*整体看作一个类型
{
public:
    static int const ID = 0x12345678; // 给一个缺心眼的ID
};

void PrintID()
{
    cout << "ID of int*: " << TypeToID<int*>::ID << endl;
}
```

嗯，这个时候它会输出0x12345678的十进制（大概？）。 可能会有较真的人说，`int*` 去匹配 `T` 或者 `T*`，也是合法的。就和你说22岁以上能结婚，那24岁当然也能结婚一样。 那为什么 `int*` 就会找 `int*`，`float *`因为没有合适的特化就去找 `T*`，更一般的就去找 `T` 呢？废话，有专门为你准备的东西你不用，非要自己找事？这就是直觉。 但是呢，直觉对付更加复杂的问题还是没用的（也不是没用，主要是你没这个直觉了）。我们要把这个直觉，转换成合理的规则——即模板的匹配规则。 当然，这个匹配规则是对复杂问题用的，所以我们会到实在一眼看不出来的时候才会动用它。一开始我们只要把握：**模板是从最特殊到最一般形式进行匹配的** 就可以了。

### 2.3 即用即推导

#### 2.3.1 视若无睹的语法错误





















































