# Style

一致性是风格最重要的方面。第二个最重要的方面是遵循一般C++程序员习惯阅读的风格

C++允许任意长度的标识符名称，因此在命名时没有理由保持简洁。使用描述性名称，并在样式上保持一致

- `CamelCase`
- `snake_case`

很常见的例子。*snake_case* 的优点是，如果需要，它还可以与拼写检查器一起使用

## Establishing A Style Guideline（建立风格准则）

无论您建立什么样的样式指导原则，一定要实现指定您期望的样式的`.clang-format`文件。虽然这对命名没有帮助，但是对于开源项目来说，保持一致的风格是特别重要的

每个IDE和许多编辑器都内置了`clang-format`的支持，或者可以通过加载项轻松安装

- VSCode: [Microsoft C/C++ extension for VS Code](https://github.com/Microsoft/vscode-cpptools)
- CLion: https://www.jetbrains.com/help/clion/clangformat-as-alternative-formatter.html
- VisualStudio https://marketplace.visualstudio.com/items?itemName=LLVMExtensions.ClangFormat#review-details
- Resharper++: https://www.jetbrains.com/help/resharper/2017.2/Using_Clang_Format.html
- Vim
  - https://github.com/rhysd/vim-clang-format
  - https://github.com/chiel92/vim-autoformat
- XCode: https://github.com/travisjeffery/ClangFormat-Xcode

## Common C++ Naming Conventions（通用C++命名约定）

- 类型以大写字母开头: `MyClass`.
- 函数和变量以小写开头: `myMethod`.
- 常量都是大写的: `const double PI=3.14159265358979323;`

C++标准库(以及其他著名的C++库，如`Boost`)使用以下指导原则:

- 宏名称使用大写和下划线: `INT_MAX`.
- 模板参数名称使用驼色大小写: `InputIterator`.
- 其他所有名称都使用 snake case: `unordered_map`.

tips：详细可以参考 `Google C++ style`



## Distinguish Private Object Data（区分私有对象数据）

将私有数据命名为 `m_` 后缀，以区别于公共数据。 `m_` 代表“成员”数据。

## Distinguish Function Parameters（区分函数参数）

最重要的是代码库的一致性;这是一种有助于保持一致性的可能性

命名函数参数 `t_`  ，`t_` can be thought of as "the"，但其含义是任意的。这样做的目的是将函数参数与作用域内的其他变量区别开来，同时提供一致的命名策略

可以为您的组织选择任何前缀或后缀。这只是一个例子。这个建议是有争议的，对此问题进行讨论

```cpp
struct Size
{
  int width;
  int height;

  Size(int t_width, int t_height) : width(t_width), height(t_height) {}
};

// 这个版本可能对线程安全或其他方面有意义
// 但更重要的是，有时我们需要隐藏数据，有时我们不需要
class PrivateSize
{
  public:
    int width() const { return m_width; }
    int height() const { return m_height; }
    PrivateSize(int t_width, int t_height) : m_width(t_width), m_height(t_height) {}

  private:
    int m_width;
    int m_height;
};
```

## Don't Name Anything Starting With `_`（不要以 `_` 开头命名任何东西）

如果这样做，就有可能与编译器和标准库实现使用保留的名称发生冲突:

http://stackoverflow.com/questions/228783/what-are-the-rules-about-using-an-underscore-in-a-c-identifier

## Well-Formed Example（格式良好的例子）

```cpp
class MyClass
{
public:
  MyClass(int t_data)
    : m_data(t_data)
  {
  }

  int getData() const
  {
    return m_data;
  }

private:
  int m_data;
};
```

## Enable Out-of-Source-Directory Builds（使用源目录外构建）

确保生成的文件放入与源文件夹分开的输出文件夹中

## Use `nullptr`

C++ 11引入了`nullptr`，这是一个表示空指针的特殊值。这应该用来代替0或NULL来表示空指针

## Comments（注释）

注释块应该使用`//`，而不是`/* */`。使用`//`可以更容易地在调试时注释掉一段代码。

```cpp
// this function does something
int myFunc()
{
}
```

要在调试期间注释掉这个函数块，我们可以这样做:

```cpp
/*
// this function does something
int myFunc()
{
}
*/
```

如果函数注释头使用`/* */`，这将是不可能的

## Never Use `using namespace` in a Header File（永远不要在头文件中使用命名空间）

这将导致您正在使用的命名空间被拉入包含头文件的所有文件的命名空间。它污染了名称空间，并且可能在将来导致名称冲突。不过，在实现文件中使用名称空间进行编写是可以的

> 这条语句就有点偏激了，有点标题党的意思

## Include Guards（include 守卫）

头文件必须包含一个名称清晰的include `guard`，以避免多次包含同一头文件的问题，并防止与其他项目的头文件冲突

```cpp
#ifndef MYPROJECT_MYCLASS_HPP
#define MYPROJECT_MYCLASS_HPP

namespace MyProject {
  class MyClass {
  };
}

#endif
```

您还可以考虑使用`#pragma once`指令，它是许多编译器的准标准。内容简短，意图明确

## {} Are Required for Blocks.（需要使用 {}）

不使用它们可能会导致代码中的语义错误

```cpp
// Bad Idea
// This compiles and does what you want, but can lead to confusing
// errors if modification are made in the future and close attention
// is not paid.
for (int i = 0; i < 15; ++i)
  std::cout << i << std::endl;

// Bad Idea
// The cout is not part of the loop in this case even though it appears to be.
int sum = 0;
for (int i = 0; i < 15; ++i)
  ++sum;
  std::cout << i << std::endl;


// Good Idea
// It's clear which statements are part of the loop (or if block, or whatever).
int sum = 0;
for (int i = 0; i < 15; ++i) {
  ++sum;
  std::cout << i << std::endl;
}
```

## Keep Lines a Reasonable Length（保持线条长度）

```cpp
// Bad Idea
// hard to follow
if (x && y && myFunctionThatReturnsBool() && caseNumber3 && (15 > 12 || 2 < 3)) {
}

// Good Idea
// Logical grouping, easier to read
if (x && y && myFunctionThatReturnsBool()
    && caseNumber3
    && (15 > 12 || 2 < 3)) {
}
```

许多项目和编码标准都有一个软准则，即每行使用的字符应该少于80或100个。这样的代码通常更容易阅读。它还可以让两个单独的文件在一个屏幕上相邻，而不用使用很小的字体

## Use "" for Including Local Files（使用""表示包含本地文件）

`<>` 用于包含系统库文件

```cpp
// 坏主意。对编译器需要额外的-I指令
// 以及违背标准
#include <string>
#include <includes/MyHeader.hpp>

// 坏主意
// 可能需要更具体的-I指令和
// 使代码更难打包和分发
#include <string>
#include <MyHeader.hpp>


// Good Idea
// 不需要额外的参数，并通知用户文件是本地文件
#include <string>
#include "MyHeader.hpp"
```

## Initialize Member Variables（初始化成员变量）

使用成员初始化器列表（initializer list）

对于POD类型，初始化器列表的性能与手动初始化相同，但对于其他类型，有明显的性能提升，见下文

```cpp
// Bad Idea
class MyClass
{
public:
  MyClass(int t_value)
  {
    m_value = t_value;
  }

private:
  int m_value;
};

// Bad Idea
// 这将导致对m_myOtherClass的额外构造函数调用
// 在assignment 之前
class MyClass
{
public:
  MyClass(MyOtherClass t_myOtherClass)
  {
    m_myOtherClass = t_myOtherClass;
  }

private:
  MyOtherClass m_myOtherClass;
};

// Good Idea
// 这里没有提高性能，但代码更清晰
class MyClass
{
public:
  MyClass(int t_value)
    : m_value(t_value)
  {
  }

private:
  int m_value;
};

// Good Idea
// m_myOtherClass的默认构造函数在这里永远不会被调用, so
// 如果MyOtherClass不是is_trivially_default_constructible(默认构造函数)，则会有性能提升
class MyClass
{
public:
  MyClass(MyOtherClass t_myOtherClass)
    : m_myOtherClass(t_myOtherClass)
  {
  }

private:
  MyOtherClass m_myOtherClass;
};
// std::is_trivially_default_constructible<T>::value
```

在C++ 11中，可以为每个成员分配默认值(使用`=`或使用`{}`)

### Assigning default values with =（使用=分配默认值）

```cpp
// ... //
private:
  int m_value = 0; // allowed(允许的)
  unsigned m_value_2 = -1; // 允许signed 到 unsigned 的narrowing(收缩)
// ... //
```

这确保没有构造函数“忘记”初始化成员对象

### Assigning default values with brace initialization（用大括号初始化来分配默认值）

使用大括号初始化不允许在编译时收缩

```cpp
// Best Idea

// ... //
private:
  int m_value{ 0 }; // allowed
  unsigned m_value_2 { -1 }; // 不允许signed 到 unsigned 的narrowing(收缩), 这会导致编译时错误
// ... //
```

首选`{}`初始化，而不是`=`，除非你有充分的理由不这样做。

忘记初始化成员是未定义行为错误的来源，这些错误通常很难发现。

如果成员变量在初始化后不会更改，则将其标记为`const`

```cpp
class MyClass
{
public:
  MyClass(int t_value)
    : m_value{t_value}
  {
  }

private:
  const int m_value{0};
};
```

由于不能给`const`成员变量赋新值，这样的类可能没有有意义的复制赋值操作符

## Always Use Namespaces（总是使用命名空间）

几乎没有理由在全局名称空间中声明标识符。相反，函数和类应该存在于适当命名的名称空间中，或者存在于名称空间内部的类中。放在全局命名空间中的标识符有可能与来自其他库(主要是C库，它们没有名称空间)的标识符发生冲突

## Use the Correct Integer Type for Standard Library Features（为标准库特性使用正确的整数类型）

标准库通常使用`std::size_t`来处理所有与size相关的内容。`size_t`的大小由实现定义

一般来说，使用`auto`可以避免大部分问题，但不是全部

确保使用正确的整数类型并与C++标准库保持一致。它可能不会在您当前使用的平台上发出警告，但当您更改平台时，它可能会发出警告。

注意，在对无符号值执行某些操作时，可能会导致整数下溢。例如:

```cpp
std::vector<int> v1{2,3,4,5,6,7,8,9};
std::vector<int> v2{9,8,7,6,5,4,3,2,1};
const auto s1 = v1.size();
const auto s2 = v2.size();
const auto diff = s1 - s2; // diff下溢到一个非常大的数
```

## Use .hpp and .cpp for Your File Extensions（使用.hpp和.cpp作为文件扩展名）

归根结底，这是个人喜好的问题，但是.hpp和.cpp被各种编辑器和工具广泛认可。因此，选择是务实的。具体来说，Visual Studio只能自动识别C++文件的.cpp和.cxx，而Vim不一定能识别。.cc为C++文件

一个特别大的项目([OpenStudio](https://github.com/NREL/OpenStudio))使用.hpp和.cpp来表示用户生成的文件，使用.hxx和.cxx来表示工具生成的文件。两者都是公认的，区分开来是有帮助的

## Never Mix Tabs and Spaces（不要混用制表符和空格）

一些编辑器喜欢在默认情况下混用制表符和空格进行缩进。这使得没有使用完全相同的制表符缩进设置的人无法阅读代码。配置您的编辑器，这样就不会发生这种情况

## Never Put Code with Side Effects Inside an assert() （永远不要把有副作用的代码放在assert里）

```cpp
assert(registerSomeThing()); // 确保registerSomeThing()返回true
```

上述代码在进行调试构建时成功，但在进行发布构建时被编译器删除，使您在调试和发布构建之间有不同的行为。这是因为`assert()`是一个宏，它在`release`模式下展开为空

## Don't Be Afraid of Templates（不要害怕模板）

它们可以帮助你坚持[DRY原则](http://en.wikipedia.org/wiki/Don%27t_repeat_yourself)。它们应该比宏更受欢迎，因为宏不尊重名称空间等

## Use Operator Overloads Judiciously（明智地使用操作符重载）

发明运算符重载是为了支持表达语法。两个大整数相加看起来像`a + b`，而不是a.add(b)。另一个常见的例子是std::string，通常使用`string1 + string2`连接两个字符串

但是，使用过多或错误的操作符重载很容易创建可读性不强的表达式。在重载操作符时，要遵循[stackoverflow](http://stackoverflow.com/questions/4421706/operator-overloading/4421708#4421708)中描述的三个基本规则

- Overloading `operator=()` when handling resources is a must. See [Consider the Rule of Zero](https://github.com/cpp-best-practices/cppbestpractices/blob/master/03-Style.md#consider-the-rule-of-zero) below. 当处理资源时必须重载操作符`=`()。参见下面的考虑零的规则
- 对于所有其他操作符，只有在通常连接到这些操作符的上下文中使用它们时才重载它们。典型的场景是用`+`连接事物，否定可以被认为是“真”或“假”的表达式，等等
- 一定要注意[操作符的优先级](http://en.cppreference.com/w/cpp/language/operator_precedence)，尽量避免不直观的构造
- 不要重载像~或%这样的外来操作符，除非实现的是数字类型或遵循特定领域中易于识别的语法
- [Never](http://stackoverflow.com/questions/5602112/when-to-overload-the-comma-operator?answertab=votes#tab-top) overload `operator,()` (the comma operator). 永远不要重载操作符，()(逗号操作符)
- 处理流时使用非成员函数`operator>>()`和`operator<<()`。例如，你可以重载操作符`<<(std::ostream &， MyClass const &)`来将你的类“写入”到一个流中，例如`std::cout`或`std::fstream`或`std::stringstream`。后者通常用于创建值的字符串表示
- [这里](http://stackoverflow.com/questions/4421706/operator-overloading?answertab=votes#tab-top)描述了更多需要重载的常见操作符

更多关于自定义操作符实现细节的技巧可以在[这里](http://courses.cms.caltech.edu/cs11/material/cpp/donnie/cpp-ops.html)找到

## Avoid Implicit Conversions（避免隐式转换）

### Single Parameter Constructors（单参数构造函数）

可以在编译时应用单个参数构造函数在类型之间自动转换。这对于像`std::string(const char *)`这样的事情很方便，但通常应该避免，因为它们可能会增加意外的运行时开销。

相反，将单个参数构造函数标记为显式`explicit`，这要求显式地调用它们

### Conversion Operators（转换操作符）

与单个形参构造函数类似，转换操作符可以由编译器调用，并引入意外开销。它们还应该被标记为`explicit`

```cpp
//bad idea
struct S {
  operator int() {
    return 2;
  }
};
//good idea
struct S {
  explicit operator int() {
    return 2;
  }
};
```

## Consider the Rule of Zero（考虑一下零的规则）

零的规则规定，除非所构造的类具有某种新的所有权形式，否则不能提供编译器可以提供的任何函数(复制构造函数、复制赋值操作符、移动构造函数、移动赋值操作符、析构函数)。

**目标是让编译器提供在添加更多成员变量时自动维护的最佳版本**

本文提供了一个背景，并解释了几乎100%实现的技术