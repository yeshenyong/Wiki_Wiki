# Workflow 阅读笔记



### workflow 安装

```sh
git clone https://github.com/sogou/workflow
cd workflow
make
mkdir build
cd build
cmake ..
make
sudo make install
cd tutorial
make
```

编译指令

```sh
clang++ tutorial-01-wget.cc -std=c++2a -o a.out -lworkflow
```



任务序列

### SeriesWork

由（series）控制

总之，如果你想在一个任务之后启动下一个任务，一般是使用push_back操作来完成（还有些情况可能要用到push_front）。
而series_of()则是一个非常重要的调用，是一个不属于任何类的全局函数。其定义和实现在[Workflow.h](https://github.com/sogou/workflow/blob/master/src/factory/Workflow.h#L140)里：

```c++
static inline SeriesWork *series_of(const SubTask *task)
{
    return (SeriesWork *)task->get_pointer();
}
```

任何task都是SubTask类型的派生。而任何运行中的task，一定属于某个series。通过series_of调用，得到了任务所在的series。
而push_back是SeriesWork类的一个调用，其功能是将一个task放到series的末尾。类似调用还有push_front。本示例中，用哪个调用并没有区别。

```c++
class SeriesWork
{
    ...
public:
    void push_back(SubTask *task);
    void push_front(SubTask *task);
    ...
}
```

SeriesWork类在我们整个体系中，扮演重要的角色。在下一个示例中，我们将展现SeriesWork更多的功能。

​	之前两组示例中，我们直接调用task->start()启动第一个任务。task->start()操作实际的工作方法是，
先创建一个以task为首任务的SeriesWork，再启动这个series。在[WFTask.h](https://github.com/sogou/workflow/blob/master/src/factory/WFTask.h)里，可以看到start的实现：

```C++
template<class REQ, class RESP>
class WFNetWorkTask : public CommRequest
{
public:
    void start()
    {
        assert(!series_of(this));
        Workflow::start_series_work(this, nullptr);
    }
    ...
};
```

​	我们想给series设置一个callback，并加入一些上下文。所以我们不使用任务的start接口，而是自己创建一个series。
​	SeriesWork不能new，delete，也不能派生。只能通过Workflow::create_series_work()接口产生。在[Workflow.h](https://github.com/sogou/workflow/blob/master/src/factory/Workflow.h)中，
通常是用这个调用：

```c++
using series_callback_t = std::function<void (const SeriesWork *)>;

class Workflow
{
public:
    static SeriesWork *create_series_work(SubTask *first, series_callback_t callback);
};
```

在示例代码中，我们的用法是：

```C++
struct tutorial_series_context
{
    std::string http_url;
    std::string redis_url;
    size_t body_len;
    bool success;
};
...
struct tutorial_series_context context;
...
SeriesWork *series = Workflow::create_series_work(http_task, series_callback);
series->set_context(&context);
series->start();
```

​	之前的示例，我们用task里的void *user_data指针保存上下文信息。但这个示例中，我们把上文信息放在series里，
这么做显然更合理一些，series是完整的任务链，所有任务都能得到并修改上下文。
​	series的callback函数在series所有任务被执行完之后调用，在这里，我们简单的用一个lamda函数，打印运行结果并唤醒主线程。







转载自：https://mp.weixin.qq.com/s/kt2baYRJJHl9PCvY3FCU-Q

workflow的关于Task的架构图：



![图片](https://mmbiz.qpic.cn/mmbiz_jpg/JeibBY5FJRBGD0bILQ8gtQd2q7p5icRiaTegiafS1icpdUz5vWA55XVfZOUbqr5rUhlxGJEfSHoDRCLCice9fRJaNQzg/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1&wx_co=1)



再简单的贴一个定时器Task的实现代码给大家看看：



项目里所有的Task都通过工厂创建：

```c++
static WFTimerTask *create_timer_task(const std::string& timer_name,                                          unsigned int microseconds,timer_callback_t callback);
```





​	乍一看可能感觉非常麻烦，为什么实现一个普通的定时功能会搞这么多继承关系，但你真正看了源码后就会发现，项目抽象出的所有Task，比如计数器Task、文件IOTask、网络Task、MySQLTask等，都是通过这种SubTask、XXXRequest、XXXSession的形式来实现，后期再来个XXXTask可以很方便的扩展，这才是优秀项目该有的架构，真的佩服。



**小总结**

**接口的设计**：项目的接入极其简单，几行代码就可搭建个client或者server，几行代码也可构建出简单的任务流图，可用于处理复杂的业务逻辑；



**架构的设计**：项目中的各种类是如何派生的，作者的设计思路是怎么样的；



**网络通信**：项目没有使用任何网络框架，而是使用网络裸接口进行网络通信，我们都知道在大型项目中使用网络裸接口进行网络通信需要处理很多异常条件，这里值得学习一波；



**任务流的封装**：为什么可以动态的构建任务流的串并联图，并在项目内部灵活的调度呢？



**文件I/O**：项目号称内部文件I/O操作比标准调用性能还好，它是怎么做到的？



**内存的管理**：项目没有使用任何智能指针，却能管理好内存问题，这是个技术活，当然，也得益于这优秀的架构设计。



### **优秀的系统设计**

在作者的设计理念中：程序 = 协议 + 算法 + 任务流。



**协议：**就是指通用的网络协议，比如http、redis等，当然还可以自定义网络协议，这里只需要提供序列化和反序列化函数就可以达到想要的效果。



**算法：**workflow提供了一些通用的算法，比如sort、merge、reduce等，当然还可以自定义算法，用过C++ STL的朋友应该都知道如何自定义算法吧，在workflow中，任何复杂的计算都应该包装成算法。



**任务流：**我认为这应该就是整个系统设计的核心，通过任务流来抽象封装实际的业务逻辑，就是把`开发好的协议`和`算法`组成一个任务流程图，然后调度执行这个图。



### **任务流**

这里多聊聊任务流，在workflow中，一切业务逻辑皆是任务，多个任务会组成任务流，任务流可组成图，这个图可能是串联图，可能是并联图，也有可能是串并联图，类似于这种：

![图片](https://mmbiz.qpic.cn/mmbiz_png/JeibBY5FJRBGc1KoSp59UoeSokw1YoSjpO99WJlu3CIMb46Ijk1ngujD2PSaoaMoN7KjLW8MaVl4nJXf6drFk5A/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

也可能是这种复杂的DAG图：

![图片](https://mmbiz.qpic.cn/mmbiz_png/JeibBY5FJRBGc1KoSp59UoeSokw1YoSjpalHPbJcvhrX4cWxTJOsVicY4WayY6YFhqcUbcxoycVU3aIEKnoAW5vg/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

图的层次结构可以由用户自定义，框架也是支持动态地创建任务流。



引用作者的一句话：

> ​	如果把业务逻辑想象成用设计好的电子元件搭建电路，那么每个电子元件内部可能又是一个复杂电路。



workflow对任务做了很好的抽象和封装。整个系统包含6种基础任务：通讯、文件IO、CPU、定时器、计数器。



workflow提供了任务工厂，所有的任务都由任务工厂产生，并且会自动回收。



大多数情况下，通过任务工厂创建的任务都是一个复合任务，但用户并不感知。例如一个http请求，可能包含很多次异步过程（DNS，重定向），内部有很多复杂的任务，但对用户来讲，这就是一次简单的通信任务。



哪有什么岁月静好，只不过是有人替你负重前行。workflow的一大特点就是接口暴露的特别简洁，非常方便用户的接入，外部接入如此简单，肯定是将很多组合的逻辑都放在了内部，但其实workflow项目内部代码结构层次非常简洁清晰，感兴趣的朋友可以自己研究研究哈。



### **内存管理机制**

还有就是项目的内存管理机制，workflow整个项目都遵循着谁申请谁释放的原则，内部申请的内存不需要外部显式释放，内部会自动回收内存。



而且整个项目都没有使用shared_ptr，那多个对象共同使用同一块裸内存，workflow是怎么处理的呢？



内部为这种需要共享的对象各自维护一个引用计数，手动incref和decref，至于为什么要这样做，可以看看workflow美女架构师的回答【https://www.zhihu.com/question/33084543/answer/2209929271】。



作者总结了一下：

- shared_ptr是非侵入式指针，一层包一层，而且为了保持shared_ptr覆盖对象整个生命周期，每次传递时都必须带着智能指针模板，使用具有传染性，而且也比较麻烦。
- shared_ptr引用计数的内存区域和数据区域不一致，不连续，缓存失效可能导致性能问题，尽管有make_shared，但还是容易用错。
- 手动为对象做incref和decref，使用起来更灵活，可以明确在引用计数为固定数字时做一些自定义操作，而且方便调试。因为手动管理对象的引用计数，就会要求开发者明晰对象的生命周期，明确什么时候该使用对象，什么时候该释放对象。
- 如果使用shared_ptr可能会激起开发者的惰性，反正也不需要管理内存啦，就无脑使用shared_ptr呗，最后出现问题时调试起来也比较困难。



 **步骤一**

通过工厂方法创建WFClientTask，同时设置callback；

**● 步骤二**

外部调用start()方法，start()中调用Workflow::start_series_work()方法；

**● 步骤三**

start_series_work()中调用SubTask的dispatch()方法，这个dispatch()方法由SubTask的子类CommRequest（WFClientTask的父类）实现；

**● 步骤四**

dispatch()方法在异步操作结束后会触发subtask_done()方法；

**● 步骤五**

subtask_done()方法内会触发done()方法；

**● 步骤六**

done()方法内会触发callback，然后delete this；

**● 步骤七**

内存释放完成。





### Redis 

> 相信经常使用redis的人，对这个接口不会有什么疑问。但必须注意，我们的请求是禁止SELECT命令和AUTH命令的。
> 因为用户每次请求并不能指定具体连接，SELECT之后下一次请求并不能保证在同一个连接上发起，那么这个命令对用户来讲没有任何意义。
> 对数据库选择和密码的指定，请在redis URL里完成。并且，必须是每次请求的URL都带着这些信息。
> 另外，我们的redis client是支持cluster模式的，可以自动处理MOVED和ASK回复并重定向。用户不能自己发送ASKING命令。

























