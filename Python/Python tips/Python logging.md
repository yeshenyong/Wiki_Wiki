### logging 模块

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

在配置日志器日志级别的基础上，在配置下日志输出目标文件和日志格式

```python
LOG_FORMAT = "%(asctime)s - %(levelname)s - %(message)s"
logging.basicConfig(filename='my.log', level=logging.DEBUG, format=LOG_FORMAT)

logging.debug("This is a debug log.")
logging.info("This is a info log.")
logging.warning("This is a warning log.")
logging.error("This is a error log.")
logging.critical("This is a critical log.")
```

此时会发现控制台中已经没有输出日志内容了，但是在python代码文件的相同目录下会生成一个名为'my.log'的日志文件，该文件中的内容为：

```python
2017-05-08 14:29:53,783 - DEBUG - This is a debug log.
2017-05-08 14:29:53,784 - INFO - This is a info log.
2017-05-08 14:29:53,784 - WARNING - This is a warning log.
2017-05-08 14:29:53,784 - ERROR - This is a error log.
2017-05-08 14:29:53,784 - CRITICAL - This is a critical log.
```

在上面的基础上，我们再来设置下日期/时间格式

```python
LOG_FORMAT = "%(asctime)s - %(levelname)s - %(message)s"
DATE_FORMAT = "%m/%d/%Y %H:%M:%S %p"

logging.basicConfig(filename='my.log', level=logging.DEBUG, format=LOG_FORMAT, datefmt=DATE_FORMAT)

logging.debug("This is a debug log.")
logging.info("This is a info log.")
logging.warning("This is a warning log.")
logging.error("This is a error log.")
logging.critical("This is a critical log.")
```

此时会在my.log日志文件中看到如下输出内容：

```python
05/08/2017 14:29:04 PM - DEBUG - This is a debug log.
05/08/2017 14:29:04 PM - INFO - This is a info log.
05/08/2017 14:29:04 PM - WARNING - This is a warning log.
05/08/2017 14:29:04 PM - ERROR - This is a error log.
05/08/2017 14:29:04 PM - CRITICAL - This is a critical log.
```

掌握了上面的内容之后，已经能够满足我们平时开发中需要的日志记录功能。

###  其他说明

几个要说明的内容：

- `logging.basicConfig()`函数是一个一次性的简单配置工具使，也就是说只有在第一次调用该函数时会起作用，后续再次调用该函数时完全不会产生任何操作的，多次调用的设置并不是累加操作。
- 日志器（Logger）是有层级关系的，上面调用的logging模块级别的函数所使用的日志器是`RootLogger`类的实例，其名称为'root'，它是处于日志器层级关系最顶层的日志器，且该实例是以单例模式存在的。
- 如果要记录的日志中包含变量数据，可使用一个格式字符串作为这个事件的描述消息（logging.debug、logging.info等函数的第一个参数），然后将变量数据作为第二个参数*args的值进行传递，如:`logging.warning('%s is %d years old.', 'Tom', 10)`，输出内容为`WARNING:root:Tom is 10 years old.`
- logging.debug(), logging.info()等方法的定义中，除了msg和args参数外，还有一个**kwargs参数。它们支持3个关键字参数: `exc_info, stack_info, extra`，下面对这几个关键字参数作个说明。

关于exc_info, stack_info, extra关键词参数的说明:

- ***exc_info：*** 其值为布尔值，如果该参数的值设置为True，则会将异常异常信息添加到日志消息中。如果没有异常信息则添加None到日志信息中。
- ***stack_info：*** 其值也为布尔值，默认值为False。如果该参数的值设置为True，栈信息将会被添加到日志信息中。
- ***extra：*** 这是一个字典（dict）参数，它可以用来自定义消息格式中所包含的字段，但是它的key不能与logging模块定义的字段冲突。

##### 一个例子：

在日志消息中添加exc_info和stack_info信息，并添加两个自定义的字端 ip和user

```python
LOG_FORMAT = "%(asctime)s - %(levelname)s - %(user)s[%(ip)s] - %(message)s"
DATE_FORMAT = "%m/%d/%Y %H:%M:%S %p"

logging.basicConfig(format=LOG_FORMAT, datefmt=DATE_FORMAT)
logging.warning("Some one delete the log file.", exc_info=True, stack_info=True, extra={'user': 'Tom', 'ip':'47.98.53.222'})
```

输出结果：

```python
05/08/2017 16:35:00 PM - WARNING - Tom[47.98.53.222] - Some one delete the log file.
NoneType
Stack (most recent call last):
  File "C:/Users/wader/PycharmProjects/LearnPython/day06/log.py", line 45, in <module>
    logging.warning("Some one delete the log file.", exc_info=True, stack_info=True, extra={'user': 'Tom', 'ip':'47.98.53.222'}
```

```python
05/08/2017 16:35:00 PM - WARNING - Tom[47.98.53.222] - Some one delete the log file.
NoneType
Stack (most recent call last):
  File "C:/Users/wader/PycharmProjects/LearnPython/day06/log.py", line 45, in <module>
    logging.warning("Some one delete the log file.", exc_info=True, stack_info=True, extra={'user': 'Tom', 'ip':'47.98.53.222'}
```