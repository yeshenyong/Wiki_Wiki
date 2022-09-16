# std::ref用法

参考

https://murphypei.github.io/blog/2019/04/cpp-std-ref

https://github.com/downdemo/Cpp-Concurrency-in-Action-2ed/blob/master/docs/01%20%E7%BA%BF%E7%A8%8B%E7%AE%A1%E7%90%86.md

https://blog.csdn.net/m0_51551385/article/details/123965079



## 为什么要有`ref`

`std::ref` 用于取某个变量的引用，引入其是为了解决函数式编程（如`std::bind`、`thread`）的一些传参问题。





我们知道 C++ 中本来就有引用的存在，为何 C++11 中还要引入一个 `std::ref` 了？主要是考虑函数式编程（如 `std::bind`）在使用时，是对参数直接拷贝，而不是引用。下面通过例子说明

```cpp
#include <functional>
#include <iostream>

void f(int& n1, int& n2, const int& n3)
{
    std::cout << "In function: " << n1 << ' ' << n2 << ' ' << n3 << '\n';
    ++n1; // increments the copy of n1 stored in the function object
    ++n2; // increments the main()'s n2
    // ++n3; // compile error
}

int main()
{
    int n1 = 1, n2 = 2, n3 = 3;
    std::function<void()> bound_f = std::bind(f, n1, std::ref(n2), std::cref(n3));
    n1 = 10;
    n2 = 11;
    n3 = 12;
    std::cout << "Before function: " << n1 << ' ' << n2 << ' ' << n3 << '\n';
    bound_f();
    std::cout << "After function: " << n1 << ' ' << n2 << ' ' << n3 << '\n';
}
```



输出：

```sh
Before function: 10 11 12
In function: 1 11 12
After function: 10 12 12
```



上述代码在执行 `std::bind` 后，在函数 `f()` 中 `n1` 的值仍然是 1，`n2` 和 `n3` 改成了修改的值，**说明 `std::bind` 使用的是参数的拷贝而不是引用，因此必须显示利用 `std::ref` 来进行引用绑定**。具体为什么 `std::bind` 不使用引用，可能确实有一些需求，使得 C++11 的设计者认为默认应该采用拷贝，如果使用者有需求，加上 `std::ref` 即可。

示例2：

```cpp
#include<thread>
#include<iostream>
#include<string>

void threadFunc(std::string &str, int a)
{
    str = "change by threadFunc";
    a = 13;
}

int main()
{
    std::string str("main");
    int a = 9;
    std::thread th(threadFunc, std::ref(str), a);

    th.join();

    std::cout<<"str = " << str << std::endl;
    std::cout<<"a = " << a << std::endl;

    return 0;
}
```

输出：

```sh
str = change by threadFunc
a = 9
```



可以看到，和 `std::bind` 类似，多线程的 `std::thread` 也是必须显式通过 `std::ref` 来绑定引用进行传参，否则，形参的引用声明是无效的。





### ref和引用的区别

首先就是，上面的例子里，使用`bind`的时候，普通引用和`ref`引用有区别

`std::ref`只是尝试模拟引用传递，并不能真正变成引用，在非模板情况下，`std::ref`根本没法实现引用传递，只有**模板自动推导类型**或**类型隐式转换**时，`ref`能用包装类型`reference_wrapper`来代替原本会被识别的值类型，而`reference_wrapper`能隐式转换为被引用的值的引用类型。



## 为带参数的函数创建线程

- 有参数的函数也能传给 [std::thread](https://en.cppreference.com/w/cpp/thread/thread)，参数的默认实参会被忽略

```cpp
#include <thread>

void f(int i = 1) {}

int main() {
  std::thread t{f, 42};  // std::thread t{f} 则会出错，因为默认实参会被忽略
  t.join();
}
```

- 参数的引用类型也会被忽略，为此要使用 [std::ref](https://en.cppreference.com/w/cpp/utility/functional/ref)

```cpp
#include <cassert>
#include <thread>

void f(int& i) { ++i; }

int main() {
  int i = 1;
  std::thread t{f, std::ref(i)};
  t.join();
  assert(i == 2);
}
```

- **如果对一个实例的 non-static 成员函数创建线程，第一个参数类型为成员函数指针，第二个参数类型为实例指针，后续参数为函数参数**

```cpp
#include <iostream>
#include <thread>

class A {
 public:
  void f(int i) { std::cout << i; }
};

int main() {
  A a;
  std::thread t1{&A::f, &a, 42};  // 调用 a->f(42)
  std::thread t2{&A::f, a, 42};   // 拷贝构造 tmp_a，再调用 tmp_a.f(42)
  t1.join();
  t2.join();
}
```

- 如果要为参数是 move-only 类型的函数创建线程，则需要使用 [std::move](https://en.cppreference.com/w/cpp/utility/move) 传入参数

```cpp
#include <iostream>
#include <thread>
#include <utility>

void f(std::unique_ptr<int> p) { std::cout << *p; }

int main() {
  std::unique_ptr<int> p(new int(42));
  std::thread t{f, std::move(p)};
  t.join();
}
```









