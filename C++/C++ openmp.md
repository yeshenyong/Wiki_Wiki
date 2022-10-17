# C++ openmp

https://blog.csdn.net/acaiwlj/article/details/49818965

https://zhuanlan.zhihu.com/p/51173703

## OpenMP

> 真的很牛！而且原生支持

> openmp是由一系列#paragma指令组成，这些指令控制如何多线程的执行程序。另外，即使编译器不支持omp，程序也也能够正常运行，只是程序不会多线程并行运行。

```cpp
int main() {
	vector<int> vecInt(100);
        #pragma omp parallel for
	for (int i = 0; i < vecInt.size(); ++i) {
	    vecInt[i] = i*i;
	}
	return 0;
}
```

> 以上代码会自动以多线程的方式运行for循环中的内容。如果你删除"#pragma omp parallel for"这行，程序依然能够正常运行，唯一的区别在于程序是在单线程中执行。由于C和C++的标准规定，当编译器遇到无法识别的"#pragma"指令时，编译器自动忽略这条指令。所以即使编译器不支持omp，也不会影响程序的编译和运行

#### omp语法

> 所有的omp指令都是以"#pragma omp“开头，换行符结束。并且除了barrier和flush两个指令不作用于代码以外，其他的指令都只与指令后面的那段代码相关，比如上面例子中的for循环

#### parallel编译指示

> parallel告诉编译器开始 一个并行块，编译器会创建一个包含N（在运行时决定，通常为硬件线程数）个线程的线程组，所有线程都运行接下来的语句或者由”{...}"包含的代码块，在这执行结束之后，又回到主线程，创建的这N个线程会被回收

```cpp
#pragma omp parallel
{
    cout << "parallel run!!!\n";
}
```

> 以上代码在4双核4线程的cpu的电脑上运行时，输出了4行”parallel run!!!"。即编译器创建了一个包含4个线程的线程组来运行这段代码。在这段代码运行结束后，程序执行回到主线程。GCC编译器的实现方式是在内部创建一个函数，然后将相关的执行代码移至这个函数，这样一来代码块中定义的变量成为了线程的局部变量，互不影响。而ICC的实现方式是利用fork()来实现。
>
> 线程之间共享的变量是通过传递引用或者利用register变量来实现同步的，其中register变量在代码执行结束之后或者在flush指令调用时进行同步。
>
> 我们也可以利用if条件判断来决定是否对后续的代码采用并行方式执行，如：

```cpp
extern int parallelism_enabled;
#pragma omp parallel for if(parallelism_enabled)
for(int c=0; c<n;++c)
    handle(c);
// 在这个例子中，如果parallelism_enabled为false，那么这个for循环只会由一个线程来执行

```



#### Schedule指令

Schedule指令提供对for指令中线程调度更多的控制能力。它有两种调度方式：static和dynamic

在dynamic调度方式中，还可以指定每次索取的迭代值数量。如

```cpp
#pragma omp parallel  for schedule(dynamic，3)
for (int i = 0; i < 10; ++i) {
   printf("%d ", i);
}
// 在这个例子中，每个线程每次都索取3个迭代值。执行完之后，再拿3个迭代值，直到for循环所有迭代值都运行结束。在最后一次索取的结果有可能不足3个
```







OpenMP的指令有以下一些：**（常用的已标黑）**

- **parallel**，用在一个代码段之前，表示这段代码将被多个线程并行执行
- **for**，用于for循环之前，将循环分配到多个线程中并行执行，必须保证每次循环之间无相关性。
- **parallel for**， parallel 和 for语句的结合，也是用在一个for循环之前，表示for循环的代码将被多个线程并行执行。
- sections，用在可能会被并行执行的代码段之前
- parallel sections，parallel和sections两个语句的结合
- critical，用在一段代码临界区之前
- single，用在一段只被单个线程执行的代码段之前，表示后面的代码段将被单线程执行。
- flush，
- **barrier**，用于并行区内代码的线程同步，所有线程执行到barrier时要停止，直到所有线程都执行到barrier时才继续往下执行。
- atomic，用于指定一块内存区域被制动更新
- master，用于指定一段代码块由主线程执行
- ordered， 用于指定并行区域的循环按顺序执行
- threadprivate, 用于指定一个变量是线程私有的

























