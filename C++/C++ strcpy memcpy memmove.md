# strcpy、memcpy、memmove

参考自： https://www.cnblogs.com/stonejin/archive/2011/09/16/2179248.htmlhttps://blog.csdn.net/m0_52640673/article/details/114571259

- strcpy 和 memcpy 的区别

strcpy和memcpy都是标准C库函数，它们有下面的特点。 

strcpy提供了字符串的复制。即strcpy只用于字符串复制，并且它不仅复制字符串内容之外，还会复制字符串的结束符。 

已知strcpy函数的原型是：char* strcpy(char* dest, const char* src);
memcpy提供了一般内存的复制。即memcpy对于需要复制的内容没有限制，因此用途更广。

strcpy和memcpy主要有以下3方面的区别。
1、复制的内容不同。strcpy只能复制字符串，而memcpy可以复制任意内容，例如字符数组、整型、结构体、类等。
2、复制的方法不同。strcpy不需要指定长度，它遇到被复制字符的串结束符"\0"才结束，所以容易溢出。memcpy则是根据其第3个参数决定复制的长度。
3、用途不同。通常在复制字符串时用strcpy，而需要复制其他类型数据时则一般用memcpy

- memcpy 和 memmove的区别

我们发现两者传递的参数相同，而两者之不同，我们现在给出： 

**如果将要拷贝到的目标缓存区与原缓存区有重叠（overlap），则我们需要使用memmove函数；其他情况下，则两者皆可。 也就是说，memmove函数支持源缓冲区与目标缓冲区是同一块并且能够处理两者的重叠的情况。**

 **当我们使用memmove函数时，memmove函数其实已经帮我们考虑到了缓冲区是否重叠的问题了，它允许我们把一个字符串内的内容向它自己进行拷贝**。 

而memcpy则没有这么贴心了，memcpy不会对于重叠的情况加以区别 但是值得一提的是，**如果我们使用VS的编译器，memcpy与memmove的实现方式其实是一致的，两者等价 我们可以这么说，memmove是memcpy的升级版，memmove是更加安全的**（对不同的overlap 情况加以区别并解决问题）













