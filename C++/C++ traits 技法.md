# C++ traits 技法

traits 是个技法，不是区别于普通C++ 的特殊语法！



> traits 编程技法大量运用于STL 实现品种，它利用“内嵌型别（类别）” 的编程技巧与编译器的template 参数推导功能，增强 C++ 未能提供的关于型别（类别）认证方面的能力，弥补C++ 不为强型别（strong typed）语言的遗憾。了解traits 编程技法，就像获得“芝麻开门” 的口诀一样，从此得以一窥STL 源代码的堂奥。



学习自：STL 源码剖析

```cpp
// 节录自SGI STL <stl_iterator.h>
// 五种迭代器类型
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

// 为避免写码时挂㆒漏万，自行开发的迭代器最好继承自㆘面这个std::iterator
template <class Category, class T, class Distance = ptrdiff_t,
class Pointer = T*, class Reference = T&>
struct iterator {
    typedef Category iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef Pointer pointer;
    typedef Reference reference;
};

// 「榨汁机」traits
template <class Iterator>
struct iterator_traits {
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::value_type value_type;
    typedef typename Iterator::difference_type difference_type;
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::reference reference;
};

// 针对原生指标（native pointer）而设计的traits 偏特化版。
template <class T>
struct iterator_traits<T*> {
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
};

// 针对原生之 pointer-to-const 而设计的traits 偏特化版
template <class T>
struct iterator_traits<const T*> {
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef const T* pointer;
    typedef const T& reference;
};

template <class Iterator>
inline typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator&) {
    typedef typename iterator_traits<Iterator>::iterator_category category;
    return category();
}
```



> 根据类的继承关系和关系，利用函数的重载功能实现if-else

```cpp
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
```



