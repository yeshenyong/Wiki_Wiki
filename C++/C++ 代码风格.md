# C++ 代码风格

\- 单一职责原则（Single ResponsibilityPrinciple）

\- 开闭原则 (Open Close Principle)

\- 里氏替换原则 (Liskov Substitution Principle)

\- 接口隔离原则 (InterfaceSegregation Principle)

\- 依赖反转原则 (Dependence InversionPrinciple)

### 头文件

#### Self-contained 头文件

> 头文件应该能够自给自足（self-contained,也就是可以作为第一个头文件被引入），以 `.h` 结尾。至于用来插入文本的文件，说到底它们并不是头文件，所以应以 `.inc` 结尾。不允许分离出 `-inl.h` 头文件的做法.

#### #define 保护

> 防止头文件重复包含

#### 前置声明

尽可能地避免使用前置声明。使用 `#include` 包含需要的头文件即可。

#### 内联函数

> 只有当函数只有 10 行甚至更少时才将其定义为内联函数.

**定义:**

> 当函数被声明为内联函数之后, 编译器会将其内联展开, 而不是按通常的函数调用机制进行调用.

**优点:**

> 只要内联的函数体较小, 内联该函数可以令目标代码更加高效. 对于存取函数以及其它函数体比较短, 性能关键的函数, 鼓励使用内联.

**缺点:**

> 滥用内联将导致程序变得更慢. 内联可能使目标代码量或增或减, 这取决于内联函数的大小. 内联非常短小的存取函数通常会减少代码大小, 但内联一个相当大的函数将戏剧性的增加代码大小. 现代处理器由于更好的利用了指令缓存, 小巧的代码往往执行更快。

另一个实用的经验准则: 内联那些包含循环或 `switch` 语句的函数常常是得不偿失 (除非在大多数情况下, 这些循环或 `switch` 语句从不被执行).

有些函数即使声明为内联的也不一定会被编译器内联, 这点很重要; 比如虚函数和递归函数就不会被正常内联. 通常, 递归函数不应该声明成内联函数.（YuleFox 注: 递归调用堆栈的展开并不像循环那么简单, 比如递归层数在编译时可能是未知的, 大多数编译器都不支持内联递归函数). 虚函数内联的主要原因则是想把它的函数体放在类定义内, 为了图个方便, 抑或是当作文档描述其行为, 比如精短的存取函数.

####  `#include` 的路径及顺序

> 使用标准的头文件包含顺序可增强可读性, 避免隐藏依赖: 相关头文件, C 库, C++ 库, 其他库的 .h, 本项目内的 .h.

```c++
#include "base/logging.h"
```



### 作用域

#### 命名空间

> 鼓励在 `.cc` 文件内使用匿名命名空间或 `static` 声明. 使用具名的命名空间时, 其名称可基于项目名或相对路径. 禁止使用 using 指示（using-directive）。禁止使用内联命名空间（inline namespace）。

**定义:**

> 命名空间将全局作用域细分为独立的, 具名的作用域, 可有效防止全局作用域的命名冲突.

**优点:**

> 虽然类已经提供了（可嵌套的）命名轴线 (YuleFox 注: 将命名分割在不同类的作用域内), 命名空间在这基础上又封装了一层.
>
> 举例来说, 两个不同项目的全局作用域都有一个类 `Foo`, 这样在编译或运行时造成冲突. 如果每个项目将代码置于不同命名空间中, `project1::Foo` 和 `project2::Foo` 作为不同符号自然不会冲突.
>
> 内联命名空间会自动把内部的标识符放到外层作用域，比如：
>
> ```c++
> namespace X {
> inline namespace Y {
> void foo();
> }  // namespace Y
> }  // namespace X
> ```

**缺点:**

> 命名空间具有迷惑性, 因为它们使得区分两个相同命名所指代的定义更加困难。
>
> 内联命名空间很容易令人迷惑，毕竟其内部的成员不再受其声明所在命名空间的限制。内联命名空间只在大型版本控制里有用。
>
> 有时候不得不多次引用某个定义在许多嵌套命名空间里的实体，使用完整的命名空间会导致代码的冗长。
>
> 在头文件中使用匿名空间导致违背 C++ 的唯一定义原则 (One Definition Rule (ODR)).

> 根据下文将要提到的策略合理使用命名空间.
>
> - 遵守 [命名空间命名](https://google-styleguide.readthedocs.io/zh_CN/latest/google-cpp-styleguide/naming.html#namespace-names) 中的规则。
>
> - 像之前的几个例子中一样，在命名空间的最后注释出命名空间的名字。
>
> - 用命名空间把文件包含, [gflags](https://gflags.github.io/gflags/) 的声明/定义, 以及类的前置声明以外的整个源文件封装起来, 以区别于其它命名空间:
>
>   > ```c++
>   > // .h 文件
>   > namespace mynamespace {
>   > 
>   > // 所有声明都置于命名空间中
>   > // 注意不要使用缩进
>   > class MyClass {
>   >     public:
>   >     ...
>   >     void Foo();
>   > };
>   > 
>   > } // namespace mynamespace
>   > ```
>   >
>   > ```c++
>   > // .cc 文件
>   > namespace mynamespace {
>   > 
>   > // 函数定义都置于命名空间中
>   > void MyClass::Foo() {
>   >     ...
>   > }
>   > 
>   > } // namespace mynamespace
>   > ```
>   >
>   > 更复杂的 `.cc` 文件包含更多, 更复杂的细节, 比如 gflags 或 using 声明。
>   >
>   > ```c++
>   > #include "a.h"
>   > 
>   > DEFINE_FLAG(bool, someflag, false, "dummy flag");
>   > 
>   > namespace a {
>   > 
>   > ...code for a...                // 左对齐
>   > 
>   > } // namespace a
>   > ```
>
> - 不要在命名空间 `std` 内声明任何东西, 包括标准库的类前置声明. 在 `std` 命名空间声明实体是未定义的行为, 会导致如不可移植. 声明标准库下的实体, 需要包含对应的头文件.
>
> - 不应该使用 *using 指示* 引入整个命名空间的标识符号。
>
>   > ```c++
>   > // 禁止 —— 污染命名空间
>   > using namespace foo;
>   > ```
>
> - 不要在头文件中使用 *命名空间别名* 除非显式标记内部命名空间使用。因为任何在头文件中引入的命名空间都会成为公开API的一部分。
>
>   > ```c++
>   > // 在 .cc 中使用别名缩短常用的命名空间
>   > namespace baz = ::foo::bar::baz;
>   > ```
>   >
>   > ```c++
>   > // 在 .h 中使用别名缩短常用的命名空间
>   > namespace librarian {
>   > namespace impl {  // 仅限内部使用
>   > namespace sidetable = ::pipeline_diagnostics::sidetable;
>   > }  // namespace impl
>   > 
>   > inline void my_inline_function() {
>   >   // 限制在一个函数中的命名空间别名
>   >   namespace baz = ::foo::bar::baz;
>   >   ...
>   > }
>   > }  // namespace librarian
>   > ```
>
> - 禁止用内联命名空间



#### 匿名命名空间和静态变量

在 `.cc` 文件中定义一个不需要被外部引用的变量时，可以将它们放在匿名命名空间或声明为 `static` 。但是不要在 `.h` 文件中这么做。

**定义:**

> 所有置于匿名命名空间的声明都具有内部链接性，函数和变量可以经由声明为 `static` 拥有内部链接性，这意味着你在这个文件中声明的这些标识符都不能在另一个文件中被访问。即使两个文件声明了完全一样名字的标识符，它们所指向的实体实际上是完全不同的。

**结论:**

> 推荐、鼓励在 `.cc` 中对于不需要在其他地方引用的标识符使用内部链接性声明，但是不要在 `.h` 中使用。
>
> 匿名命名空间的声明和具名的格式相同，在最后注释上 `namespace` :
>
> ```c++
> namespace {
> ...
> }  // namespace
> ```

**结论:**

> 有时, 把函数的定义同类的实例脱钩是有益的, 甚至是必要的. 这样的函数可以被定义成静态成员, 或是非成员函数. 非成员函数不应依赖于外部变量, 应尽量置于某个命名空间内. 相比单纯为了封装若干不共享任何静态数据的静态成员函数而创建类, 不如使用 [2.1. 命名空间](https://google-styleguide.readthedocs.io/zh_CN/latest/google-cpp-styleguide/scoping.html#namespaces) 。举例而言，对于头文件 `myproject/foo_bar.h` , 应当使用
>
> ```c++
> namespace myproject {
> namespace foo_bar {
> void Function1();
> void Function2();
> }  // namespace foo_bar
> }  // namespace myproject
> ```
>
> 而非
>
> ```c++
> namespace myproject {
> class FooBar {
>  public:
>   static void Function1();
>   static void Function2();
> };
> }  // namespace myproject
> ```
>
> 定义在同一编译单元的函数, 被其他编译单元直接调用可能会引入不必要的耦合和链接时依赖; 静态成员函数对此尤其敏感. 可以考虑提取到新类中, 或者将函数置于独立库的命名空间内.
>
> 如果你必须定义非成员函数, 又只是在 `.cc` 文件中使用它, 可使用匿名 [2.1. 命名空间](https://google-styleguide.readthedocs.io/zh_CN/latest/google-cpp-styleguide/scoping.html#namespaces) 或 `static` 链接关键字 (如 `static int Foo() {...}`) 限定其作用域.



#### 局部变量

将函数变量尽可能置于最小作用域内, 并在变量声明时进行初始化.

C++ 允许在函数的任何位置声明变量. 我们提倡在尽可能小的作用域中声明变量, 离第一次使用越近越好. 这使得代码浏览者更容易定位变量声明的位置, 了解变量的类型和初始值. 特别是，应使用初始化的方式替代声明再赋值, 比如:

> ```c++
> int i;
> i = f(); // 坏——初始化和声明分离
> int j = g(); // 好——初始化时声明
> vector<int> v;
> v.push_back(1); // 用花括号初始化更好
> v.push_back(2);
> vector<int> v = {1, 2}; // 好——v 一开始就初始化
> ```

属于 `if`, `while` 和 `for` 语句的变量应当在这些语句中正常地声明，这样子这些变量的作用域就被限制在这些语句中了，举例而言:

> ```c++
> while (const char* p = strchr(str, '/')) str = p + 1;
> ```

警告

有一个例外, 如果变量是一个对象, 每次进入作用域都要调用其构造函数, 每次退出作用域都要调用其析构函数. 这会导致效率降低.

```c++
// 低效的实现
for (int i = 0; i < 1000000; ++i) {
    Foo f;                  // 构造函数和析构函数分别调用 1000000 次!
    f.DoSomething(i);
}
```

在循环作用域外面声明这类变量要高效的多:

```c++
Foo f;                      // 构造函数和析构函数只调用 1 次
for (int i = 0; i < 1000000; ++i) {
    f.DoSomething(i);
}
```

#### 静态和全局变量

> 禁止定义静态储存周期非POD变量，禁止使用含有副作用的函数初始化POD全局变量，因为多编译单元中的静态变量执行时的构造和析构顺序是未明确的，这将导致代码的不可移植。

禁止使用类的 [静态储存周期](http://zh.cppreference.com/w/cpp/language/storage_duration#.E5.AD.98.E5.82.A8.E6.9C.9F) 变量：由于构造和析构函数调用顺序的不确定性，它们会导致难以发现的 bug 。不过 `constexpr` 变量除外，毕竟它们又不涉及动态初始化或析构。

静态生存周期的对象，即包括了全局变量，静态变量，静态类成员变量和函数静态变量，都必须是原生数据类型 (POD : Plain Old Data): 即 int, char 和 float, 以及 POD 类型的指针、数组和结构体。

静态变量的构造函数、析构函数和初始化的顺序在 C++ 中是只有部分明确的，甚至随着构建变化而变化，导致难以发现的 bug. 所以除了禁用类类型的全局变量，我们也不允许用函数返回值来初始化 POD 变量，除非该函数（比如 `getenv()` 或 `getpid()` ）不涉及任何全局变量。函数作用域里的静态变量除外，毕竟它的初始化顺序是有明确定义的，而且只会在指令执行到它的声明那里才会发生。

> 注解
>
> Xris 译注:
>
> 同一个编译单元内是明确的，静态初始化优先于动态初始化，初始化顺序按照声明顺序进行，销毁则逆序。不同的编译单元之间初始化和销毁顺序属于未明确行为 (unspecified behaviour)。

同理，全局和静态变量在程序中断时会被析构，无论所谓中断是从 `main()` 返回还是对 `exit()` 的调用。析构顺序正好与构造函数调用的顺序相反。但既然构造顺序未定义，那么析构顺序当然也就不定了。比如，在程序结束时某静态变量已经被析构了，但代码还在跑——比如其它线程——并试图访问它且失败；再比如，一个静态 string 变量也许会在一个引用了前者的其它变量析构之前被析构掉。

改善以上析构问题的办法之一是用 `quick_exit()` 来代替 `exit()` 并中断程序。它们的不同之处是前者不会执行任何析构，也不会执行 `atexit()` 所绑定的任何 handlers. 如果您想在执行 `quick_exit()` 来中断时执行某 handler（比如刷新 log），您可以把它绑定到 `_at_quick_exit()`. 如果您想在 `exit()` 和 `quick_exit()` 都用上该 handler, 都绑定上去。

综上所述，我们只允许 POD 类型的静态变量，即完全禁用 `vector` (使用 C 数组替代) 和 `string` (使用 `const char []`)。

如果您确实需要一个 `class` 类型的静态或全局变量，可以考虑在 `main()` 函数或 `pthread_once()` 内初始化一个指针且永不回收。注意只能用 raw 指针，别用智能指针，毕竟后者的析构函数涉及到上文指出的不定顺序问题。



### 类

#### 构造函数的职责

**总述**

不要在构造函数中调用虚函数, 也不要在无法报出错误时进行可能失败的初始化.

**定义**

在构造函数中可以进行各种初始化操作.

**优点**

- 无需考虑类是否被初始化.
- 经过构造函数完全初始化后的对象可以为 `const` 类型, 也能更方便地被标准容器或算法使用.

**缺点**

- 如果在构造函数内调用了自身的虚函数, 这类调用是不会重定向到子类的虚函数实现. 即使当前没有子类化实现, 将来仍是隐患.
- 在没有使程序崩溃 (因为并不是一个始终合适的方法) 或者使用异常 (因为已经被 [禁用](https://google-styleguide.readthedocs.io/zh_CN/latest/google-cpp-styleguide/others.html#exceptions) 了) 等方法的条件下, 构造函数很难上报错误
- 如果执行失败, 会得到一个初始化失败的对象, 这个对象有可能进入不正常的状态, 必须使用 `bool IsValid()` 或类似这样的机制才能检查出来, 然而这是一个十分容易被疏忽的方法.
- 构造函数的地址是无法被取得的, 因此, 举例来说, 由构造函数完成的工作是无法以简单的方式交给其他线程的.

**结论**

构造函数不允许调用虚函数. 如果代码允许, 直接终止程序是一个合适的处理错误的方式. 否则, 考虑用 `Init()` 方法或工厂函数.

**构造函数不得调用虚函数, 或尝试报告一个非致命错误**. 如果对象需要进行有意义的 (non-trivial) 初始化, 考虑使用明确的 Init() 方法或使用工厂模式. Avoid `Init()` methods on objects with no other states that affect which public methods may be called (此类形式的半构造对象有时无法正确工作).



#### 可拷贝类型和可移动类型

如果定义了拷贝/移动操作, 则要保证这些操作的默认实现是正确的. 记得时刻检查默认操作的正确性, 并且在文档中说明类是可拷贝的且/或可移动的.

```c++
class Foo {
 public:
  Foo(Foo&& other) : field_(other.field) {}
  // 差, 只定义了移动构造函数, 而没有定义对应的赋值运算符.

 private:
  Field field_;
};
```

由于存在对象切割的风险, 不要为任何有可能有派生类的对象提供赋值操作或者拷贝 / 移动构造函数 (当然也不要继承有这样的成员函数的类). 如果你的基类需要可复制属性, 请提供一个 `public virtual Clone()` 和一个 `protected` 的拷贝构造函数以供派生类实现.

如果你的类不需要拷贝 / 移动操作, 请显式地通过在 `public` 域中使用 `= delete` 或其他手段禁用之.

```c++
// MyClass is neither copyable nor movable.
MyClass(const MyClass&) = delete;
MyClass& operator=(const MyClass&) = delete;
```



#### 结构体 vs 类

**总述**

仅当只有数据成员时使用 `struct`, 其它一概使用 `class`.

**说明**

在 C++ 中 `struct` 和 `class` 关键字几乎含义一样. 我们为这两个关键字添加我们自己的语义理解, 以便为定义的数据类型选择合适的关键字.

`struct` 用来定义包含数据的被动式对象, 也可以包含相关的常量, 但除了存取数据成员之外, 没有别的函数功能. 并且存取功能是通过直接访问位域, 而非函数调用. 除了构造函数, 析构函数, `Initialize()`, `Reset()`, `Validate()` 等类似的用于设定数据成员的函数外, 不能提供其它功能的函数.

如果需要更多的函数功能, `class` 更适合. 如果拿不准, 就用 `class`.

为了和 STL 保持一致, 对于仿函数等特性可以不用 `class` 而是使用 `struct`.

注意: 类和结构体的成员变量使用不同的 [命名规则](https://google-styleguide.readthedocs.io/zh_CN/latest/google-cpp-styleguide/naming.html#variable-names).



#### 存取控制

**总述**

将 *所有* 数据成员声明为 `private`, 除非是 `static const` 类型成员 (遵循 [常量命名规则](https://google-styleguide.readthedocs.io/zh_CN/latest/google-cpp-styleguide/naming.html#constant-names)). 出于技术上的原因, 在使用 [Google Test](https://github.com/google/googletest) 时我们允许测试固件类中的数据成员为 `protected`.



#### 声明顺序

**总述**

将相似的声明放在一起, 将 `public` 部分放在最前.

**说明**

类定义一般应以 `public:` 开始, 后跟 `protected:`, 最后是 `private:`. 省略空部分.

在各个部分中, 建议将类似的声明放在一起, 并且建议以如下的顺序: 类型 (包括 `typedef`, `using` 和嵌套的结构体与类), 常量, 工厂函数, 构造函数, 赋值运算符, 析构函数, 其它函数, 数据成员.

不要将大段的函数定义内联在类定义中. 通常，只有那些普通的, 或性能关键且短小的函数可以内联在类定义中. 参见 [内联函数](https://google-styleguide.readthedocs.io/zh_CN/latest/google-cpp-styleguide/headers.html#inline-functions) 一节.



#### 继承

所有继承必须是 `public` 的. 如果你想使用私有继承, 你应该替换成把基类的实例作为成员对象的方式.

不要过度使用实现继承. 组合常常更合适一些. 尽量做到只在 “是一个” (“is-a”, YuleFox 注: 其他 “has-a” 情况下请使用组合) 的情况下使用继承: 如果 `Bar` 的确 “是一种” `Foo`, `Bar` 才能继承 `Foo`.

必要的话, 析构函数声明为 `virtual`. 如果你的类有虚函数, 则析构函数也应该为虚函数.

对于可能被子类访问的成员函数, 不要过度使用 `protected` 关键字. 注意, 数据成员都必须是 [私有的](https://google-styleguide.readthedocs.io/zh_CN/latest/google-cpp-styleguide/classes.html#access-control).

对于重载的虚函数或虚析构函数, 使用 `override`, 或 (较不常用的) `final` 关键字显式地进行标记. 较早 (早于 C++11) 的代码可能会使用 `virtual` 关键字作为不得已的选项. 因此, 在声明重载时, 请使用 `override`, `final` 或 `virtual` 的其中之一进行标记. 标记为 `override` 或 `final` 的析构函数如果不是对基类虚函数的重载的话, 编译会报错, 这有助于捕获常见的错误. 这些标记起到了文档的作用, 因为如果省略这些关键字, 代码阅读者不得不检查所有父类, 以判断该函数是否是虚函数.



#### 多重继承

只有当所有父类除第一个外都是 [纯接口类](https://google-styleguide.readthedocs.io/zh_CN/latest/google-cpp-styleguide/classes.html#interface) 时, 才允许使用多重继承. 为确保它们是纯接口, 这些类必须以 `Interface` 为后缀.



#### 笔记

1. 不在构造函数中做太多逻辑相关的初始化;
2. 编译器提供的默认构造函数不会对变量进行初始化, 如果定义了其他构造函数, 编译器不再提供, 需要编码者自行提供默认构造函数;
3. 为避免隐式转换, 需将单参数构造函数声明为 `explicit`;
4. 为避免拷贝构造函数, 赋值操作的滥用和编译器自动生成, 可将其声明为 `private` 且无需实现;
5. 仅在作为数据集合时使用 `struct`;
6. 组合 > 实现继承 > 接口继承 > 私有继承, 子类重载的虚函数也要声明 `virtual` 关键字, 虽然编译器允许不这样做;
7. 避免使用多重继承, 使用时, 除一个基类含有实现外, 其他基类均为纯接口;
8. 接口类类名以 `Interface` 为后缀, 除提供带实现的虚析构函数, 静态成员函数外, 其他均为纯虚函数, 不定义非静态数据成员, 不提供构造函数, 提供的话, 声明为 `protected`;
9. 为降低复杂性, 尽量不重载操作符, 模板, 标准类中使用时提供文档说明;
10. 存取函数一般内联在头文件中;
11. 声明次序: `public` -> `protected` -> `private`;
12. 函数体尽量短小, 紧凑, 功能单一;



### 函数

#### 编写简短函数

**总述**

我们倾向于编写简短, 凝练的函数.

**说明**

我们承认长函数有时是合理的, 因此并不硬性限制函数的长度. 如果函数超过 40 行, 可以思索一下能不能在不影响程序结构的前提下对其进行分割.

即使一个长函数现在工作的非常好, 一旦有人对其修改, 有可能出现新的问题, 甚至导致难以发现的 bug. 使函数尽量简短, 以便于他人阅读和修改代码.

在处理代码时, 你可能会发现复杂的长函数. 不要害怕修改现有代码: 如果证实这些代码使用 / 调试起来很困难, 或者你只需要使用其中的一小段代码, 考虑将其分割为更加简短并易于管理的若干函数.



#### 引用参数

**总述**

所有按引用传递的参数必须加上 `const`.

**定义**

在 C 语言中, 如果函数需要修改变量的值, 参数必须为指针, 如 `int foo(int *pval)`. 在 C++ 中, 函数还可以声明为引用参数: `int foo(int &val)`.

**优点**

定义引用参数可以防止出现 `(*pval)++` 这样丑陋的代码. 引用参数对于拷贝构造函数这样的应用也是必需的. 同时也更明确地不接受空指针.

**缺点**

容易引起误解, 因为引用在语法上是值变量却拥有指针的语义.

**结论**

函数参数列表中, 所有引用参数都必须是 `const`:

```
void Foo(const string &in, string *out);
```

事实上这在 Google Code 是一个硬性约定: 输入参数是值参或 `const` 引用, 输出参数为指针. 输入参数可以是 `const` 指针, 但决不能是非 `const` 的引用参数, 除非特殊要求, 比如 `swap()`.

有时候, 在输入形参中用 `const T*` 指针比 `const T&` 更明智. 比如:

- 可能会传递空指针.
- 函数要把指针或对地址的引用赋值给输入形参.

总而言之, 大多时候输入形参往往是 `const T&`. 若用 `const T*` 则说明输入另有处理. 所以若要使用 `const T*`, 则应给出相应的理由, 否则会使得读者感到迷惑.



#### 函数重载

**总述**

若要使用函数重载, 则必须能让读者一看调用点就胸有成竹, 而不用花心思猜测调用的重载函数到底是哪一种. 这一规则也适用于构造函数.

**定义**

你可以编写一个参数类型为 `const string&` 的函数, 然后用另一个参数类型为 `const char*` 的函数对其进行重载:

```c++
class MyClass {
    public:
    void Analyze(const string &text);
    void Analyze(const char *text, size_t textlen);
};
```

**优点**

通过重载参数不同的同名函数, 可以令代码更加直观. 模板化代码需要重载, 这同时也能为使用者带来便利.

**缺点**

如果函数单靠不同的参数类型而重载 (acgtyrant 注：这意味着参数数量不变), 读者就得十分熟悉 C++ 五花八门的匹配规则, 以了解匹配过程具体到底如何. 另外, 如果派生类只重载了某个函数的部分变体, 继承语义就容易令人困惑.

**结论**

如果打算重载一个函数, 可以试试改在函数名里加上参数信息. 例如, 用 `AppendString()` 和 `AppendInt()` 等, 而不是一口气重载多个 `Append()`. 如果重载函数的目的是为了支持不同数量的同一类型参数, 则优先考虑使用 `std::vector` 以便使用者可以用 [列表初始化](https://google-styleguide.readthedocs.io/zh_CN/latest/google-cpp-styleguide/others.html#braced-initializer-list) 指定参数.



#### 缺省函数

**结论**

对于虚函数, 不允许使用缺省参数, 因为在虚函数中缺省参数不一定能正常工作. 如果在每个调用点缺省参数的值都有可能不同, 在这种情况下缺省函数也不允许使用. (例如, 不要写像 `void f(int n = counter++);` 这样的代码.)



#### 函数返回类型后置语法

**总述**

只有在常规写法 (返回类型前置) 不便于书写或不便于阅读时使用返回类型后置语法.

**定义**

C++ 现在允许两种不同的函数声明方式. 以往的写法是将返回类型置于函数名之前. 例如:

```c++
int foo(int x);
```

C++11 引入了这一新的形式. 现在可以在函数名前使用 `auto` 关键字, 在参数列表之后后置返回类型. 例如:

```c++
auto foo(int x) -> int;
```

后置返回类型为函数作用域. 对于像 `int` 这样简单的类型, 两种写法没有区别. 但对于复杂的情况, 例如类域中的类型声明或者以函数参数的形式书写的类型, 写法的不同会造成区别.

**优点**

后置返回类型是显式地指定 [Lambda 表达式](https://google-styleguide.readthedocs.io/zh_CN/latest/google-cpp-styleguide/others.html#lambda-expressions) 的返回值的唯一方式. 某些情况下, 编译器可以自动推导出 Lambda 表达式的返回类型, 但并不是在所有的情况下都能实现. 即使编译器能够自动推导, 显式地指定返回类型也能让读者更明了.

有时在已经出现了的函数参数列表之后指定返回类型, 能够让书写更简单, 也更易读, 尤其是在返回类型依赖于模板参数时. 例如:

```c++
template <class T, class U> auto add(T t, U u) -> decltype(t + u);
```

对比下面的例子:

```c++
template <class T, class U> decltype(declval<T&>() + declval<U&>()) add(T t, U u);
```



### 来自Google 的奇技

#### 所有权与智能指针

**> 结论**

如果必须使用动态分配, 那么更倾向于将所有权保持在分配者手中. 如果其他地方要使用这个对象, 最好传递它的拷贝, 或者传递一个不用改变所有权的指针或引用. 倾向于使用 `std::unique_ptr` 来明确所有权传递, 例如：

```
std::unique_ptr<Foo> FooFactory();
void FooConsumer(std::unique_ptr<Foo> ptr);
```

如果没有很好的理由, 则不要使用共享所有权. 这里的理由可以是为了避免开销昂贵的拷贝操作, 但是只有当性能提升非常明显, 并且操作的对象是不可变的（比如说 `std::shared_ptr<const Foo>` ）时候, 才能这么做. 如果确实要使用共享所有权, 建议于使用 `std::shared_ptr` .

不要使用 `std::auto_ptr`, 使用 `std::unique_ptr` 代替它.



#### Cpplint

**总述**

使用 `cpplint.py` 检查风格错误.

**> 说明**

`cpplint.py` 是一个用来分析源文件, 能检查出多种风格错误的工具. 它不并完美, 甚至还会漏报和误报, 但它仍然是一个非常有用的工具. 在行尾加 `// NOLINT`, 或在上一行加 `// NOLINTNEXTLINE`, 可以忽略报错.

某些项目会指导你如何使用他们的项目工具运行 `cpplint.py`. 如果你参与的项目没有提供, 你可以单独下载 [cpplint.py](http://github.com/google/styleguide/blob/gh-pages/cpplint/cpplint.py).

### 其他C++ 特性

#### 引用参数





### 命名规范

**目录名：**

小写、下划线

```sh
server_login_plugin/plugin
```

**文件名、类名：**

驼峰。大小写字母，下划线构成

方便与类实现对应起来

```sh
QueryMyProcesser.cpp
```



**namespace**

小写、下划线

命名空间不占缩进。一个工程一个，尽量一层，别超过2层。

```c++
namespace class{
namespace student{

}
}
```



**类方法**

驼峰、小写字母开头

```c++
class MyProccesser {
	bool processCalculate();
};
```



**全局变量**

k+驼峰

```c++
const std::string kMyDataBaseName =  "123456"；
```



**全局变量**

g+驼峰

```c++
TreeNode gTree;
```



**宏**

大写加下划线



**const 接口修饰**

> 没有改写对象数据的接口，一律加上const。



**类成员变量**

小写字母开头，驼峰，下划线开头或结尾

```c
class QueryParseProcessor {
	int queryNum_;
};
```



### 日志规范

1. `error` 需要处理、`warning` 需要监控，不允许error 和 warning刷屏
2. 所有的 `info` 日志，都需要有 `key` 可供查询，无意义刷屏日志算 `bug`。统计部分使用counter实现，正常多次出现，需要打印case 的日志。



