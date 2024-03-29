# C++ Runtime 库

参考自：https://github.com/sanhuohq/AwesomeCpp/blob/master/C%20Runtime%E5%BA%93.md

# C 运行库

------

1)C运行时库就是 C run-time library，是 C 而非 C++ 语言世界的概念:取这个名字就是因为你的 C 程序运行时需要这些库中的函数.

2)C 语言是所谓的“小内核”语言，就其语言本身来说很小（不多的关键字，程序流程控制，数据类型等）；所以，C 语言内核开发出来之后，Dennis Ritchie 和 Brian Kernighan 就用 C 本身重写了 90% 以上的 UNIX 系统函数，并且把其中最常用的部分独立出来，形成头文件和对应的 LIBRARY，C run-time library 就是这样形成的。

3)随后，随着 C 语言的流行，各个 C 编译器的生产商/个体/团体都遵循老的传统，在不同平台上都有相对应的 Standard Library，但大部分实现都是与各个平台有关的。由于各个 C 编译器对 C 的支持和理解有很多分歧和微妙的差别，所以就有了 ANSI C；ANSI C （主观意图上）详细的规定了 C 语言各个要素的具体含义和编译器实现要求，引进了新的函数声明方式，同时订立了 Standard Library 的标准形式。所以C运行时库由编译器生产商提供。至于由其他厂商/个人/团体提供的头文件和库函数，应当称为第三方 C 运行库（Third party C run-time libraries）。

4)C run-time library里面含有初始化代码，还有错误处理代码(例如divide by zero处理)。你写的程序可以没有math库，程序照样运行，只是不能处理复杂的数学运算，不过如果没有了C run-time库，main()就不会被调用，exit()也不能被响应。因为C run-time library包含了C程序运行的最基本和最常用的函数。

------

运行时库是程序在运行时所需要的库文件，通常运行时库是以LIB或DLL形式提供的。C运行时库诞生于20世纪70年代，当时的程序世界还很单纯，应用程序都是单线程的，多任务或多线程机制在此时还属于新观念。所以这个时期的C运行时库都是单线程的。

　　随着操作系统多线程技术的发展，最初的C运行时库无法满足程序的需求，出现了严重的问题。C运行时库使用了多个全局变量（例如errno）和静态变量，这可能在多线程程序中引起冲突。假设两个线程都同时设置errno，其结果是后设置的errno会将先前的覆盖，用户得不到正确的错误信息。

　　因此，Visual C++提供了两种版本的C运行时库。一个版本供单线程应用程序调用，另一个版本供多线程应用程序调用。多线程运行时库与单线程运行时库有两个重大差别：

　　（1）类似errno的全局变量，每个线程单独设置一个；

　　这样从每个线程中可以获取正确的错误信息。

　　（2）多线程库中的数据结构以同步机制加以保护。

　　这样可以避免访问时候的冲突。



# C++ 运行库和STL 库的区别

一个是必须的（没有main 函数，你怎么活啊，没有if-else，变量声明，你怎么活啊）

一个是需要的时候include 进来再进行编译的