# C++ Exception 机制

​	本文的目的是对C++ Exception进行简单测试与分析。首先对Exception的性能进行评测，探究C++ Exception对程序性能的影响，然后对C++ Exception的实现机制做一个简单探索，让大家明白Exception对程序运行到底产生了哪些影响，进而写出更高质量的代码。



### **Benchmark**

首先我们先通过性能测试直观地感受一下添加Exception对程序性能的影响。

####  参考Investigating the Performance Overhead of C++ Exceptions（网址：https://pspdfkit.com/blog/2020/performance-overhead-of-exceptions-in-cpp/）的测试思路，我们对其测试用例进行改动。简单解释一下我们的测试代码，我们定义一个函数，该函数会根据概率决定是否调用目标函数：





```
const int randomRange = 2;const int errorInt = 1;int getRandom() { return random() % randomRange; }
template<typename T>T testFunction(const std::function<T()>& fn) {    auto num = getRandom();    for (int i{0}; i < 5; ++i) {        if (num == errorInt) {            return fn();        }    }}
```



执行testFunction时，目标函数fn有50%的概率被调用。



```
void exitWithStdException() {    testFunction<void>([]() -> void {        throw std::runtime_error("Exception!");    });}
void BM_exitWithStdException(benchmark::State& state) {    for (auto _ : state) {        try {            exitWithStdException();        } catch (const std::runtime_error &ex) {            BLACKHOLE(ex);        }    }}
```



BM_exitWithStdException用于测试函数exitWithStdException，该函数会抛出一个Exception，然后在BM_exitWithStdException中立刻被catch，catch后我们什么也不做。


类似的，我们设计用于测试ErrorCode模式的代码如下：

```
void BM_exitWithErrorCode(benchmark::State& state) {    for (auto _ : state) {        auto err = exitWithErrorCode();        if (err < 0) {            // handle_error()            BLACKHOLE(err);        }    }}
int exitWithErrorCode() {    testFunction<int>([]() -> int {        return -1;    });
    return 0;}
```



将ErrorCode测试代码放进try{...}catch{...} 测试只进入try是否会对性能有影响：

```
void BM_exitWithErrorCodeWithinTry(benchmark::State& state) {    for (auto _ : state) {        try {            auto err = exitWithErrorCode();            if (err < 0) {                BLACKHOLE(err);            }        } catch(...) {        }    }}
```



利用gtest/banchmark开始我们的测试：

```
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





```
/// throw.hstruct Exception {};
#ifdef __cplusplusextern "C" {#endif
    void raiseException();
#ifdef __cplusplus};#endif
```



throw.cpp：



```
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



```
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















