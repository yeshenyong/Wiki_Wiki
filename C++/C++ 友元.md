# C++ 友元

介绍优质学习仓库：

https://github.com/yeshenyong/Wiki_Wiki（囊括计算机各种学习）

https://github.com/yeshenyong/practice_cpp （C/C++ 每日练习干货）

# 正文

> 什么时候希望一个类成为另一个类的友元呢？

了解友元，我们需要了解为什么要用友元、如何去使用友元

#### 友元类

> 一句话：通过友元类访问其它类的私有成员

举个例子：

一个家庭类可以声明为（即一个家庭可以有电视和电脑这些电子产品，但是它不独属于家里任何一个人，但是它可以被家庭的任何人使用与访问）

```C++
#ifndef FAMILY_h
#define FAMILY_H
class Family {
public:
    friend class FamilyMember
private:
    Tv _tv;			// 电视类
    Computer _cp;	// 电脑类
    ...
};

class FamilyMember {
public:
    void changeTv(Family& f) { f._tv = Tv("XIAOMI"); }
    void changeComputer(Family& f) { f._cp = Computer("Apple"); }
    void breakTv(Family& f) {...}
}
#endif	// FAMILY_H
```



#### 友元成员函数

> 一句话：通过友元成员函数对类私有成员进行访问

接着上面的例子：

有一天爸爸妈妈都觉得的每个人都可以直接控制电脑和电视机，不合理，家庭会议决定：大家只能通过各自接口`changeTv`和 `changeComputer` 来访问电视机和电脑，没必要整个类都成为另一个类的友元

```c++
class Family {
public:
    friend void FamilyMember::changeTv(Family &t);
    friend void FamilyMember::changeComputer(Family &t);
private:
    Tv _tv;			// 电视类
    Computer _cp;	// 电脑类
    ...
};
```

##### 友元函数带来的问题

然而，要使编译器能够处理这条语句，它必须知道 `FamilyMember` 的定义。

否则，它无法知道 `FamilyMember` 是一个类，而 `changeTv` 和 `changeComputer` 是这个类的方法，这意味着 `Family`要知道有 `FamilyMember` 的类方法，`FamilyMember` 要先知道`Family` 的定义。

所以，整个排列的顺序需要如下：

```c++
class Family;
class FamilyMember {}
class Family {}
```

而不能是

```c++
class FamilyMember;
class Family {}
class FamilyMember {}
```

详细请看，C++ 的声明与定义顺序



还有一个问题，就是 `FamilyMember` 声明包含了内联代码 

```C++
void changeTv(Family& f) { f._tv = Tv("XIAOMI"); }
```

由于这将导致调用的是`Family` 的一个方法，所以编译器此时必须已经看到了 `Family` 类的声明，这样才能知道`Family` 有哪些方法，但正如上述声明定义顺序。

所以解决方法是：将 `FamilyMember` 声明中包含方法声明，并将实际的定义放在 `Family` 类之后。这样，顺序又变成了

```c++
class Family;
class FamilyMember {
public:
    void changeTv(Family& f);
    void changeComputer(Family& f);
}
class Family {...}

inline void changeTv(Family& f) { f._tv = tv("XIAOMI"); }
inline void changeComputer(Family& f) { f._cp = Computer("Apple"); }
```



总结原因在于：

1. 编译器在 `Family` 类的声明中看到了 `FamilyMember` 的方法 `changeTv` 、`changeComputer` 被声明在了 `Family` 类的友元之前，应该先看到 `FamilyMember` 声明和 `changeTv` 方法的声明
2. 同时`changeTv` 和 `changeComputer` 又要知道 `Family` 中的声明



引入话题：

内联函数的连接性是内部的，这意味着函数定义必须在使用函数的文件中。在这个例子中，内敛函数位于头文件中，因此在使用函数的文件中包含头文件可确保定义放在正确的地方。也可以将实现放在实现文件中 ，但必须删除关键字inline，这样函数的链接性将是外部的。

























