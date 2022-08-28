# Chapter 23. Boost.Any



强类型语言，例如C++，要求每个变量都有一个特定的类型来定义它可以存储什么样的信息。这种实用像极了JavaScript、Python，允许开发人员将任何类型的信息存储在变量中



### 使用

例子

```cpp
#include <boost/any.hpp>

int main() {
    boost::any a = 1;
    a = 3.14;
    a = true;
}
```

要使用`boost::any` 

前提

1. 包含头文件 `boost/any.hpp` 
2. 存储在类型变量中的任何值都必须是可复制构造的
3. 因此不可以存储C 数组，因为C 数组是不可复制构造的



例子，将字符串存储在 `boost::any` 

```cpp
#include <boost/any.hpp>
#include <string>

int main() {
	boost:any a = std::string{"Boost"};
}
```

例子，访问`boost::any` 的值，请使用强转运算符 `boost::any_cast` 

```cpp
#include <boost/any.hpp>
#include <iostream>

int main() {
  boost::any a = 1;
  std::cout << boost::any_cast<int>(a) << '\n';
  a = 3.14;
  std::cout << boost::any_cast<double>(a) << '\n';
  a = true;
  std::cout << std::boolalpha << boost::any_cast<bool>(a) << '\n';
}
```



例子，如果 `boost::any_cast` 变量值被转换。如果指定了无效类型， `boost::bad_any_cast` 则会抛出类型异常

```cpp
#include <boost/any.hpp>
#include <iostream>

int main() {
  try
  {
    boost::any a = 1;
    std::cout << boost::any_cast<float>(a) << '\n';
  }
  catch (boost::bad_any_cast &e)
  {
    std::cerr << e.what() << '\n';
  }
}
```



例子，检查当前存储值的类型

```cpp
#include <boost/any.hpp>
#include <typeinfo>
#include <iostream>

int main() {
  boost::any a = 1;
  if (!a.empty())
  {
    const std::type_info &ti = a.type();
    std::cout << ti.name() << '\n';
  }
}
```



例子，通过指针访问值

```cpp
#include <boost/any.hpp>
#include <iostream>

int main() {
  boost::any a = 1;
  int *i = boost::any_cast<int>(&a);
  std::cout << *i << '\n';
}
```



### 源码实现

https://www.boost.org/doc/libs/1_80_0/boost/any.hpp

### 个人实现

`any.hpp` 

```cpp
#include <cassert>
#include <typeindex>
#include <typeinfo>
namespace selfboost {
class any {
    public:
        any() : content(nullptr) {}
        // 模板构造
        template <typename _Tp>
        any(const _Tp& value) : content(new holder<_Tp>(value)) {}
        // 拷贝构造
        any(const any& other) : content(other.content ? other.content->clone() : nullptr) {}
        // 移动构造
        any(any&& other) noexcept : content(other.content) { other.content = nullptr; }
        ~any() {
            if (!content) delete content;
        }

        // 判空
        bool isNull() const  { return content == nullptr; }
        const std::type_info& type() const { return content == nullptr ? typeid(void) : content->type(); }


        any& swap(any& rhs) noexcept {
            placeholder* tmp = content;
            content = rhs.content;
            rhs.content = tmp;
            return *this;
        }

        template <typename _Tp>
        _Tp *get() { return content == nullptr ? nullptr : content->type() == typeid(_Tp) ? &static_cast<any::holder<_Tp> *>(content)->hold : nullptr; }

        template <typename _Tp>
        any& operator=(const _Tp& rhs) {
            any(rhs).swap(*this);
            return *this;
        }

        // 完美转发
        template <typename _Tp>
        any& operator=(_Tp&& rhs) {
            any(static_cast<_Tp&&>(rhs)).swap(*this);
            return *this;
        }

        any & operator=(any rhs) {
            rhs.swap(*this);
            return *this;
        }
    public:
        bool empty() const noexcept {
            return !content;
        }

        void clear() noexcept {
            any().swap(*this);
        }
    private:
        // 保存数据接口
        class placeholder {
            public:
                virtual ~placeholder() {}
                // 查询类型
                virtual const std::type_info& type() const noexcept = 0;
                // 实现复制
                virtual placeholder* clone(void) = 0;          
        };
        template <typename _Tp>
        class holder final : public placeholder {
            public:
                holder(const _Tp& value) : hold(value) {}
                ~holder() {}
                const std::type_info& type() const noexcept override { return typeid(_Tp); }
                placeholder* clone() override {
                    return new holder(hold);
                }
                holder &operator=(const holder&) = delete;
            public:
                _Tp hold;
        };
        // 数据保存
        placeholder* content;
        // 用于类型转换
        template< typename _Tp>
        friend _Tp *any_cast(const any& rhs);
};
template <typename _Tp>
_Tp* any_cast(const any& rhs) {
    assert(rhs.type() == typeid(_Tp));
    return &(static_cast<any::holder<_Tp>*>(rhs.content)->hold);
}

} // namespace selfboost
```



使用

```cpp
#include "any.hpp"
#include <iostream>

using namespace std;

int main() {
    selfboost::any a = 1;
    std::cout << *selfboost::any_cast<int>(a) << '\n';
    a = 3.14;
    std::cout << *selfboost::any_cast<double>(a) << '\n';
    a = true;
    std::cout << std::boolalpha << *selfboost::any_cast<bool>(a) << '\n';


    return 0;
}
```



缺少功能

1. 异常捕捉（输出了不符合hold 类型）
2. 没有对一些不符合any 类型的（如C 数组）进行剔除
3. 待完善













