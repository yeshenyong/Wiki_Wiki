### 多线程（Python多线程就是菜逼）

普通多线程池例子

```Python
import threading
lock = threading.Lock()
def func(times, name, ret):
    for i in range(times):
        print(name + ' run: ' + str(i))
    ret[name] = name + " finished with " + str(times) + " times printed"
    return

if __name__ == '__main__':

    thread_pool = []
    ret = {}
    th_1 = threading.Thread(target=func, args=[3, 'th_1', ret], name='th_1')
    th_2 = threading.Thread(target=func, args=[5, 'th_2', ret], name='th_2')
    thread_pool.append(th_1)
    thread_pool.append(th_2)

    for th in thread_pool:
        th.start()

    for th in thread_pool:
        th.join()

    print(ret)
```

加互斥锁方式

```python
lock = threading.Lock()
with lock:
	'''
	'''

lock.acquire()
'''
'''
lock.release()
```

结合类（可封装返回值）

```python
class MyThread(threading.Thread):
    def __init__(self, func, args=()):
        super(MyThread, self).__init__()
        self.func = func
        self.args = args
    def run(self):
        self.result = self.func(*self.args)
    def get_result(self):
        try:
            return self.result
        except Exception as e:
            return None
def getThread(thread_num):
    thread_pool = []
    for i in range(thread_num):
        thread_pool.append(MyThread(func, args=(i,))) #threading.Thread(target=func, args=[i], name='th_'+str(i)))
    return thread_pool
```



### 多进程（多线程的替代）

法一

```python
from multiprocessing import Process
import multiprocessing


class MyProcess(Process):
    def __init__(self, target, args):
        Process.__init__(self)
        self.target = target
        self.args = args

    def run(self):
        self.result = self.target(*self.args)


def func(begin, return_dict):
    for i in range(100):
        begin += 1
    return_dict[begin - 100] = begin


def getProcess(thread_num, queue):
    process_pool = []
    for i in range(thread_num):
        process_pool.append(MyProcess(target=func, args=(i*100, queue)))
    return process_pool


if __name__ == '__main__':
    PROCESS = 10
    # 共享变量通信return 值
    manager = multiprocessing.Manager()
    return_dict = manager.dict()
    process_pool = getProcess(PROCESS, return_dict)
    for process in process_pool:
        process.start()
    for process in process_pool:
        process.join()
    print return_dict.values()
```



法二

```python
import time
from multiprocessing import process, Manager, Pool
import multiprocessing as mp
def createPool(proc_num):
    pool = Pool(processes=proc_num)
    return pool


def calculate(x, y):
    for i in range(x + 10000):
        x += y
    print(mp.current_process())
    return x


def run(pool):
    result_list = []
    for i in range(10):
        result_list.append(pool.apply_async(calculate, [i * 10000, 10000]))
    pool.close()
    pool.join()
    for res in result_list:
        print "the result:", res.get()


print("begin at {}".format(time.ctime()))
pool1 = createPool(4)
run(pool1)
print("done at {}".format(time.ctime()))
print()
print("begin at {}".format(time.ctime()))
pool2 = createPool(4)
run(pool2)
print("done at {}".format(time.ctime()))
```





### 反射

hasattr(self, func_name)

getattr(self, func_name)



### pdb 调试

```sh
python -m pdb test.py
```

| 解释                    |                            |
| ----------------------- | -------------------------- |
| **break 或 b 设置断点** | 设置断点                   |
| **continue 或 c**       | 继续执行程序               |
| **list 或 l**           | 查看当前行的代码段         |
| **step 或 s**           | 进入函数                   |
| **return 或 r**         | 执行代码直到从当前函数返回 |
| **exit 或 q**           | 中止并退出                 |
| **next 或 n**           | 执行下一行                 |
| **pp**                  | 打印变量的值               |
| **a**                   | 查看全部栈内变量           |



### atexit



除法保留小数添加

```python
from __future__ import division
```



list 转 set小心了

argsort()函数是**将x中的元素从小到大排列**，**提取其对应的index(索引)，然后输出到y**



### replace

Python replace() 方法把字符串中的 old（旧字符串） 替换成 new(新字符串)，如果指定第三个参数max，则替换不超过 max 次。



### map 函数

map() 函数语法：

```python
map(function, iterable, ...)
```

Python 2.x 返回列表。

Python 3.x 返回迭代器。

```
function -- 函数
iterable -- 一个或多个序列
```

```python
>>> def square(x) :            # 计算平方数
...     return x ** 2
...
>>> map(square, [1,2,3,4,5])   # 计算列表各个元素的平方
[1, 4, 9, 16, 25]
>>> map(lambda x: x ** 2, [1, 2, 3, 4, 5])  # 使用 lambda 匿名函数
[1, 4, 9, 16, 25]

# 提供了两个列表，对相同位置的列表数据进行相加
>>> map(lambda x, y: x + y, [1, 3, 5, 7, 9], [2, 4, 6, 8, 10])
[3, 7, 11, 15, 19]
```



#### pool.map**

```python
import numpy as np
from time import time
from multiprocessing import Process, Queue
import multiprocessing as mp
import random
 
def my_func(x):
    s0 = time()
    res = 0
    for _ in range(x*1000000):
        res += 1
    print(mp.current_process(),'run time:%.3f s, result:%.1f'%(time()-s0,res))
    return res
 
'''
multiprocessing.Pool 只是用来启动多个进程而不是在每个core上启动一个进程。
换句话说Python解释器本身不会去在每个core或者processor去做负载均衡。
这个是由操作系统决定的。如果你的工作特别的计算密集型的话，操作系统确实会分配更多的core，但这也不是Python或者代码所能控制的或指定的。 
multiprocessing.Pool(num)中的num可以很小也可以很大,比如I/O密集型的操作，这个值完全可以大于cpu的个数。 
硬件系统的资源分配是由操作系统决定的，如果你希望每个core都在工作，就需要更多的从操作系统出发了~
这段话转自https://segmentfault.com/q/1010000011117956
'''
def main():
  pool = mp.Pool(processes=mp.cpu_count())
  st = time()
  result = pool.map(my_func, [30, 30, 30, 30])
  print('total run time: %.3f s'%(time()-st))
  print(result)
 
if __name__ == "__main__":
    main()
```





### assert

assert a > 0, b < 0

assert a==0 and b==0 and c==0





#### 转义字符

所有的[ASCII码](https://baike.baidu.com/item/ASCII码)都可以用`\`加数字（一般是8进制数字）来表示。而[C](https://baike.baidu.com/item/C/7252092)中定义了一些字母前加"\"来表示常见的那些不能显示的ASCII字符，如`\0`,`\t`,`\n`等，就称为转义字符，因为后面的[字符](https://baike.baidu.com/item/字符/4768913)，都不是它本来的ASCII字符意思了





### logging 模块（文件夹）

抛砖引玉，由模块延伸至日志系统

**logging 模块的日志级别**

​	logging模块默认定义了以下几个日志等级，它允许开发人员自定义其他日志级别，但是这是不被推荐的，尤其是在开发供别人使用的库时，因为这会导致日志级别的混乱。

| 日志等级（level） | 描述                                                         |
| ----------------- | ------------------------------------------------------------ |
| **DEBUG**         | 最详细的日志信息，典型应用场景是 问题诊断                    |
| **INFO**          | 信息详细程度仅次于DEBUG，通常只记录关键节点信息，用于确认一切都是按照我们预期的那样进行工作 |
| **WARNING**       | 当某些不期望的事情发生时记录的信息（如，磁盘可用空间较低），但是此时应用程序还是正常运行的 |
| **ERROR**         | 由于一个更严重的问题导致某些功能不能正常运行时记录的信息     |
| CRITICAL          | 当发生严重错误，导致应用程序不能继续运行时记录的信息         |

​	开发应用程序或部署开发环境时，可以使用DEBUG或INFO级别的日志获取尽可能详细的日志信息来进行开发或部署调试；应用上线或部署生产环境时，应该使用WARNING或ERROR或CRITICAL级别的日志来降低机器的I/O压力和提高获取错误日志信息的效率。日志级别的指定通常都是在应用程序的配置文件中进行指定的。

> - 上面列表中的日志等级是从上到下依次升高的，即：DEBUG < INFO < WARNING < ERROR < CRITICAL，而日志的信息量是依次减少的；
> - 当为某个应用程序指定一个日志级别后，应用程序会记录所有日志级别大于或等于指定日志级别的日志信息，而不是仅仅记录指定级别的日志信息，nginx、php等应用程序以及这里要提到的python的logging模块都是这样的。同样，logging模块也可以指定日志记录器的日志级别，只有级别大于或等于该指定日志级别的日志记录才会被输出，小于该等级的日志记录将会被丢弃。



logging模块提供了两种记录日志的方式：

- 第一种方式是使用logging提供的模块级别的函数
- 第二种方式是使用Logging日志系统的四大组件

其实，logging所提供的模块级别的日志记录函数也是对logging日志系统相关类的封装而已。

##### logging模块定义的模块级别的常用函数

| 函数                                   | 说明                                 |
| -------------------------------------- | ------------------------------------ |
| logging.debug(msg, *args, **kwargs)    | 创建一条严重级别为DEBUG的日志记录    |
| logging.info(msg, *args, **kwargs)     | 创建一条严重级别为INFO的日志记录     |
| logging.warning(msg, *args, **kwargs)  | 创建一条严重级别为WARNING的日志记录  |
| logging.error(msg, *args, **kwargs)    | 创建一条严重级别为ERROR的日志记录    |
| logging.critical(msg, *args, **kwargs) | 创建一条严重级别为CRITICAL的日志记录 |
| logging.log(level, *args, **kwargs)    | 创建一条严重级别为level的日志记录    |
| logging.basicConfig(**kwargs)          | 对root logger进行一次性配置          |

其中`logging.basicConfig(**kwargs)`函数用于指定“要记录的日志级别”、“日志格式”、“日志输出位置”、“日志文件的打开模式”等信息，其他几个都是用于记录各个级别日志的函数。

##### logging模块的四大组件

| 组件       | 说明                                                         |
| ---------- | ------------------------------------------------------------ |
| loggers    | 提供应用程序代码直接使用的接口                               |
| handlers   | 用于将日志记录发送到指定的目的位置                           |
| filters    | 提供更细粒度的日志过滤功能，用于决定哪些日志记录将会被输出（其它的日志记录将会被忽略） |
| formatters | 用于控制日志信息的最终输出格式                               |

> ***说明：*** logging模块提供的模块级别的那些函数实际上也是通过这几个组件的相关实现类来记录日志的，只是在创建这些类的实例时设置了一些默认值。





### python 反射机制

```python
imp = input(“请输入你想导入的模块名:”)
CC = __import__(imp) 這种方式就是通过输入字符串导入你所想导入的模块 
CC.f1()  # 执行模块中的f1方法

```



上面还存在一点问题:那就是我的模块名有可能不是在本级目录中存放着。有可能是如下图存放方式：

　　![img](https://images2015.cnblogs.com/blog/1136072/201706/1136072-20170612193825415-699956760.png)

　　那么这种方式我们该如何搞定呢?看下面代码:



```python
dd = __import__("lib.text.commons")  #这样仅仅导入了lib模块
dd = __import__("lib.text.commons",fromlist = True)  #改用这种方式就能导入成功
```



```python
class Foo(object):
 
    def __init__(self):
        self.name = 'wupeiqi'
 
    def func(self):
        return 'func'
 
obj = Foo()
 
# #### 检查是否含有成员 ####
hasattr(obj, 'name')
hasattr(obj, 'func')
#判断对象obj是否包含名为name的属性，及func的方法（hasattr是通过调用getattr(ojbect, name)是否抛出异常来实现的） 
 
# #### 获取成员 ####
print(getattr(obj, 'name'))#如果Instance 对象中有属性name则打印self.name的值，否则打印'None'
getattr(obj, 'func','default')#如果有方法method，返回其地址，否则返回default 
print (getattr(obj, 'func', 'default')()) #如果有方法method，运行函数并打印None否则打印default 
 
# #### 设置成员 ####
setattr(obj, 'age', 18)
setattr(obj, 'show', lambda num: num + 1)
 
# #### 删除成员 ####
delattr(obj, 'name')
delattr(obj, 'func')
 
反射代码示例
```



```python
class Foo(object):
    def __init__(self):
        self.name = 'yeshenyong'

    def func(self):
        return 'func'
    def gogo(self):
        return 'gogo'

obj = Foo()

print(hasattr(obj, 'name'))
print(hasattr(obj, 'func'))

print(getattr(obj, 'name'))
print(getattr(obj, 'func', 'default'))

print(getattr(obj, 'func', 'default')())

setattr(obj, 'age', 18)
setattr(obj, 'show', lambda num: num + 1)

print obj.age
print obj.name
print obj.show(1)

delattr(obj, 'name')
delattr(obj, 'gogo')
delattr(obj, 'func')
```



### 垃圾回收机制

python里也同java一样采用了垃圾收集机制，不过不一样的是，python采用的是**引用计数机制**为主，**标记-清除和分代收集**两种机制为辅的策略



#### 引用机制

若此对象无其他对象引用，则立马回收掉

#### 优点：简单、实时（将处理垃圾时间分摊到运行代码时，而不是等到一次回收）

#### 缺点：

1.保存对象引用数会占用一点点内存空间

2.每次执行语句都可能更新引用数，不再使用大的数据结构时，会引起大量对象被回收

3.不能处理循环引用的情况



#### **标记清除解决循环引用**

此方式主要用来处理循环引用的情况，只有容器对象（list、dict、tuple，instance）才会出现循环引用的情况

标记-清扫式垃圾回收器是一种直接的全面停顿算法。简单的说，它们找出所有不可达的对象，并将它们放入空闲列表Free

清扫过程将分为标记阶段和清扫阶段

Python采用了**“标记-清除”(Mark and Sweep)**算法，解决容器对象可能产生的循环引用问题。(注意，只有容器对象才会产生循环引用的情况，比如列表、字典、用户自定义类的对象、元组等。而像数字，字符串这类简单类型不会出现循环引用。作为一种优化策略，对于只包含简单类型的元组也不在标记清除算法的考虑之列)

跟其名称一样，该算法在进行垃圾回收时分成了两步，分别是：

- A）标记阶段，遍历所有的对象，如果是可达的（reachable），也就是还有对象引用它，那么就标记该对象为可达；
- B）清除阶段，再次遍历对象，如果发现某个对象没有标记为可达，则就将其回收。

![img](https://img2018.cnblogs.com/blog/1046062/201903/1046062-20190308212848414-1797660854.png)

#### 处理过程

```
1.将所有容器对象放到一个双向链表中（链表为了方便插入删除），这些对象为0代

2.循环遍历链表，如果被本链表内的对象引入，自身的被引用数-1，如果被引用数为0，则触发引用计数回收条件，被回收掉

3.未被回收的对象，升级为1代
```

缺点：需要扫描整个堆区，时间开销较大

会发生非常明显的卡顿现象（Stop The World）

#### **分代回收**

​	因为循环引用的原因，并且因为你的程序使用了一些比其他对象存在时间更长的对象，从而被分配对象的计数值与被释放对象的计数值之间的差异在逐渐增长。一旦这个差异累计超过某个阈值，则Python的收集机制就启动了，并且触发上边所说到的零代算法，释放“浮动的垃圾”，并且将剩下的对象移动到一代链表。

​	随着时间的推移，一代链表越来越多，多到触发gc阈值，同样会对一代链表进行标记清除操作，然后将剩下活跃对象升为二代

​	在循环引用对象的回收中，整个应用程序会被暂停，为了减少应用程序暂停的时间，Python 通过**“分代回收”(Generational Collection)**以空间换时间的方法提高垃圾回收效率。

​	分代回收是基于这样的一个统计事实，**对于程序，存在一定比例的内存块的生存周期比较短；而剩下的内存块，生存周期会比较长，甚至会从程序开始一直持续到程序结束。生存期较短对象的比例通常在 80%～90% 之间，这种思想简单点说就是：对象存在时间越长，越可能不是垃圾，应该越少去收集。这样在执行标记-清除算法时可以有效减小遍历的对象数，从而提高垃圾回收的速度。**

​	python gc给对象定义了三种世代(0,1,2),每一个新生对象在generation zero中，如果它在一轮gc扫描中活了下来，那么它将被移至generation one,在那里他将较少的被扫描，如果它又活过了一轮gc,它又将被移至generation two，在那里它被扫描的次数将会更少。

​	gc的扫描在什么时候会被触发呢?答案是当某一世代中被分配的对象与被释放的对象之差达到某一阈值的时候，就会触发gc对某一世代的扫描。值得注意的是当某一世代的扫描被触发的时候，比该世代年轻的世代也会被扫描。也就是说如果世代2的gc扫描被触发了，那么世代0,世代1也将被扫描，如果世代1的gc扫描被触发，世代0也会被扫描。

![img](https://img2018.cnblogs.com/blog/1046062/201903/1046062-20190308213942672-108078035.png)

在Python中，主要通过引用计数进行垃圾回收；通过 “标记-清除” 解决容器对象可能产生的循环引用问题；通过 “分代回收” 以空间换时间的方法提高垃圾回收效率



gc 何时触发

#### 何时触发

1.被引用为0时，立即回收当前对象

2.达到了垃圾回收的阈值，触发标记-清除

3.手动调用gc.collect()

4.Python虚拟机退出的时候





### from __future__ import *

​	其实也就几个

​	我们在读代码的时候，总是会看到代码开头会加上from __future__ import *这样的语句。这样的做法的作用就是将新版本的特性引进当前版本中，也就是说我们可以在当前版本使用新版本的一些特性。

如果你想用python2.x体验python3.x的写法，就可以使用from __future__ import print_function来实现

```python
# python 2.x
print "Hello World"
 
# python 3.x
print("Hello World")
```

而这时候如果再使用原来python2.x的标准写法就会报错，

```python
# python 2.x
from __future__ import print_function
print "Hello World"
 
>>> print "Hello World"
  File "<stdin>", line 1
    print "Hello World"
                      ^
SyntaxError: invalid syntax
```

除了print函数，__future__模块还有很多其他功能，

```python
# python 2.x
5/2
>>> 2
 
from __future__ import division
5/2
>>> 2.5
```

```python
# python 2.x
try:
    with open('test.txt', 'w') as f:
    f.write('Hello World')
finally:
    f.close()
 
# 用with替代上述异常检测代码：
from __future__ import with_statement
with open('test.txt', 'w') as f:
    f.write('Hi there!')
```

​	绝对引入（absolute_import）

绝对引入主要是针对python2.4及之前的版本的，这些版本在引入某一个.py文件时，会首先从当前目录下查找是否有该文件。如果有，则优先引用当前包内的文件。而如果我们想引用python自带的.py文件时，则需要使用

```python
from __future__ import absolute_import
```

直观地看就是说”加入绝对引入这个新特性”。说到绝对引入，当然就会想到相对引入。那么什么是相对引入呢?比如说，你的包结构是这样的:

> pkg/
> pkg/init.py
> pkg/main.py
> pkg/string.py

如果你在main.py中写`import string`,那么在Python 2.4或之前, Python会先查找当前目录下有没有string.py, 若找到了，则引入该模块，然后你在main.py中可以直接用string了。如果你是真的想用同目录下的string.py那就好，但是如果你是想用系统自带的标准string.py呢？那其实没有什么好的简洁的方式可以忽略掉同目录的string.py而引入系统自带的标准`string.py`。这时候你就需要`from __future__ import absolute_import`了。这样，你就可以用`import string`来引入系统的标准`string.py`, 而用`from pkg import string`来引入当前目录下的`string.py`了



## Python 常用模块

### numpy

从图中我们可以看出ndarray在存储数据的时候，数据与数据的地址都是连续的，这样就给使得批量操作数组元素时速度更快。

![img](https://bkimg.cdn.bcebos.com/pic/cdbf6c81800a19d8c4ae965b3efa828ba61e463a?x-bce-process=image/watermark,image_d2F0ZXIvYmFpa2UxNTA=,g_7,xp_5,yp_5/format,f_auto)

这是因为ndarray中的所有元素的类型都是相同的，而Python列表中的元素类型是任意的，所以ndarray在存储元素时内存可以连续，而python原生list就只能通过寻址方式找到下一个元素，这虽然也导致了在通用性能方面Numpy的ndarray不及Python原生list，但在科学计算中，Numpy的ndarray就可以省掉很多循环语句，代码使用方面比Python原生list简单的多。

**numpy内置了并行运算功能，当系统有多个核心时，做某种计算时，numpy会自动做并行计算**。

**Numpy底层使用C语言编写，数组中直接存储对象，而不是存储对象指针，所以其运算效率远高于纯Python代码。**

NumPy 数组的维数称为秩（rank），秩就是轴的数量，即数组的维度，一维数组的秩为 1，二维数组的秩为 2，以此类推。

在 NumPy中，每一个线性的数组称为是一个轴（axis），也就是维度（dimensions）。比如说，二维数组相当于是两个一维数组，其中第一个一维数组中每个元素又是一个一维数组。所以一维数组就是 NumPy 中的轴（axis），第一个轴相当于是底层数组，第二个轴是底层数组里的数组。而轴的数量——秩，就是数组的维数。 [3] 

很多时候可以声明 axis。axis=0，表示沿着第 0 轴进行操作，即对每一列进行操作；axis=1，表示沿着第1轴进行操作，即对每一行进行操作。

NumPy 的数组中比较重要 ndarray 对象属性有：

| 属性             | 说明                                                         |
| ---------------- | ------------------------------------------------------------ |
| ndarray.ndim     | 秩，即轴的数量或维度的数量                                   |
| ndarray.shape    | 数组的维度，对于矩阵，n 行 m 列                              |
| ndarray.size     | 数组元素的总个数，相当于 .shape 中 n*m 的值                  |
| ndarray.dtype    | ndarray 对象的元素类型                                       |
| ndarray.itemsize | ndarray 对象中每个元素的大小，以字节为单位                   |
| ndarray.flags    | ndarray 对象的内存信息                                       |
| ndarray.real     | ndarray元素的实部                                            |
| ndarray.imag     | ndarray 元素的虚部                                           |
| ndarray.data     | 包含实际数组元素的缓冲区，由于一般通过数组的索引获取元素，所以通常不需要使用这个属性。 |



```python
z = np.random.uniform(-1., 1., size=[batch_size, noise_dim])
```



### pandas



### absl

```python
from absl import app
from absl import flags
 
FLAGS = flags.FLAGS # 用法和TensorFlow的FLAGS类似，具有谷歌独特的风格。
flags.DEFINE_string("name", None, "Your name.")
flags.DEFINE_integer("num_times", 1,


```



### collections

collections是日常工作中的重点、高频模块，常用类型有：

- 计数器（Counter）
- 双向队列（deque）
- 默认字典（defaultdict）
- 有序字典（OrderedDict）
- 可命名元组（namedtuple）　



## 一句话的Python

```python
word2id = dict()
id2word = dict(zip(word2id.values(), word2id.keys()))
```



https://blog.csdn.net/zzc15806/article/details/81133045

https://www.cnblogs.com/yyds/p/6901864.html

