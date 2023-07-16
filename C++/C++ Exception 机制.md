# C++ Exception 机制

[C++异常机制 (qq.com)](https://mp.weixin.qq.com/s/wR89iBn9RCmhqP6sLm1SpA)

异常处理是C++的一项语言机制，用于在程序中处理异常事件。异常事件在 C++ 中表示为**异常对象**。

异常事件发生时，程序使用throw关键字抛出异常表达式，抛出点称为异常出现点，由操作系统为程序设置当前异常对象，然后执行程序的当前异常处理代码块，在包含了异常出现点的最内层的 **try** 块，依次匹配catch语句中的异常对象（只进行类型匹配，catch参数有时在 catch 语句中并不会使用到）。若匹配成功，则执行 catch 块内的异常处理语句，然后接着执行 **try...catch...** 块之后的代码。如果在当前的 try...catch... 块内找不到**匹配**该异常对象的catch语句,则由更外层的 try...catch... 块来处理该异常；如果当前函数内所有的 try...catch... 块都不能匹配该异常，则递归回退到调用栈的上一层去处理该异常。如果一直退到主函数 main() 都不能处理该异常，则调用系统函数 terminate() 终止程序。

执行 throw 语句时，throw 表达式将作为对象被复制构造为一个新的对象，称为异常对象。异常对象放在内存的特殊位置，该位置既不是栈也不是堆，**在 window 上是放在线程信息块 TIB 中**。这个构造出来的新对象与本级的 try 所对应的 catch 语句进行**类型匹配**，类型匹配的原则在下面介绍。

![图片](https://mmbiz.qpic.cn/mmbiz_png/pldYwMfYJphp2icoiao3CKJL6cGrUibUQC4V17hTqntQWvYlZ6MuJLEVL3LXOmUwhYBQiaucaI9R6IgspRcH6Z8onw/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

在本例中，依据 score 构造出来的对象类型为 int，与 catch(int score) 匹配上，程序控制权转交到 catch 的语句块，进行异常处理代码的执行。如果在本函数内与 catch 语句的类型匹配不成功，则在调用栈的外层函数继续匹配，如此递归执行直到匹配上 catch 语句，或者直到 main 函数都没匹配上而调用系统函数 terminate() 终止程序。

当执行一个 throw 语句时，跟在 throw 语句之后的语句将不再被执行，throw 语句的语法有点类似于 return，因此导致在调用栈上的函数可能提早退出。

除了抛出用户自定义的类型外，C++ 标准库定义了一组类，用户报告标准库函数遇到的问题。这些标准库异常类只定义了几种运算，包括创建或拷贝异常类型对象，以及为异常类型的对象赋值。

| 标准异常类       |                          描述                          |    头文件 |
| ---------------- | :----------------------------------------------------: | --------: |
| exception        | 最通用的异常类，只报告异常的发生而不提供任何额外的信息 | exception |
| runtime_error    |              只有在运行时才能检测出的错误              | stdexcept |
| rang_error       |       运行时错误：产生了超出有意义值域范围的结果       | stdexcept |
| overflow_error   |                  运行时错误：计算上溢                  | stdexcept |
| underflow_error  |                  运行时错误：计算下溢                  | stdexcept |
| logic_error      |                      程序逻辑错误                      | stdexcept |
| domain_error     |            逻辑错误：参数对应的结果值不存在            | stdexcept |
| invalid_argument |                   逻辑错误：无效参数                   | stdexcept |
| length_error     |     逻辑错误：试图创建一个超出该类型最大长度的对象     | stdexcept |
| out_of_range     |           逻辑错误：使用一个超出有效范围的值           | stdexcept |
| bad_alloc        |                    内存动态分配错误                    |       new |
| bad_cast         |                dynamic_cast类型转换出错                | type_info |

### catch 关键字

catch语句匹配被抛出的异常对象。如果 catch 语句的参数是引用类型，则该参数可直接作用于异常对象，即参数的改变也会改变异常对象，而且在 catch 中**重新抛出异常**时会继续传递这种改变。如果 catch 参数是传值的，则复制构函数将依据异常对象来构造catch 参数对象。在该 catch 语句结束的时候，先析构 catch 参数对象，然后再析构异常对象。

在进行异常对象的匹配时，编译器不会做任何的隐式类型转换或类型提升。除了以下几种情况外，异常对象的类型必须与 catch 语句的声明类型完全匹配：

1. 允许从非常量到常量的类型转换。
2. 允许派生类到基类的类型转换。
3. 数组被转换成指向数组（元素）类型的指针。
4. 函数被转换成指向函数类型的指针。

寻找 catch 语句的过程中，匹配上的未必是类型完全匹配那项，而在是最靠前的第一个匹配上的 catch 语句（我称它为最先匹配原则）。所以，派生类的处理代码 catch 语句应该放在基类的处理 catch 语句之前，否则先匹配上的总是参数类型为基类的 catch 语句，而能够精确匹配的 catch 语句却不能够被匹配上。

在 catch 块中，如果在当前函数内无法解决异常，可以继续向外层抛出异常，让外层catch 异常处理块接着处理。此时可以使用不带表达式的 throw 语句将捕获的异常重新抛出：

```cpp
catch(type x)
{
    //做了一部分处理
    throw;
}
```

### **栈展开、RAII**

其实栈展开已经在前面说过，就是从异常抛出点一路向外层函数寻找匹配的 catch 语句的过程，寻找结束于某个匹配的 catch 语句或标准库函数 terminate。这里重点要说的是栈展开过程中对局部变量的销毁问题。我们知道，在函数调用结束时，函数的局部变量会被系统自动销毁，类似的，throw 可能会导致调用链上的语句块提前退出，此时，语句块中的局部变量将按照构成生成顺序的逆序，依次调用析构函数进行对象的销毁。例如下面这个例子：

```cpp

//一个没有任何意义的类
class A
{
public:
    A() :a(0){ cout << "A默认构造函数" << endl; }
    A(const  A& rsh){ cout << "A复制构造函数" << endl; }
    ~A(){ cout << "A析构函数" << endl; }
private:
    int  a;
};
int main()
{
        try
        {
            A a ;
            throw a;
        }
        catch (A a)
        {
            ;
        }
    return 0;
}
```

程序将输出：

![图片](https://mmbiz.qpic.cn/mmbiz_png/pldYwMfYJphp2icoiao3CKJL6cGrUibUQC47gMbuHia3OIS2cMAcrCfrw6SrHGD1iaiaAPATYebch7ukEic1LO7ZqprEA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

定义变量 a 时调用了默认构造函数，使用 a 初始化异常变量时调用了复制构造函数，使用异常变量复制构造 catch 参数对象时同样调用了复制构造函数。三个构造对应三个析构，也即 try 语句块中局部变量 a 自动被析构了。然而，如果 a 是在自由存储区上分配的内存时：

```cpp

int main()
{
    try
    {
        A * a= new A;
        throw *a;
    }
    catch (A a)
    {
        ;
    }
    getchar();
    return 0;
}
```

程序运行结果：

![图片](https://mmbiz.qpic.cn/mmbiz_png/pldYwMfYJphp2icoiao3CKJL6cGrUibUQC4hsKQ4KJ2wah81gsUatcpibsaDOAOSJWA2urYwNXPVzvibAf6PibUkevKg/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)



同样的三次构造，却只调用了两次的析构函数！说明 a 的内存在发生异常时并没有被释放掉，发生了内存泄漏。

**RAII机制有助于解决这个问题**，RAII（Resource acquisition is initialization，资源获取即初始化）。它的思想是**以对象管理资源**。为了更为方便、鲁棒地释放已获取的资源，避免资源死锁，一个办法是把资源数据用对象封装起来。程序发生异常，执行栈展开时，封装了资源的对象会被自动调用其析构函数以释放资源。C++ 中的智能指针便符合RAII。关于这个问题详细可以看

**异常机制与构造函数**

异常机制的一个合理的使用是在构造函数中。构造函数没有返回值，所以应该使用异常机制来报告发生的问题。更重要的是，构造函数抛出异常表明构造函数还没有执行完，其对应的析构函数不会自动被调用，因此析构函数应该先析构所有所有已初始化的基对象，成员对象，再抛出异常。


C++ 类构造函数初始化列表的异常机制，称为 function-try block。一般形式为：

```cpp
myClass::myClass(type1 pa1)
    try:  _myClass_val (初始化值)
{
  /*构造函数的函数体 */
}
  catch ( exception& err )
{
  /* 构造函数的异常处理部分 */
};
```

**异常机制与析构函数**

C++ 不禁止析构函数向外界抛出异常，但析构函数被期望不向外界函数抛出异常。析构函数中向函数外抛出异常，将直接调用 terminator() 系统函数终止程序。如果一个析构函数内部抛出了异常，就应该在析构函数的内部捕获并处理该异常，不能让异常被抛出析构函数之外。可以如此处理：

1. 若析构函数抛出异常，调用 std::abort() 来终止程序。
2. 在析构函数中 catch 捕获异常并作处理。

关于具体细节，有兴趣可以看《Effective C++》条款08：**别让异常逃离析构函数**。

# noexcept修饰符与noexcept操作符

noexcept 修饰符是 C++11 新提供的异常说明符，用于声明一个函数不会抛出异常。编译器能够针对不抛出异常的函数进行优化，另一个显而易见的好处是你明确了某个函数不会抛出异常，别人调用你的函数时就知道不用针对这个函数进行异常捕获。在 C++98中关于异常处理的程序中你可能会看到这样的代码：

```cpp
void func() throw(int ,double ) {...}void func() throw(){...}
```

这是 throw 作为函数异常说明，前者表示 func（）这个函数可能会抛出 int 或 double 类型的异常，后者表示 func() 函数不会抛出异常。事实上前者很少被使用，在 C++11 这种做法已经被摒弃，而后者则被 C++11 的 noexcept 异常声明所代替：

```cpp
void func() noexcept {...}//等价于void func() throw(){...}
```

在 C++11 中，编译器并不会在编译期检查函数的 noexcept 声明，因此，被声明为noexcept 的函数若携带异常抛出语句还是可以通过编译的。在函数运行时若抛出了异常，编译器可以选择直接调用 terminate() 函数来终结程序的运行，因此，noexcept 的一个作用是**阻止异常的传播,提高安全性**.

上面一点提到了，我们不能让异常逃出析构函数，因为那将导致程序的不明确行为或直接终止程序。实际上出于安全的考虑，C++11 标准中让类的析构函数默认也是 noexcept 的。同样是为了安全性的考虑，经常被析构函数用于释放资源的 delete 函数，C++11 也默认将其设置为 noexcept。

**异常处理的性能分析**

异常处理机制的主要环节是运行期类型检查。当抛出一个异常时，必须确定异常是不是从 try 块中抛出。异常处理机制为了完善异常和它的处理器之间的匹配，需要存储每个异常对象的类型信息以及 catch 语句的额外信息。由于异常对象可以是任何类型（如用户自定义类型），并且也可以是多态的，获取其动态类型必须要使用运行时类型检查（RTTI），此外还需要运行期代码信息和关于每个函数的结构。

当异常抛出点所在函数无法解决异常时，异常对象沿着调用链被传递出去，程序的控制权也发生了转移。转移的过程中为了将异常对象的信息携带到程序执行处（如对异常对象的复制构造或者 catch 参数的析构），在时间和空间上都要付出一定的代价，本身也有不安全性，特别是异常对象是个复杂的类的时候。

异常处理技术在不同平台以及编译器下的实现方式都不同，**但都会给程序增加额外的负担，当异常处理被关闭时，额外的数据结构、查找表、一些附加的代码都不会被生成**，正是因为如此，对于明确不抛出异常的函数，我们需要使用 noexcept 进行声明。





### **异常对象**

**异常对象**是一种特殊的对象，编译器依据异常抛出表达式复制构造异常对象，这要求抛出异常表达式不能是一个不完全类型（一个类型在声明之后定义之前为一个不完全类型。不完全类型意味着该类型没有完整的数据与操作描述），而且可以进行复制构造，这就要求异常抛出表达式的复制构造函数（或移动构造函数）、析构函数不能是私有的。





​	本文的目的是对C++ Exception进行简单测试与分析。首先对Exception的性能进行评测，探究C++ Exception对程序性能的影响，然后对C++ Exception的实现机制做一个简单探索，让大家明白Exception对程序运行到底产生了哪些影响，进而写出更高质量的代码。



### **Benchmark**

首先我们先通过性能测试直观地感受一下添加Exception对程序性能的影响。

参考Investigating the Performance Overhead of C++ Exceptions（网址：https://pspdfkit.com/blog/2020/performance-overhead-of-exceptions-in-cpp/）的测试思路，我们对其测试用例进行改动。简单解释一下我们的测试代码，我们定义一个函数，该函数会根据概率决定是否调用目标函数：





```cpp
const int randomRange = 2;const int errorInt = 1;int getRandom() { return random() % randomRange; }
template<typename T>T testFunction(const std::function<T()>& fn) {    auto num = getRandom();    for (int i{0}; i < 5; ++i) {        if (num == errorInt) {            return fn();        }    }}
```



执行testFunction时，目标函数fn有50%的概率被调用。



```cpp
void exitWithStdException() {    testFunction<void>([]() -> void {        throw std::runtime_error("Exception!");    });}
void BM_exitWithStdException(benchmark::State& state) {    for (auto _ : state) {        try {            exitWithStdException();        } catch (const std::runtime_error &ex) {            BLACKHOLE(ex);        }    }}
```



BM_exitWithStdException用于测试函数exitWithStdException，该函数会抛出一个Exception，然后在BM_exitWithStdException中立刻被catch，catch后我们什么也不做。


类似的，我们设计用于测试ErrorCode模式的代码如下：

```cpp
void BM_exitWithErrorCode(benchmark::State& state) {    for (auto _ : state) {        auto err = exitWithErrorCode();        if (err < 0) {            // handle_error()            BLACKHOLE(err);        }    }}
int exitWithErrorCode() {    testFunction<int>([]() -> int {        return -1;    });
    return 0;}
```



将ErrorCode测试代码放进try{...}catch{...} 测试只进入try是否会对性能有影响：

```cpp
void BM_exitWithErrorCodeWithinTry(benchmark::State& state) {    for (auto _ : state) {        try {            auto err = exitWithErrorCode();            if (err < 0) {                BLACKHOLE(err);            }        } catch(...) {        }    }}
```



利用gtest/banchmark开始我们的测试：

```cpp
BENCHMARK(BM_exitWithStdException);BENCHMARK(BM_exitWithErrorCode);BENCHMARK(BM_exitWithErrorCodeWithinTry);
BENCHMARK_MAIN();
```



测试结果：



```
2021-07-08 20:59:44Running ./benchmarkTests/benchmarkTestsRun on (12 X 2600 MHz CPU s)CPU Caches:  L1 Data 32K (x6)  L1 Instruction 32K (x6)  L2 Unified 262K (x6)  L3 Unified 12582K (x1)Load Average: 2.06, 1.88, 1.94***WARNING*** Library was built as DEBUG. Timings may be affected.------------------------------------------------------------------------Benchmark                              Time             CPU   Iterations------------------------------------------------------------------------BM_exitWithStdException             1449 ns         1447 ns       470424BM_exitWithErrorCode                 126 ns          126 ns      5536967BM_exitWithErrorCodeWithinTry        126 ns          126 ns      5589001
```

###  

这是我在自己的mac上测试的结果，使用的编译器版本为gcc version 10.2.0，异常模型为DWARF2。可以看到，当Error/Exception发生率为50%时，Exception的处理速度要比返回ErrorCode慢10多倍。同时，对一段不会抛出异常的代码添加try{...}catch{...}则不会对性能有影响。我们可以再将Error/Exception的发生率调的更低测试下：

### 





```
const int randomRange = 100;const int errorInt = 1;int getRandom() { return random() % randomRange; }
```



我们将异常的概率降低到了 1%，继续测试：





```
2021-07-08 21:16:01Running ./benchmarkTests/benchmarkTestsRun on (12 X 2600 MHz CPU s)CPU Caches:  L1 Data 32K (x6)  L1 Instruction 32K (x6)  L2 Unified 262K (x6)  L3 Unified 12582K (x1)Load Average: 2.80, 2.22, 1.93***WARNING*** Library was built as DEBUG. Timings may be affected.------------------------------------------------------------------------Benchmark                              Time             CPU   Iterations------------------------------------------------------------------------BM_exitWithStdException              140 ns          140 ns      4717998BM_exitWithErrorCode                 111 ns          111 ns      6209692BM_exitWithErrorCodeWithinTry        113 ns          113 ns      6230807
```



### 

可以看到，Exception模式的性能大幅提高，接近了ErrorCode模式。

 从实验结果，我们可以得出如下的结论：

 1.在throw发生的很频繁的情况（50%）下，Exception机制相比 ErrorCode 会慢非常多；

 2.在throw并不是经常发生的情况（1%）下，Exception机制并不会比 ErrorCode 慢。

 由此结论，我们可以进而得到如下的使用建议：



- 不要使用try{throw ...}catch(){...}来充当你的代码控制流，这会导致你的C++慢的离谱。
- 应当把Exception用在真正发生异常的情况下，比如内存超限、数据格式错误等较为严重却不会经常发生的场景下。



**二、libc++ Exception 实现浅探**

###  

前一节我们验证了C++ Exception在频繁发生异常的情况下会导致程序性能变慢的现象，这一节开始我们尝试去寻找导致这一现象的原因。



首先，Exception机制的实现位于C++标准库中，而由于C语言中没有Exception机制，我们可以尝试将具有throw关键字的由 .cpp编译而来的可重定位二进制文件与由 .c编译得到的包含main函数的二进制进行链接。目的是找出对于throw关键字，libc++为我们最终生成的可执行文件添加了哪些额外函数。

 throw.h:





```cpp
/// throw.hstruct Exception {};
#ifdef __cplusplusextern "C" {#endif
    void raiseException();
#ifdef __cplusplus};#endif
```



throw.cpp：



```cpp
/// throw.cpp#include "throw.h"
extern "C" {    void raiseException() {        throw Exception();    }}
```



raiseException函数只是简单的抛出异常。这里我们使用extern "C"告诉C++编译器，按照C语言的规则去生成临时函数名，目的是为了让生成的可重定位目标文件能够被后续用C语言完成的main函数链接。main.c如下：





```
/// main.c#include "throw.h"
int main() {    raiseException();    return 0;}
```



我们分别编译throw.cpp和main.c：





```
> g++ -c -o throw.o -O0 -ggdb throw.cpp> gcc -c -o main.o -O0 -ggdb main.c
```



直觉来说，我们是可以完成链接的，毕竟函数 raiseException 的定义是完整的。试验一下：





```
> gcc main.o throw.o -o app
Undefined symbols for architecture x86_64:  "__ZTVN10__cxxabiv117__class_type_infoE", referenced from:      __ZTI9Exception in throw.o  NOTE: a missing vtable usually means the first non-inline virtual member function has no definition.  "___cxa_allocate_exception", referenced from:      _raiseException in throw.o  "___cxa_throw", referenced from:      _raiseException in throw.old: symbol(s) not found for architecture x86_64collect2: error: ld returned 1 exit status
```



链接出错了，报错信息看上去好像懂了——应该跟Exception相关，但是很明显我们并没有完全懂——这三个未定义的符号到底是啥？



这三个符号：

__ZTVN10__cxxabiv117__class_type_infoE,___cxa_allocate_exception, ___cxa_throw，均代表了libc++中对应Exception处理机制的入口函数。是编译器在编译时添加的部分，链接时的会在libc++中寻找这三个符号的完整定义。


我们链接时使用的是gcc指令，只会链接libc，C语言中并没有这三个符号的定义，所以我们在链接时才会报错。改用g++链接之后确实没问题了：



```
> g++ main.o throw.o -o app                   > ./appterminate called after throwing an instance of 'Exception'[1]    37016 abort      ./app
```

通过这个demo我们知道，g++确实在编译与链接时做了一些额外的工作，帮我们实现了throw关键字。对于try {...} catch () {...}来说也一样，链接时会链接到libc++中对应的函数实现，我们通过汇编代码再来体会一下：



```
void raise() {    throw Exception();}
void try_but_dont_catch() {    try {        raise();    } catch(Fake_Exception&) {        printf("Running try_but_dont_catch::catch(Fake_Exception)\n");    }
    printf("try_but_dont_catch handled an exception and resumed execution");}
```



对应的汇编（精简过后）：



```
_Z5raisev:    call    __cxa_allocate_exception    call    __cxa_throw
_Z18try_but_dont_catchv:    .cfi_startproc    .cfi_personality 0,__gxx_personality_v0    .cfi_lsda 0,.LLSDA1    ...    call    _Z5raisev    jmp     .L8
```





_Z5raisev对应函数raise函数的实现，从字面意思就可以看出__cxa_allocate_exception是为Exception类型分配空间，__cxa_throw函数的实现位于libc++中，该函数是后续Exception处理机制的入口。z18try_but_dont_catchv的前三行先不管，直接看到call _Z5raisev。到这里也很好理解，如果_Z5raisev执行正常的话，跳到 .L8 程序正常退出。

### 

```
.L8:    leave    .cfi_restore 5    .cfi_def_cfa 4, 4    ret    .cfi_endproc
```



如果try内的代码执行出现问题，那么会执行这段代码（怎么跳过来的我们目前还不知道）：



```cpp
    cmpl    $1, %edx    je      .L5
.LEHB1:    call    _Unwind_Resume.LEHE1:
.L5:    call    __cxa_begin_catch    call    __cxa_end_catch
```



这段汇编，首先比较Exception的类型，如果能够类型匹配，就去执行 .L5，如果不匹配，我们就会顺序执行到 _Unwind_Resume。


很明显，.L5 的部分对应代码的catch关键字，而且 .L5 执行之后也会跳到 .L8，该函数可以正常退出。


Unwind_Resume 应该又是 libc++ 里面的函数了。该函数的作用是去其他栈帧寻找是否有该类型 Exception 的处理函数。


看到这里，其实我们就能够明白了导致之前测试结果的原因：



1.try 后面若不抛出异常，则程序的执行流程不会执行 __cax_throw；


2.__cax_throw是后续的异常判断以及栈回退的入口，不执行该函数则不会对性能有影响；


3.__cax_throw执行后，会逐个栈帧寻找异常处理函数，该过程会导致严重的性能损耗。



**三、总结**



本文简单地对C++ Exception的性能做了一个测试，由测试结果我们进行了合理的推测：C++ Exception背后处理的过程是由libc++中对应的函数实现的，并且对该推测进行了验证。

 实际上C++ Exception的完整实现还有很多深入的细节，感兴趣的同学可以进一步探索。















