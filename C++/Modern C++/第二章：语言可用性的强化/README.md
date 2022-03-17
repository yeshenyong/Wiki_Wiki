# 第二章：语言可用性的强化

​	当我们声明、定义一个变量或者常量，对代码进行流程控制、面向对象的功能、模板编程等这些都 是运行时之前，可能发生在编写代码或编译器编译代码时的行为。为此，我们通常谈及语言可用性，是 指那些发生在运行时之前的语言行为。



### 2.1 常量

**nullptr**



​	nullptr 出现的目的是为了替代 NULL。在某种意义上来说，传统 C++ 会把 NULL、0 视为同一种东 西，这取决于编译器如何定义 NULL，有些编译器会将 NULL 定义为 ((void*)0)，有些则会直接将其定义 为 0。 

​	C++ 不允许直接将 void * 隐式转换到其他类型。但如果编译器尝试把 NULL 定义为 `((void*)0)`， 那么在下面这句代码中：

```c++
char *ch = NULL;
```

​	没有了 void * 隐式转换的 C++ 只好将 NULL 定义为 0。而这依然会产生新的问题，将 NULL 定义 成 0 将导致 C++ 中重载特性发生混乱。考虑下面这两个 foo 函数：

```c++
void foo(char*);
void foo(int);
```

​	那么 foo(NULL); 这个语句将会去调用 foo(int)，从而导致代码违反直觉。 

​	为了解决这个问题，C++11 引入了 nullptr 关键字，**专门用来区分空指针、0**。

​	而 nullptr 的类型 为 **nullptr_t**，能够隐式的转换为任何指针或成员指针的类型，也能和他们进行相等或者不等的比较。



```c++
#include <iostream>
#include <type_traits>

void foo(char *);
void foo(int);

int main()
{
	if (std::is_same<decltype(NULL), decltype(0)>::value)
		std::cout << "NULL == 0" << std::endl;
	if (std::is_same<decltype(NULL), decltype((void *)0)>::value)
		std::cout << "NULL == (void *)0" << std::endl;
	if (std::is_same<decltype(NULL), std::nullptr_t>::value)
		std::cout << "NULL == nullptr" << std::endl;
	
	foo(0);	// 调用foo(int)
	// foo(NULL);	// 该行不能通过编译
	foo(nullptr);	// 调用foo(char *)
	return 0;
}

void foo(char *) {
    std::cout << "foo(char*) is called" << std::endl;
}
void foo(int i) {
    std::cout << "foo(int) is called" << std::endl;
}
```

```sh
clang++ 2.1.cpp -std=c++2a -o 2.1
```



从输出中我们可以看出，NULL 不同于 0 与 nullptr。所以，请养成直接使用 nullptr 的习惯。



​	此外，在上面的代码中，我们使用了 `decltype` 和 `std::is_same` 这两个属于现代 C++ 的语法，简 单来说，`decltype` 用于类型推导，而 `std::is_same` 用于比较两个类型是否相同，我们会在后面decltype 一节中详细讨论。



`constexpr`

​	C++ 本身已经具备了常量表达式的概念，比如 1+2, 3*4 这种表达式总是会产生相同的结果并且没 有任何副作用。

​	如果编译器能够在编译时就把这些表达式直接优化并植入到程序运行时，将能增加程序 的性能。一个非常明显的例子就是在数组的定义阶段：



```c++
#include <iostream>
#define LEN 10

int len_foo() {
	int i = 2;
	return i;
}

constexpr int len_foo_constexpr() {
	return 5;
}

constexpr int fibonacci(const int n) {
	return n == 1 || n == 2 ? 1 : fibonacci(n - 1) + fibonacci(n - 2);
}

int main()
{
	char arr_1[10];								//合法
	char arr_2[10];								//合法
	
	int len = 10;
	//char arr_3[len];							//非法
	
	const int len_2 = len + 1;
	constexpr int len_2_constexpr = 1 + 2 + 3;
	// char arr4[len_2];						//非法
	char arr_4[len_2_constexpr];				//合法
	
	// char arr_5[len_foo()+5];					//非法
	char arr_6[len_foo_constexpr() + 1];		//合法
	
	std::cout << fibonacci(10) << std::endl;	
	 
	std::cout << fibonacci(10) << std::endl;
	
	return 0;
}

```

​	上面的例子中，char arr_4[len_2] 可能比较令人困惑，因为 len_2 已经被定义为了常量。为什么 char arr_4[len_2] 仍然是非法的呢？这是因为 C++ 标准中数组的长度必须是一个常量表达式，而对 于 len_2 而言，这是一个 const 常数，而不是一个常量表达式，因此（即便这种行为在大部分编译器中 都支持，但是）它是一个非法的行为，我们需要使用接下来即将介绍的 C++11 引入的 constexpr 特性 来解决这个问题；而对于 arr_5 来说，C++98 之前的编译器无法得知 len_foo() 在运行期实际上是返 回一个常数，这也就导致了非法的产生。



注意，现在大部分编译器其实都带有自身编译优化，很多非法行为在编译器优化的加持下会 变得合法，若需重现编译报错的现象需要使用老版本的编译器。



​	C++11 提供了 constexpr 让用户显式的声明函数或对象构造函数在编译期会成为常量表达式，这 个关键字明确的告诉编译器应该去验证 len_foo 在编译期就应该是一个常量表达式。

此外，constexpr 修饰的函数可以使用递归：

```c++
constexpr int fibonacci(const int n) {
	return n == 1 || n == 2 ? 1 : fibonacci(n-1)+fibonacci(n-2);
}
```

从 C++14 开始，constexpr 函数可以在内部使用局部变量、循环和分支等简单语句，例如下面的 代码在 C++11 的标准下是不能够通过编译的：

```c++
constexpr int fibonacci(const int n) {
    if(n == 1) return 1;
    if(n == 2) return 1;
    return fibonacci(n-1) + fibonacci(n-2);
}
```

为此，我们可以写出下面这类简化的版本来使得函数从 C++11 开始即可用：

```c++
constexpr int fibonacci(const int n) {
	return n == 1 || n == 2 ? 1 : fibonacci(n-1)+fibonacci(n-2);
}
```



### 2.2 变量及其初始化

**if/switch 变量声明强化**

​	在传统 C++ 中，变量的声明虽然能够位于任何位置，甚至于 for 语句内能够声明一个临时变量 int，但始终没有办法在 if 和 switch 语句中声明一个临时的变量。例如：

```c++
#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
	std::vector<int> vec = {1, 2, 3, 4};
	
	// 在C++17 之前
	const std::vector<int>::iterator itr = std::find(vec.begin(), vec.end(), 2);
	if (itr != vec.end()) {
		*itr = 3;
	}
	
	// 需要重新定义一个新的变量 
	const std::vector<int>::iterator itr2 = std::find(vec.begin(), vec.end(), 3);
	if (itr != vec.end()) {
		*itr = 4;
	}
	
	// 将输出1, 4, 3, 4
	for (std::vector<int>::iterator element = vec.begin();element != vec.end();element++) {
		std::cout << *element << std::endl;
	} 
		
	return 0;
}
```



​	在上面的代码中，我们可以看到 itr 这一变量是定义在整个 main() 的作用域内的，这导致当我们 需要再次遍历整个 std::vectors 时，需要重新命名另一个变量。C++17 消除了这一限制，使得我们可 以在 if（或 switch）中完成这一操作：

```c++
#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
	std::vector<int> vec = {1, 2, 3, 4};
	
	// 在C++17 之前
	const std::vector<int>::iterator itr = std::find(vec.begin(), vec.end(), 2);
	if (itr != vec.end()) {
		*itr = 3;
	}
	
	// 需要重新定义一个新的变量 
	const std::vector<int>::iterator itr2 = std::find(vec.begin(), vec.end(), 3);
	if (itr != vec.end()) {
		*itr = 4;
	}
	
	// 将输出1, 4, 3, 4
	for (std::vector<int>::iterator element = vec.begin();element != vec.end();element++) {
		std::cout << *element << std::endl;
	} 
	
	// C++ 17 类似于Go
	//	if (const std::vector<int>::iterator itr3 = std::find(vec.begin(), vec.end(), 3); itr3 != vec.end()) {
	//		*itr3 = 4;
	//	} 
	
		
	return 0;
}
```



**初始化列表**

​	初始化是一个非常重要的语言特性，最常见的就是在对象进行初始化时进行使用。在传统 C++ 中， 不同的对象有着不同的初始化方法，例如普通数组、POD （Plain Old Data，即没有构造、析构和虚函 数的类或结构体）类型都可以使用 {} 进行初始化，也就是我们所说的初始化列表。而对于类对象的初始 化，要么需要通过拷贝构造、要么就需要使用 () 进行。这些不同方法都针对各自对象，不能通用。例如：

```c++
#include <iostream>
#include <vector>

class Foo{
	public:
		int value_a;
		int value_b;
		Foo(int a, int b):value_a(a), value_b(b) {} 
};

int main()
{
	// before C++11
	int arr[3] = {1, 2, 3};
	Foo foo(1, 2);
	std::vector<int> vec = {1, 2, 3, 4, 5};
	
	std::cout << "arr[0]: " << arr[0] << std::endl;
	std::cout << "foo:" << foo.value_a << "," << foo.value_b << std::endl;
	for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it) {
		std::cout << *it << std::endl;
	}
		
	return 0;
}
```

​	为了解决这个问题，C++11 首先把初始化列表的概念绑定到了类型上，并将其称之为 `std::initializer_list` ， 允许构造函数或其他函数像参数一样使用初始化列表，这就为类对象的初始化与普通数组和 POD 的初 始化方法提供了统一的桥梁，例如：

```c++
#include <initializer_list>
#include <vector>
#include <iostream>

class MagicFoo {
	public:
		std::vector<int> vec;
		MagicFoo(std::initializer_list<int> list) {
			for (std::initializer_list<int>::iterator it = list.begin(); it != list.end(); ++it) {
				vec.emplace_back(*it);
			}
		}
};

int main()
{
	// after C++11
	MagicFoo magicFoo = {1, 2, 3, 4, 5};
	
	std::cout << "magicFoo: ";
	
	for (std::vector<int>::iterator it = magicFoo.vec.begin(); it != magicFoo.vec.end(); ++it) {
		std::cout << *it << std::endl;
	}
	
		
	return 0;
}
```

这种构造函数被叫做初始化列表构造函数，具有这种构造函数的类型将在初始化时被特殊关照。 

初始化列表除了用在对象构造上，还能将其作为普通函数的形参，例如：

```c++
#include <initializer_list>
#include <vector>
#include <iostream>

class MagicFoo {
	public:
		std::vector<int> vec;
		MagicFoo(std::initializer_list<int> list) {
			for (std::initializer_list<int>::iterator it = list.begin(); it != list.end(); ++it) {
				vec.emplace_back(*it);
			}
		}
		void foo(std::initializer_list<int> list) {
			for (std::initializer_list<int>::iterator it = list.begin(); it != list.end(); ++it) {
				vec.emplace_back(*it);
			}
		}
};

int main()
{
	// after C++11
	MagicFoo magicFoo = {1, 2, 3, 4, 5};
	
	std::cout << "magicFoo: ";
	
	for (std::vector<int>::iterator it = magicFoo.vec.begin(); it != magicFoo.vec.end(); ++it) {
		std::cout << *it << std::endl;
	}
	
	magicFoo.foo({1, 2, 3, 4});
		
	return 0;
}
```

其次，C++11 还提供了统一的语法来初始化任意的对象，例如：

```c++
Foo foo2 {3, 4};
```



**结构化绑定**

​	结构化绑定提供了类似其他语言中提供的多返回值的功能。在容器一章中，我们会学到 C++11 新 增了 std::tuple 容器用于构造一个元组，进而囊括多个返回值。

​	但缺陷是，C++11/14 并没有提供一种 简单的方法直接从元组中拿到并定义元组中的元素，尽管我们可以使用 std::tie 对元组进行拆包，但 我们依然必须非常清楚这个元组包含多少个对象，各个对象是什么类型，非常麻烦。

C++17 完善了这一设定，给出的结构化绑定可以让我们写出这样的代码：

```c++
#include <iostream>
#include <tuple>

std::tuple<int, double, std::string> f() {
	return std::make_tuple(1, 2.3, "456");
}

int main()
{
	auto [x, y, z] = f();
	std::cout << x << ", " << y << ", " << z << std::endl;
		
	return 0;
}
```



### 2.3 类型推倒

​	在传统 C 和 C++ 中，参数的类型都必须明确定义，这其实对我们快速进行编码没有任何帮助，尤 其是当我们面对一大堆复杂的模板类型时，必须明确的指出变量的类型才能进行后续的编码，这不仅拖 慢我们的开发效率，也让代码变得又臭又长。

​	C++11 引入了 auto 和 decltype 这两个关键字实现了类型推导，让编译器来操心变量的类型。这 使得 C++ 也具有了和其他现代编程语言一样，某种意义上提供了无需操心变量类型的使用习惯。

**auto**

一些其他的常见用法：

```c++
auto i = 5; // i 被推导为 int
auto arr = new auto(10); // arr 被推导为 int *
```

从 C++ 20 起，auto 甚至能用于函数传参，考虑下面的例子：

```c++
int add(auto x, auto y) {
	return x+y;
}
```

注意：auto 还不能用于推导数组类型：

```c++
auto auto_arr2[10] = {arr}; // 错误, 无法推导数组元素类型
```



**decltype**

decltype 关键字是为了解决 auto 关键字只能对变量进行类型推导的缺陷而出现的。它的用法和 typeof 很相似：



decltype(表达式)

有时候，我们可能需要计算某个表达式的类型，例如：

```c++
auto x = 1;
auto y = 2;
decltype(x+y) z;
```



​	你已经在前面的例子中看到 decltype 用于推断类型的用法，下面这个例子就是判断上面的变量 x, y, z 是否是同一类型：

```c++
if (std::is_same<decltype(x), int>::value)
	std::cout << "type x == int" << std::endl;
if (std::is_same<decltype(x), float>::value)
	std::cout << "type x == float" << std::endl;
if (std::is_same<decltype(x), decltype(z)>::value)
	std::cout << "type z == type x" << std::endl;
```

其中，std::is_same 用于判断 T 和 U 这两个类型是否相等。输出结果为：

```sh
type x == int
type z == type x
```

**尾返回类型推导**

​	你可能会思考，在介绍 auto 时，我们已经提过 auto 不能用于函数形参进行类型推导，那么 auto 能不能用于推导函数的返回类型呢？还是考虑一个加法函数的例子，在传统 C++ 中我们必须这么写：

```c++
template<typename R, typename T, typename U>
R add(T x, U y) {
	return x+y;
}
```

​	注意：typename 和 class 在模板参数列表中没有区别，在 typename 这个关键字出现之前，都 是使用 class 来定义模板参数的。但在模板中定义有嵌套依赖类型的变量时，需要用 typename 消除歧义

​	这样的代码其实变得很丑陋，因为程序员在使用这个模板函数的时候，必须明确指出返回类型。但事实上我们并不知道 add() 这个函数会做什么样的操作，以及获得一个什么样的返回类型。

​	在 C++11 中这个问题得到解决。虽然你可能马上会反应出来使用 decltype 推导 x+y 的类型，写 出这样的代码：

```c++
decltype(x+y) add(T x, U y)
```

​	但事实上这样的写法并不能通过编译。这是因为在编译器读到 decltype(x+y) 时，x 和 y 尚未被定 义。为了解决这个问题，C++11 还引入了一个叫做**尾返回类型**（trailing return type），利用 **auto 关键 字将返回类型后置**：

```c++
template<typename T, typename U>
auto add2(T x, U y) -> decltype(x+y){
	return x + y;
}
```

​	令人欣慰的是从 C++14 开始是可以直接让普通函数具备返回值推导，因此下面的写法变得合法：

```c++
template<typename T, typename U>
auto add3(T x, U y){
	return x + y;
}
```



可以检查一下类型推导是否正确：

```c++
// after c++11
auto w = add2<int, double>(1, 2.0);
if (std::is_same<decltype(w), double>::value) {
	std::cout << "w is double: ";
}
std::cout << w << std::endl;
// after c++14
auto q = add3<double, int>(1.0, 2);
std::cout << "q: " << q << std::endl;
```

```c++
#include <iostream>
#include <tuple>

template<typename U, typename T>
auto add(U x, T y) -> decltype(x + y) {
	return x + y;
}

template<typename U, typename T>
auto add2(U x, T y) {
	return x + y;
}


int main()
{
	auto w = add<int, double>(1, 2.0);
	if (std::is_same<decltype(w), double>::value) {
		std::cout << "sum is double" << std::endl;
	}
	std::cout << "w = " << w << std::endl;
	
	auto q = add2<int, double>(1.0, 2);
	std::cout << "q = " << q << std::endl;
		
	return 0;
}
```



**decltype(auto)**

decltype(auto) 是 C++14 开始提供的一个略微复杂的用法。





































