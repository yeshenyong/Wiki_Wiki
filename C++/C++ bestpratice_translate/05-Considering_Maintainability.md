# Considering Maintainability（考虑可维护性）

## Avoid Compiler Macros（避免编译器宏）

编译器定义和宏在编译器运行之前被预处理器替换。这使得调试变得非常困难，因为调试器不知道源来自哪里

```cpp
// Bad Idea
#define PI 3.14159;

// Good Idea
namespace my_project {
  class Constants {
  public:
    // if the above macro would be expanded, then the following line would be:
    //   static const double 3.14159 = 3.14159;
    // which leads to a compile-time error. Sometimes such errors are hard to understand.
    static constexpr double PI = 3.14159;
  };
}
```

## Consider Avoiding Boolean Parameters（考虑避免bool参数）

在阅读代码时，它们不提供任何额外的含义。您可以创建一个具有更有意义的名称的单独函数，或者传递一个使意义更明确的枚举

看 http://mortoray.com/2015/06/15/get-rid-of-those-boolean-function-parameters/ 获得更多信息

## Avoid Raw Loops（避免生循环）

了解并理解现有的c++标准算法，并将其付诸使用

- 详细 [cppreference](https://en.cppreference.com/w/cpp/algorithm)
- 看 [C++ Seasoning](https://www.youtube.com/watch?v=qH6sSOr-yk8)

Consider a call to `[]` as a potential code smell, indicating that an algorithm was not used where it could have been.

将对`[]`的调用看作是一种潜在的代码气味，表明算法没有在本可以使用它的地方使用

## Never Use `assert` With Side Effects（永远不要使用有副作用的`assert`）

```cpp
// Bad Idea
assert(set_value(something));

// Better Idea
[[maybe_unused]] const auto success = set_value(something);
assert(success);
```

assert()将在发布版本（`release`）中被删除，这将防止`set_value`调用发生

所以，虽然第二个版本更难看，但第一个版本是不正确的

## Properly Utilize `override` and `final`（正确使用`override`和`final`）

这些关键字使其他开发人员清楚地知道虚函数是如何被利用的，如果虚函数的签名发生变化，它们可以捕获潜在的错误，并可能向[编译器提示](http://stackoverflow.com/questions/7538820/how-does-the-compiler-benefit-from-cs-new-final-keyword)可以执行的优化