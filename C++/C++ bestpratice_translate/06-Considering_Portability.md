# Considering Portability（考虑可移植性）

## Know Your Types（知道你的类型）

大多数产生警告的可移植性问题都是因为我们不注意类型。标准库和数组使用`size_t`索引。标准容器大小在`size_t`中报告。如果对`size_t`的处理错误，就会产生微妙（奇怪）的潜伏64位问题，只有在开始溢出32位整数的索引之后才会出现这些问题。`char`类型和`unsigned char`类型

http://www.viva64.com/en/a/0010/

## Use The Standard Library（使用标准库）

### `std::filesystem`

C++ 17增加了一个新的文件系统库（`filesystem`），它提供了跨所有支持编译器的可移植文件系统访问

### `std::thread`

C++ 11的线程功能充分兼容 `pthread` or `WinThreads`.

## Other Concerns（其他关注点）

本文中的大多数其他问题最终都回到了可移植性问题上。特别是避免 statics（avoid statics，详情见07.md）