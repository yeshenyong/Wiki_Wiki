# Considering Safety（考虑到安全）

## Const as Much as Possible（尽可能多的const）

`const`告诉编译器一个变量或方法是不可变的。这有助于编译器优化代码，并帮助开发人员知道函数是否有副作用。此外，使用`const &`可以防止编译器复制不必要的数据。John Carmack[对const的评论](http://kotaku.com/454293019)也很值得一读

```cpp
// Bad Idea
class MyClass
{
public:
  void do_something(int i);
  void do_something(std::string str);
};


// Good Idea
class MyClass
{
public:
  void do_something(const int i);
  void do_something(const std::string &str);
};
```

### Carefully Consider Your Return Types（仔细考虑你的返回类型）

- Getters
  - Returning by `&` or `const &` 当正常使用返回值进行观察时，可以显著节省性能
  - 按值返回更有利于线程安全，如果返回值的正常使用无论如何都是为了复制，那么就不会有性能损失
  - 如果你的API使用协变返回类型（covariant return types），你必须返回b `&` or `*`
- Temporaries and local values（临时值和本地值）
  - 始终按值返回

### Do not pass and return simple types by const ref（不要通过const引用传递和返回简单类型）

```cpp
// Very Bad Idea
class MyClass
{
public:
  explicit MyClass(const int& t_int_value)
    : m_int_value(t_int_value)
  {
  }

  const int& get_int_value() const
  {
    return m_int_value;
  }

private:
  int m_int_value;
}
```

相反，通过值传递返回简单类型，如果不打算更改传递的值，请将它们声明为`const`, 而不是`const &`  

```cpp
// Good Idea
class MyClass
{
public:
  explicit MyClass(const int t_int_value)
    : m_int_value(t_int_value)
  {
  }

  int get_int_value() const
  {
    return m_int_value;
  }

private:
  int m_int_value;
}
```

为什么?因为通过引用传递和返回会导致指针操作，而不是在处理器寄存器中更快地传递值

## Avoid Raw Memory Access（避免访问原始内存）

原始内存的访问、分配和释放在c++中很难在[不冒内存错误](http://blog2.emptycrate.com/content/nobody-understands-c-part-6-are-you-still-using-pointers)和泄漏的风险的情况下得到正确的处理。C++ 11提供了一些工具来避免这些问题

```cpp
// Bad Idea
MyClass *myobj = new MyClass;

// ...
delete myobj;


// Good Idea
auto myobj = std::make_unique<MyClass>(constructor_param1, constructor_param2); // C++14
auto myobj = std::unique_ptr<MyClass>(new MyClass(constructor_param1, constructor_param2)); // C++11
auto mybuffer = std::make_unique<char[]>(length); // C++14
auto mybuffer = std::unique_ptr<char[]>(new char[length]); // C++11

// or for reference counted objects
auto myobj = std::make_shared<MyClass>();

// ...
// myobj is automatically freed for you whenever it is no longer used.
// 当不再使用时，将自动为您释放
```

## Use `std::array` or `std::vector` Instead of C-style Arrays（使用 `std::array` `std::vector` 替代C 类型数组）

这两种方法都保证了对象的连续内存布局，并且可以(而且应该)完全取代c风格数组的使用，以解决不使用裸指针的许多原因

另外，[避免](http://stackoverflow.com/questions/3266443/can-you-use-a-shared-ptr-for-raii-of-c-style-arrays)使用`std::shared_ptr`保存数组

## Use Exceptions（使用异常）

异常不能被忽略。返回值，例如使用`boost::optional`，可以被忽略，如果不检查，可能会导致崩溃或内存错误。另一方面，异常可以被捕获和处理。可能一直到应用程序的最高级别，并带有日志和应用程序的自动重启

## Use C++-style cast instead of C-style cast（使用C++风格的类型转换，而不是C风格的类型转换）

使用c++风格的强制转换(`static_cast<>`， `dynamic_cast<>`…)而不是C风格的强制转换。c++风格的强制转换允许更多的编译器检查，而且相当安全

```cpp
// Bad Idea
double x = getX();
int i = (int) x;

// Not a Bad Idea
int i = static_cast<int>(x);
```

此外，C++类型转换样式更明显，并且有可能进行搜索。

但是，如果需要将double类型转换为int类型，请考虑重构程序逻辑(例如，对溢出和下溢进行额外检查)。测量三次，切割0.99999999981次

## Do not define a variadic function（不定义可变参数函数）

可变参数函数可以接受数量可变的参数。最著名的例子可能是printf()。您可以自己定义此类函数，但这可能存在安全风险。可变参数函数的使用不是类型安全的，并且错误的输入参数可能导致程序终止，并导致未定义的行为。这种未定义的行为可能会导致安全问题。如果您有可能使用支持C++ 11的编译器，您可以使用可变参数模板

[从技术上讲，可以用一些编译器创建类型安全的C风格可变函数](https://github.com/lefticus/cppbestpractices/issues/53)

## Additional Resources（其他资源）

David Wheeler的《[How to Prevent The Heartbleed](https://dwheeler.com/essays/heartbleed.html)》一书很好地分析了代码安全的现状以及如何确保代码安全