# C++ 美妙使用



上班可以使用的好代码



### RAII 妙用之计算函数耗时

我们平时编程过程中不可避免的需要考虑性能，性能里最主要的最常见的应该就是函数耗时了吧，基本上每个开发者都有打印 某个函数耗时的需求，你平时打印函数时间是否使用的是如下方式

```c++
void Func() {
    ...
}
int CalTime() {
    int begin = GetCurrentTime(); // 伪代码
    Func();
    int end = GetCurrentTime();
    cout << "func time is " << end - begin << " s" << endl;
}
```

想计算某个函数的耗时就在函数前后获取时间之后再算差值，丑，麻烦。 这里可以利用 RAII 方式，把函数的生命周期和一个对象绑定，对象创建时候执行函数，对象生命周期结束析构时候函数执行完 毕，这样对象存活的时间就是函数的耗时，见代码

想计算某个函数的耗时就在函数前后获取时间之后再算差值，丑，麻烦。

 这里可以利用 RAII 方式，把函数的生命周期和一个对象绑定，对象创建时候执行函数，对象生命周期结束析构时候函数执行完 毕，这样对象存活的时间就是函数的耗时，见代码：

```

```

**版本一**

`TimeLog.h`

```c++
#pragma once
#include <sys/time.h>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

using namespace std::chrono;

namespace ysy {

namespace timer {
	
class TimerLog {
	
	public:
	TimerLog(const std::string tag) {	// 对象构造时候保存开始时间
	 	m_begin = std::chrono::high_resolution_clock::now();
		m_tag = tag;
	}
	
	void Reset() { m_begin = std::chrono::high_resolution_clock::now(); }
	
	long long Elapsed() {
		return static_cast<long long>(
			duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - m_begin).count()
		);
	}
	
	~TimerLog() {	// 对象析构时候计算当前时间与对象构建时间差 
		auto time = duration_cast<std::chrono::milliseconds>(
			std::chrono::high_resolution_clock::now() - m_begin).count();
		std::cout << "time { " << m_tag << " } " << static_cast<double>(time) << "ms" << std::endl;
	}
	
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_begin;
		std::string m_tag;
};
	
}	// namespace timer

}	// namespace ysy
```



`main.cpp`

```c++
#include "TimeLog.h"
#include <iostream>
#include <thread>
void TestTimerLog() {
    auto func = []() {
        for (int i = 0; i < 5; i++)
        {
            std::cout << "i " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    };

    {
        ysy::timer::TimerLog f("func");
        func();
    }
}

int main()
{
    TestTimerLog();
    return 0;
}
```



这样就可以很方便的打印函数时间，但是这里每次都需要定义一个对象，貌似也不太方便，可以考虑加个宏，如下

版本二

`main.cpp`

加上

```c++
#define CAL_SCOPE_TIME(x) wzq::timer::TimerLog t(x)
```

```c++
#include "TimeLog.h"
#include <iostream>
#include <thread>

#define CAL_SCOPE_TIME(x) ysy::timer::TimerLog t(x)

void TestTimerLog() {
    auto func = []() {
        for (int i = 0; i < 5; i++)
        {
            std::cout << "i " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    };

    {
        CAL_SCOPE_TIME("func time");
        func();
    }
}

int main()
{
    TestTimerLog();
    return 0;
}
```

是不是更方便啦，当然也有可能要计算多个交叉函数的耗时.

```c++
void TestTime() {
    func1();
    func2();
    func3();
    func4();
}
```

这里如果想计算 func1()+func2()+func3()耗时，也想计算 func2()+func3()+func4()耗时，使用上述 RAII 貌似就不太方便了，

这里可以再写两个宏.

```c++
#define CAL_TIME_BEGIN(x) auto begin_##x = ysy::timer::TimerLog::Now();
#define CAL_TIME_END(x) \
cout << "time { " << #x << " } " << ysy::timer::TimerLog::DiffMs(begin_##x, ysy::timer::TimerLog::Now()) << "ms" << endl
```



就可以如下使用，尽管不是特别方便，但是也比最开始介绍的方便一些

```c++
void TestTime() {
    CAL_TIME_BEGIN(func123)
    func1();
    CAL_TIME_BEGIN(func234)
	func2();
    func3();
    CAL_TIME_END(func123)
    func4();
    CAL_TIME_END(func234)
}
```

这样就会输出我们想要的耗时结果。



最终版本

`main.cpp`

```c++
#include "TimeLog.h"
#include <iostream>
#include <thread>

#define CAL_SCOPE_TIME(x) ysy::timer::TimerLog t(x)
#define CAL_TIME_BEGIN(x) auto begin_##x = ysy::timer::TimerLog::Now();
#define CAL_TIME_END(x) \ 
    std::cout << "time { " << #x << " } " << ysy::timer::TimerLog::DiffMs(begin_##x, ysy::timer::TimerLog::Now()) << "ms" << std::endl;

void TestTimerLog() {
    auto func1 = []() {
        for (int i = 0; i < 5; i++)
        {
            std::cout << "i " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    };
    auto func2 = []() {
        for (int i = 0; i < 5; i++)
        {
            std::cout << "i " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    };
    auto func3 = []() {
        for (int i = 0; i < 5; i++)
        {
            std::cout << "i " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    };
    auto func4 = []() {
        for (int i = 0; i < 5; i++)
        {
            std::cout << "i " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    };
    {
        // CAL_SCOPE_TIME("func time");
        CAL_TIME_BEGIN(func123);
        func1();
        CAL_TIME_BEGIN(func234);
        func2();
        func3();
        CAL_TIME_END(func123);
        func4();
        CAL_TIME_END(func234);
    }
}

int main()
{
    TestTimerLog();
    return 0;
}
```

`TimeLog.h`

```c++
#pragma once
#include <sys/time.h>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

using namespace std::chrono;

namespace ysy {

namespace timer {
	
class TimerLog {
	
	public:
	TimerLog(const std::string tag) {	// 对象构造时候保存开始时间
	 	m_begin = std::chrono::high_resolution_clock::now();
		m_tag = tag;
	}
	
	void Reset() { m_begin = std::chrono::high_resolution_clock::now(); }
	
	long long Elapsed() {
		return static_cast<long long>(
			duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - m_begin).count()
		);
	}
	
	static std::chrono::time_point<std::chrono::high_resolution_clock> Now() { return std::chrono::high_resolution_clock::now(); }

	static long long DiffUs(std::chrono::time_point<std::chrono::high_resolution_clock> before,
							std::chrono::time_point<std::chrono::high_resolution_clock> after) {
		return static_cast<long long>(duration_cast<std::chrono::microseconds>(after - before).count());
	}

	static long long DiffMs(std::chrono::time_point<std::chrono::high_resolution_clock> before,
							std::chrono::time_point<std::chrono::high_resolution_clock> after) {
		return static_cast<long long>(duration_cast<std::chrono::milliseconds>(after - before).count());
	}

	~TimerLog() {	// 对象析构时候计算当前时间与对象构建时间差 
		auto time = duration_cast<std::chrono::milliseconds>(
			std::chrono::high_resolution_clock::now() - m_begin).count();
		std::cout << "time { " << m_tag << " } " << static_cast<double>(time) << "ms" << std::endl;
	}

	static long long GetCurrentMs() {
		struct timeval time;
		gettimeofday(&time, NULL);
		return static_cast<long long>(time.tv_sec * 1000) + static_cast<long long>(time.tv_usec / 1000);
	}

	static void ShowCurTime() {
		time_t now = time(0);
		char* dt = ctime(&now);
		std::cout << "cur time is " << dt << std::endl;
		std::cout << "cur ms is " << GetCurrentMs() << std::endl;
	}

	static struct timeval GetCurrentTimeofDay() {
		struct timeval time;
		gettimeofday(&time, NULL);
		return time;
	}
	
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_begin;
		std::string m_tag;
};
	
}	// namespace timer

}	// namespace ysy
```



其中
`函数duration_cast()` 提供了在不同的**时间单位**之间进行转换的功能。

















