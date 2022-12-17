一、

最直观的解决方法就是将其[构造函数](https://so.csdn.net/so/search?q=构造函数&spm=1001.2101.3001.7020)声明为私有的，这样就可以阻止子类构造对象了。但是这样的话，就无法构造本身的对象了，就无法利用了。
既然这样，我们又可以想定义一个静态方法来构造类和释放类。

```cpp
#include<iostream>
 using namespace std;
  class A
  {
  public:
     static A * Construct(int n)
      {
          A *pa = new A;
          pa->num = n;
          cout<<"num is:"<<pa->num<<endl;
          return pa;
     }
     static void Destruct(A * pIntance)
     {
         delete pIntance;
         pIntance = NULL;
     }
     private:
             A(){}
             ~A(){}
     public:
             int num;
 };
 int  main()
 {
     A *f = A::Construct(9);
     cout<<f->num<<endl;
     A::Destruct(f);
     return 0;
 }
```

二、

利用友元不能被继承的特性，可以实现这样的类。
主要思想，设计一个模板辅助类Base，将构造函数声明为私有的；再设计一个不能继承的类FinalClass,,将FinalClass 作为Base的友元类。
将FinalClass虚继承Base。先贴代码，然后再解释其中的原理吧

```cpp
include <iostream>
using namespace std;

template <typename T>
class Base{
    friend T;
private:
    Base(){
        cout << "base" << endl;
    }
    ~Base(){}
};

class FinalClass : virtual public Base<FinalClass>{  
 //一定注意 必须是虚继承
public:
    FinalClass(){
        cout << "FinalClass()" << endl;
    }
};

class C:public FinalClass{
public:
    C(){}     //继承时报错，无法通过编译
};


int main(){
    FinalClass b;      //B类无法被继承
    //C c;
    return 0;
}
```

类Base的构造函数和析构函数因为是私有的，只有Base类的友元可以访问，FinalClass类在继承时将模板的参数设置为了FinalClass类，所以构造FinalClass类对象时们可以直接访问父类（Base）的构造函数。

为什么必须是虚继承呢？
虚继承的功能是：当出现了菱形继承体系的时候，使用虚继承可以防止二义性，即子孙类不会继承多个原始祖先类。这有什么用呢？
那么虚继承如何解决这种二义性的呢？从具有虚基类的类继承的类在初始化时进行了特殊处理，在虚派生中，由最低层次的派生类的构造函数初始化虚基类
结合上面的代码来解释：C 在调用构造函数时，不会先调用FinalClass的构造函数，而是直接调用Base的构造函数，C不是Base的友元类，所以无法访问。这样的话C就不能继承FinalClass。

三、C++ 11 引入关键字`final`

希望类不可被继承或函数不可被重载时使用



总结：

用三