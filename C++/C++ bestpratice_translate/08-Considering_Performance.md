# Considering Performance（考虑性能）

## Build Time（构建时间）

### Forward Declare When Possible

This:

```cpp
// some header file
class MyClass;

void doSomething(const MyClass &);
```

而不是:

```cpp
// some header file
#include "MyClass.hpp"

void doSomething(const MyClass &);
```

这也适用于模板:

```cpp
template<typename T> class MyTemplatedType;
```

这是一种减少编译时间和重建依赖关系的主动方法

注意:向前声明确实会阻止更多的内联和优化。建议对发布版本使用**链接时间优化或链接时间代码生成**

### Avoid Unnecessary Template Instantiations（避免不必要的模板实例化）

模板不能自由实例化。实例化许多模板，或者使用比所需代码更多的模板会增加编译后的代码大小和构建时间

更多示例请参见 [this article](http://blog2.emptycrate.com/content/template-code-bloat-revisited-smaller-makeshared).

### Avoid Recursive Template Instantiations（避免递归模板实例化）

递归模板实例化会给编译器带来很大的负载，并且代码更难以理解

[如果可能的话，考虑使用可变展开和折叠](http://articles.emptycrate.com/2016/05/14/folds_in_cpp11_ish.html)

### Analyze the Build（分析构建）

工具[Templight](https://github.com/mikael-s-persson/templight) 可以用来分析项目的构建时间，建它需要一些努力，但一旦构建了它，就可以替换`clang++`了。 

在使用`temlight`构建之后，您需要分析结果，[templight-tools](https://github.com/mikael-s-persson/templight-tools) 提供了多种方法。(作者注:我建议使用 callgrind 转换器并使用 kcachegrind 可视化结果)

### Firewall Frequently Changing Header Files（频繁更改头文件是错误的）

#### Don't Unnecessarily Include Headers（不要不必要地包含头文件）

编译器必须对它看到的每个include指令做一些事情。即使它一看到`#ifndef` include守卫就停止，它仍然必须打开文件并开始处理它

[include-what-you-use](https://github.com/include-what-you-use/include-what-you-use) 是一个工具，可以帮助您确定需要哪些头文件

#### Reduce the load on the preprocessor（减少预处理的工作）

这是“频繁更改头文件”和“不要不必要地包含头文件”的一般形式。BOOST_PP之类的工具可能非常有用，但它们也给预处理器带来了巨大的负担

### Consider using precompiled headers（考虑使用预编译头文件）

在大型项目中，使用预编译头文件可以大大减少编译时间。选中的头文件被编译为编译器可以更快处理的中间形式(PCH文件)。建议只将经常使用且很少更改的头文件定义为预编译头文件(例如系统头文件和库头文件)，以减少编译时间。但你必须记住，使用预编译头文件有几个缺点:

- 预编译头文件的使用是不可移植的
- 生成的PCH文件依赖于机器
- 生成的PCH文件可能相当大
- 它可以打破你的头文件依赖关系。由于有预编译头文件，每个文件都有可能包含标记为预编译头文件的每个头文件。因此，如果禁用预编译的头文件，可能会导致构建失败。如果您发布的是库之类的东西，这可能会成为一个问题。因此，强烈建议构建一次启用预编译头文件，第二次不启用预编译头文件

最常见的编译器，如[GCC](https://gcc.gnu.org/onlinedocs/gcc/Precompiled-Headers.html)、[Clang](http://clang.llvm.org/docs/PCHInternals.html)和[Visual Studio](https://msdn.microsoft.com/en-us/library/szfdksca.aspx)都支持预编译头文件。像[cotire](https://github.com/sakra/cotire/) (cmake的一个插件)这样的工具可以帮助你在构建系统中添加预编译的头文件

### Consider Using Tools（考虑使用工具）

这些并不意味着要取代好的设计

- [ccache](https://ccache.samba.org/), 为 unix类的操作系统编译结果缓存
- [clcache](https://github.com/frerich/clcache), 编译结果缓存的cl.exe (MSVC)
- [warp](https://github.com/facebook/warp), Facebook 的预处理器

### Put tmp on Ramdisk（把tmp 放在内存虚拟硬盘上）

See [this](https://www.youtube.com/watch?v=t4M3yG1dWho) YouTube video for more details.

### Use the gold linker

如果是在Linux上，考虑为GCC使用 gold linker

## Runtime（运行期）

### Analyze the Code!（分析代码）

如果不分析代码，就无法知道瓶颈在哪里

代码分析器列表:

- [Intel VTune](https://software.intel.com/content/www/us/en/develop/tools/oneapi/components/vtune-profiler.html)
- [Coz - Causal Profiling](https://github.com/plasma-umass/coz)
- [Sleepy](http://www.codersnotes.com/sleepy)
- [Dyninst](https://dyninst.org/)
- [AMD CodeXL](https://github.com/GPUOpen-Archive/CodeXL)
- [lukestackwalker](http://lukestackwalker.sourceforge.net/)

### Simplify the Code（简化代码）

代码越干净，越简单，越容易阅读，编译器就越有机会很好地实现它

### Use Initializer Lists（使用初始化列表）

```cpp
// This
std::vector<ModelObject> mos{mo1, mo2};

// -or-
auto mos = std::vector<ModelObject>{mo1, mo2};
```

```cpp
// Don't do this
std::vector<ModelObject> mos;
mos.push_back(mo1);
mos.push_back(mo2);
```

初始化列表明显更高效;**减少对象复制**并**调整容器的大小**

### Reduce Temporary Objects（减少临时对象）

```cpp
// Instead of
auto mo1 = getSomeModelObject();
auto mo2 = getAnotherModelObject();

doSomething(mo1, mo2);
```

```cpp
// consider:

doSomething(getSomeModelObject(), getAnotherModelObject());
```

这类代码阻止编译器执行移动操作...（上述更好）

### Enable move operations（启用移动操作）

移动操作是C++ 11中最受推崇的特性之一。它们允许编译器通过移动临时对象而不是在某些情况下复制它们来避免额外的复制

我们所做的某些编码选择(例如声明自己的析构函数、赋值操作符或复制构造函数)会阻止编译器生成移动构造函数

对于大多数代码，一个简单的

```cpp
ModelObject(ModelObject &&) = default;
```

就足够了。然而，MSVC2013似乎还不喜欢这个代码

### 禁止`shared_ptr` 复制

`shared_ptr` 复制对象的成本比你想象的要高得多。这是因为引用计数必须是原子的和线程安全的。因此，这个内容只是加强了上面的注释:避免临时对象和过多的对象副本。仅仅因为我们使用一个pImpl，并不意味着我们的拷贝是免费的

### Reduce Copies and Reassignments as Much as Possible（尽可能减少拷贝和重分配）

对于更简单的情况，可以使用三元运算符:

```cpp
// Bad Idea
std::string somevalue;

if (caseA) {
  somevalue = "Value A";
} else {
  somevalue = "Value B";
}
```

```cpp
// Better Idea
const std::string somevalue = caseA ? "Value A" : "Value B";
```

使用[立即调用的lambda](http://blog2.emptycrate.com/content/complex-object-initialization-optimization-iife-c11)可以简化更复杂的情况

```cpp
// Bad Idea
std::string somevalue;

if (caseA) {
  somevalue = "Value A";
} else if(caseB) {
  somevalue = "Value B";
} else {
  somevalue = "Value C";
}
```

```cpp
// Better Idea
const std::string somevalue = [&](){
    if (caseA) {
      return "Value A";
    } else if (caseB) {
      return "Value B";
    } else {
      return "Value C";
    }
  }();
```

### Avoid Excess Exceptions（避免过多的异常）

在正常处理过程中在内部抛出和捕获的异常会降低应用程序的执行速度。它们还从调试器内部破坏用户体验，因为调试器监视并报告每个异常事件。最好是尽可能避免内部异常处理

### Get rid of “new”（摆脱 "new"）

我们已经知道我们不应该使用原始内存访问，所以我们使用`unique_ptr`和`shared_ptr`来代替，对吗?堆分配比堆栈分配要昂贵得多，但有时我们必须使用它们。更糟糕的是，创建`shared_ptr`实际上需要2个堆分配

然而，`make_shared`函数将其减少到只有一个

```cpp
std::shared_ptr<ModelObject_Impl>(new ModelObject_Impl());

// should become
std::make_shared<ModelObject_Impl>(); // (它也更加可读和简洁)
```

### Prefer `unique_ptr` to `shared_ptr`（优先选择`unique_ptr` 而不是 `shared_ptr`）

如果可能，使用`unique_ptr`代替`shared_ptr`。`unique_ptr`不需要跟踪它的副本，因为它是不可复制的。因此，它比`shared_ptr`更有效。等同于`shared_ptr`和`make_shared`，你应该使用`make_unique` (C++ 14或更高版本)来创建`unique_ptr`

```cpp
std::make_unique<ModelObject_Impl>();
```

目前的最佳实践建议也从工厂函数返回`unique_ptr`，然后在必要时将`unique_ptr`转换为`shared_ptr`

```cpp
std::unique_ptr<ModelObject_Impl> factory();

auto shared = std::shared_ptr<ModelObject_Impl>(factory());
```

### Get rid of std::endl（摆脱 std::endl）

`std::endl` 意味着一个刷盘操作符.。等价于 `"\n" << std::flush`.

### Limit Variable Scope（限制变量的作用域）

变量应该尽可能晚地声明，理想情况下，只有在可以初始化对象时才声明变量。减少变量作用域可以减少内存使用量，提高代码效率，并帮助编译器进一步优化代码

```cpp
// Good Idea
for (int i = 0; i < 15; ++i)
{
  MyObject obj(i);
  // do something with obj
}

// Bad Idea
MyObject obj; // meaningless object initialization
for (int i = 0; i < 15; ++i)
{
  obj = MyObject(i); // unnecessary assignment operation
  // do something with obj
}
// obj is still taking up memory for no reason
```

对于C++ 17及以上版本，可以考虑在`if`和`switch`语句中使用init-statement

```cpp
if (MyObject obj(index); obj.good()) {
    // do something if obj is good
} else {
    // do something if obj is not good
}
```

[这个主题有一个相关的讨论线程](https://github.com/lefticus/cppbestpractices/issues/52)

### Prefer `double` to `float`, But Test First（优先选择 `double` 而不是 `float`，不过要先测试）

根据情况和编译器的优化能力，一种可能比另一种更快。选择`float`将导致较低的精度，并且由于转换可能会较慢。在可向量化操作上，如果你能够牺牲精度，`float`浮点运算可能会更快

`double`是推荐的默认选择，因为它是C++中浮点值的默认类型

有关更多信息，请参阅[stackoverflow](http://stackoverflow.com/questions/4584637/double-or-float-which-is-faster)讨论

### Prefer `++i` to `i++`（优先选择 `++i` 而不是 `i++`）

当它在语义上正确时。前置自增比后置自增快，**因为它不需要创建对象的副本**

```cpp
// Bad Idea
for (int i = 0; i < 15; i++)
{
  std::cout << i << '\n';
}

// Good Idea
for (int i = 0; i < 15; ++i)
{
  std::cout << i << '\n';
}
```

即使许多现代编译器将这两个循环优化为相同的程序集代码，首选`++i`仍然是很好的实践。绝对没有理由不这样做，**而且你永远不能确定你的代码不会通过一个没有优化这一点的编译器**。您还应该意识到，编译器不能只针对整数类型进行优化，而不一定针对所有迭代器或其他用户定义的类型。底线是，如果前自增操作符在语义上与后自增操作符相同，那么建议使用它总是更容易

### Char is a char, string is a string（char 是 char，string 是 string）

```cpp
// Bad Idea
std::cout << someThing() << "\n";

// Good Idea
std::cout << someThing() << '\n';
```

这是非常次要的，但是`"\n"`必须被编译器解析为`const char *`，它必须在将其写入流(或附加到字符串)时对`\0`进行范围检查。已知`'\n'`是单个字符，可以避免许多CPU指令

如果多次低效使用，它可能会影响您的性能，但更重要的是，**考虑这两个用例会让您更多地考虑编译器和运行时必须做什么来执行您的代码**

### Never Use `std::bind`（绝不使用 `std::bind`）

`std::bind`几乎总是比您需要的开销(编译时和运行时)多得多。相反，只需使用`lambda`

```cpp
// Bad Idea
auto f = std::bind(&my_function, "hello", std::placeholders::_1);
f("world");

// Good Idea
auto f = [](const std::string &s) { return my_function("hello", s); };
f("world");
```

### Know The Standard Library（了解标准库）

正确使用供应商提供的标准库中已经高度优化的组件

#### `in_place_t` And Related（`in_place_t` 和相关）

注意如何使用`in_place_t`和相关标记来高效地创建诸如`std::tuple`、`std::any`和`std::varian`t等对象