# C++中不一样的重载

转载自：https://mp.weixin.qq.com/s/xsMwv1hR31YqquPFB4oTWw

关于什么是重载想必大家都知道，无非就是参数个数不同，参数类型不同可以重载，这里我就不过多介绍啦。



这里想分享C++中一个不一样的重载，即const重载。



可以看下面的代码：

```c++
struct A {
    int count() {
        std::cout << "non const" << std::endl;
        return 1;
    }

    int count() const {
        std::cout << "const" << std::endl;
        return 1;
    }
};

int main() {
    A a;
    a.count();
    const A b;
    b.count();
}
```

输出的是这样：

```c
non const
const
```



不知道大家平时有没有关注过这种用法，没关注过可以继续向下看哈。



![图片](https://mmbiz.qpic.cn/mmbiz_png/iaFp77VHVDePrc5TvQGXzzWwicp8jNAyz4O2uURnfYBeoHZ1VItu2A5IQFO5ohzNYV0ABcPcHCZeOtMomUILe16g/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)



从上面的输出结果我们也可以看到，const修饰的对象调用的是使用const修饰的方法，非const对象调用的是非const的方法。



然而想必大家都知道对象调用的原理：



看下面的这段代码：

```c++
A a;
a.func();
```



其实到底层，函数可能会变成这样：

```c++
func(A* a);
```



**函数是在代码段，对象是在数据段，调用不同对象的函数，其实只不过是同一个函数，传递了不同的数据参数而已。**



上面的是把对象的this指针传进去。



再回到上面的代码：

```c++
struct A {
    int count() {
        std::cout << "non const" << std::endl;
        return 1;
    }

    int count() const {
        std::cout << "const" << std::endl;
        return 1;
    }
};
```



可以理解为：

```c++

int count(A *);
int count(const A*);
```



咦，这不就是重载吗，难道还有const重载？



还真有，看下这段代码：

```c
struct A {
    int count(const int& s) {
        std::cout << "const" << std::endl;
        return 1;
    }

    int count(int& s) {
        std::cout << "non const" << std::endl;
        return 1;
    }
};

int main() {
    A a;
    a.count(4);
    int c = 5;
    a.count(c);
}
```



输出如下：

```c
const
non const
```

所以得出结论：



**不只是参数类型和个数不同会产生重载，const修饰的参数也会有重载。**



但是只有当const修饰的是指针或者引用类型时才可以，普通的int和const int会编译失败的，具体大家可以自己写代码试试。



> 修饰对象是否可变的问题。



这里大家也可以想想，问什么一定要指针或者引用类型时重载才可以呢？

