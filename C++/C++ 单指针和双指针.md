# C++ 单指针和双指针

## 1.一级指针和二级指针

一级指针即常说的指针，其保存[内存](https://so.csdn.net/so/search?q=内存&spm=1001.2101.3001.7020)地址；
二级指针即指向指针的指针，就是保存内存地址的指针变量的地址；

```cpp
例：
int a=1;
int *p=&a;　　   // p为a变量的地址，通过*p可以得到a的值
int **q=&p; 　　 // q为p指针的地址，通过**q可以得到a的值 

// 一级指针中
p：a的内存地址
*p：1（值）
    
// 二级指针中
q：指针p的内存地址
*q：a的内存地址
**q：1（值）
```

## 2.指针作为函数[参数传递](https://so.csdn.net/so/search?q=参数传递&spm=1001.2101.3001.7020)

### 程序1

```cpp
#include<stdio.h>
 
void  fun(int  *p){
    int  b=100;
    p=&b;
}
int main(){
    int  a=10;
    int  *q;
    q=&a;
    printf("%d\n",*q);
    fun(q);
    printf("%d\n",*q);
    return  0;
}
```

运行结果为：
10
10
分析：一级指针作为函数参数传递时，函数体内对指针做的变动不会对原始指针造成改变。**在传递一级指针时，只有对指针所指向的内存变量做操作才是有效的。**



### 程序2

```cpp
#include<stdio.h>
 
void  fun(int  **p){
    int  b=100;
    *p=&b;
}
int main(){
    int  a=10;
    int  *q;
    q=&a;
    printf("%d\n",*q);
    fun(&q);
    printf("%d\n",*q);
    return  0;
}
```

```c++
运行结果为：
10
100
分析：**传递二级指针时对指针指向做改变是有效的；**因为函数在传递参数时，编译器会为没有个函数参数制作一个副本，即拷贝；
例如：void fun(int *p)，指针参数p的副本为_p，编译器使_p=p，_p和p指向相同的内存空间，如果在函数内修改了_p所指向的内容，就会导致p的内容也做相应的改变。
但如果在函数内_p申请了新的内存空间或者指向其他内存空间，则_p指向了新的内存空间，而p依旧指向原来的内存空间，因此函数返回后p还是原来的p。这样的话，不但没有实现功能，反而每次都申请新的内存空间，而又得不到释放，因为没有将该内存空间的地址传递出来，容易造成内存泄露。
void fun(int **p)，如果函数参数是指针的地址，则可以通过该参数p将新分配或新指向的内存地址传递出来，这样就实现了有效的指针操作。
程序2中fun()函数运行完毕后里面的的局部变量b会被释放掉，此时main函数里面的q指向的内容是不确定的，之所以printf("%d\n",*q);能打印出100，是因为C++的内存清理机制是惰性的,所以调用完函数后执行几次cout<<*q; 会有意外的结果。
```

> graph-learn 中 也有使用二级指针

参考自：

https://blog.csdn.net/gre_paul/article/details/111881399

https://blog.csdn.net/qq_34721306/article/details/78666722