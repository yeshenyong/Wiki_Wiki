

摘自：https://github.com/sanhuohq/AwesomeCpp/blob/master/new%20Point%E5%92%8Cnew%20Point()



总结：别用，没啥用，还容易搞混



```cpp
#include <iostream>
#include <array>
#include <memory>
using namespace std;


class Point {
public:
	//Point() = default;  //在自定义了下面的构造函数后，如果没有此声明，则不会生成默认构造，直接Point p或p{}会出错，此时想要继续使用默认构造，可以使用=default来声明我还要继续使用你编译器默认声明的东西
	Point(double value) :x(value), y(value) {}
	//Point() = default;
	constexpr Point(const double &xValue = 3.3, const double &yValue = 5.0) noexcept
		: x(xValue), y(yValue){}

	Point(const Point& other)  //自声明了复制构造函数之后，类不会在自动生成复制构造函数。在声明了移动操作之后，则默认复制构造函数会被删除，但使用=default会复活，
	{                          //在声明了复制赋值运算符或者析构函数的条件下，仍然生成复制构造函数的行为已被废弃（不是不允许）。复制赋值运算符和此过程一样。
		x = other.x;
		y = other.y;
	}
	Point(Point&& other) {

	}
	//两种移动操作仅在不包括任何自定义复制、移动或析构存在的情况下才会生成
	constexpr double xValue() const noexcept { return x; }
	constexpr double yValue() const noexcept { return y; }

	inline void setX(double newX) noexcept { x = newX; }
	inline void setY(double newY) noexcept { y = newY; }
private:
	double x, y;
};

int main()
{

	constexpr Point p0(9.4, 25.0);  //只有加了constexpr才能使用constexpr构造函数返回的编译常量对象，此处不加，依旧不能在编译期知道p1的值
	auto del = [](Point *point) {delete point; };
	//std::unique_ptr<Point, decltype(del)> pPoint(new Point[5], del);
	//std::unique_ptr<Point> pPoint = new Point(1.0, 2.0);  //error  不能把裸指针直接赋值给智能指针，因为智能指针的构造函数需要为裸指针创建控制块
	//Point newp = pPoint[0];
	//cout << pPoint[0].xValue() << endl;
	std::shared_ptr<Point> p(new Point());  // 当自定了有默认值的二参数构造函数且默认构造函数不工作，new Point和new Point()都会调用这个构造函数并且使用默认值
											// 当自定了有默认值的二参数构造函数且让默认构造函数工作，此时相同于两个重载的构造函数，且new Point会优先调用默认构造函数，new Point()会调用失败，因为匹配的有两个
        									// 当自定义的二参数构造函数没有默认值时，new Point和new Point()都会调用默认构造函数，但是new Point是随机初始化,new Point()是初始化为0
	//Point p1(); //注意这块相当于声明了一个函数，返回值是Point,函数名为p1，参数列表为空， new Point或者new Point()这两个都是创建新的对象
	std::shared_ptr<Point> pf(new Point(5,6));
	cout << p->xValue() << "\t" << p->yValue() << endl;
	p.reset(new Point(1, 2));
	cout << p->xValue() << "\t" << p->yValue() << endl;
	Point *p2 = new Point(1.2, 3.5);
	p.reset(p2);
	cout << p->xValue() << p->yValue() << endl;
	auto p3 = std::make_unique<int>(5);  //true

}
```





| 当自定了有默认值的二参数构造函数且默认构造函数不工作，new Point和new Point()都会调用这个构造函数并且使用默认值 |
| ------------------------------------------------------------ |
| 当自定了有默认值的二参数构造函数且让默认构造函数工作，此时相同于两个重载的构造函数，且new Point会优先调用默认构造函数，new Point()会调用失败，因为匹配的有两个 |
| 当自定义的二参数构造函数没有默认值时，new Point和new Point()都会调用默认构造函数，但是new Point是随机初始化,new Point()是初始化为0 |











