# Python

Python 是一门动态语言

Python 支持面向过程、面向对象、函数式编程等

继承多态封装

### 面向过程思维

执行者思维

### 面向对象思维

设计者思维

- 封装
  - 隐藏对象的属性和实现细节，只对外提供必要的方法。相当于将“细节封装起来”，只对外暴露“相关调用方法”（私有属性、私有方法的形式封装，没有严格的语法级别“访问符”）
- 继承
  - 继承可以让子类具有父类特性，提高代码的重用性
- 多态
  - 多态指的是同一个方法调用由于对象不同会产生不同的行为。

### 标识符命名规则

约定俗成的命名规则

| 类型       | 规则                                                         | 例子             |
| ---------- | ------------------------------------------------------------ | ---------------- |
| 模块和包名 | 全小写字母，尽量简单。若多个单词之间用下划线                 | math、os、sys    |
| 函数名     | 全小写字母，多个单词之间用下划线隔开                         | phone、my_name   |
| 类名       | 首字母大写，采用驼峰规则。多个单词时每个单词第一个字母大写，其余部分小写 | MyPhone、MyClass |
| 常量名     | 全大写字母，多个单词使用下划线隔开                           | SPEED、MAX_SPEED |





a = 123。运行过程中，解释器先运行右边的表达式，生成一个代表表达式运算结果的对象；然后，将这个对象地址复制给左边的变量



### 删除变量和垃圾回收机制

```python
a=123
del a
```

如果123这个对象没有变量引用了，就会被垃圾回收机制所回收，清空内存空间。



### tqdm 进度条

```python
from tqdm import tqdm
for i in tqdm(range(len(length)))
```



### 链式赋值

```python
x=y=123 equal x=123 y=123
```



### 系列解包赋值

```python
a, b, c=4,5,6 equal to
a = 4
b = 5
c = 6
```

使用系列解包赋值实现变量互换

```python
a,b=1,2
a,b=b,a
print(a,b)
```



### 常量

Python不支持常量，即没有语法规则限制改变一个变量的值。我们只能约定常量的命名规则，以及在程序的逻辑上不对常量的值作出修改。



`divmod` 返回是一个元组

```python
divmod(13, 3)
(4, 1)	# 同时获得商和余数
```





### 整数缓存问题

Python仅仅对比较小的整数对象进行缓存（范围为[-5, 256]）缓存起来

is 用户判断两个变量引用对象是否为同一个

== 用于判断引用变量引用对象的值是否相等，默认调用对象的 ____eq____() 方法

解释器做了一部分优化（范围是[-5, 任意正整数]）

临时缓存



- is 比较两个对象的id 值是否相等，是否指向同一个内存地址
-  == 比较的事两个对象的内容是否相等，值是否相等
- 小数对象[-5, 256]在全局解释器范围内被放入缓存供重复使用
- is 运算符比 == 效率高，**在变量和None 进行比较时，应该使用 is**



### 字符串

字符串的本质是：字符序列。Python的字符串是不可变得，我们无法对原字符做任何修改。但，可以将字符串的一部分复制到新创建的字符串，达到“看起来修改”的效果。

​	Python不支持单字符类型，单字符也是作为一个字符串使用的。



Python3 直接支持unicode，可表示世界上任何语言字符

内置函数`ord()`可以把字符转换对应的unicode码

内置函数`chr()`可以把十进制数学转换成对应的字符



连续三个单引号或者三个双引号，可以帮助我们创建多行字符串

```python
resume = ''' "tom = 123" '''
```



### 转义字符

| 转义字符 | 描述            |
| -------- | --------------- |
| \        | 续行符          |
| \\\      | 反斜杠符号      |
| \\'      | 单引号          |
| \\''     | 双引号          |
| \b       | 退格(Backspace) |
| \n       | 换行            |
| \t       | 横向制表符      |
| \r       | 回车            |



### 字符串复制

```python
a = 'sxt' * 3
```



不换行打印

```python
print("", end='')
print("", end',')
```



从控制台读取字符串

```python
myname = input("请输入名字")
myname
```



### 字符串切片

slice操作

切片slice 操作可以让我们快速的提取子字符串。标准格式为

[起始偏移量 start:终止偏移量 end: 步长 step]



| 操作和说明                                        | 示例                       | 结果      |
| ------------------------------------------------- | -------------------------- | --------- |
| [:] 提取整个字符串                                | “abcdef” [:]               | "abcdef"  |
| [start:] 从start索引开始到结尾                    | "abcdef"[2:]               | "cdef"    |
| [:end] 从头开始直到end - 1                        | "abcdef"[:2]               | "ab"      |
| [start:end] 从start 到 end-1                      | "abcdef" [2:4]             | "cd"      |
| [start: end:step] 从start 提取到end-1，步长是step | "abcdef" [1:5:2]           | ”bd“      |
| "abcdefghijklmno"[-3:]                            | 倒数三个                   | "mno"     |
| ”abcdefghijklmno“[-8, -3]                         | 倒数第八个到倒数第三个     | "jklmno"  |
| ”abcdefghijklmno“[::-1]                           | 步长为负，从右到左反向提取 | onmlkj... |



切片操作时，起始偏移量和终止偏移量不在[0, 字符串长度-1]这个范围，也不会报错。起始偏移量小于0则会当做0，终止偏移量大于”长度-1“会被当成-1.



### split() 分割、join() 合并

split 可以基于分隔符将字符串分割成多个子字符串（存储到列表中）。如果不指定分隔符，则默认使用空白字符（换行符、空格、制表符）



join 的作用和split恰好相反，用于将一系列字符串连接起来

```
a = ['xst', 'xst', 'xst200']
'*',join(a)
'xst*xst*xst200'
```



#### **拼接字符串要点**

​	使用字符串拼接符+，会生成新的字符串对象，因此不推荐使用+来拼接字符串。推荐使用join函数，因为join函数会拼接字符串之前会计算所有字符串的长度，然后逐一拷贝，仅新建一次对象。

```python
for i in range(100000):	
    a += 'sxt'

li = []
for i in range(100000):
    li.append("sxt")
str1 = "".join(li)
```



**join后者，速度快4~60倍，字符串+号一定要少写**



### 字符串驻留机制

​	字符串驻留：仅保存一份且不可变字符串的方法，不同的值被存放在字符串驻留池中，Python支持字符串驻留机制，对于符合标识符规则的字符串（不同Python版本不一样会不一致）回启东字符串驻留机制



### 字符串前加 u、r、b

字符串前加**u**

> u"'我是含有中文字符的字符串'

Unicode 编码

字符串前加**r**

例：r"\n\n\n"	# 表示一个普通生字符串\n\n\n，而不表示换行

作用：去掉反斜杠的转移机制

字符串前加**b**

例：response = b'<h1>hellow world</h1>'	# b''表示是一个bytes 对象

作用：

b"" 前缀表示：后面字符串是bytes 类型

用处：网络编程中，服务器和浏览器只认bytes 类型数据

如：send函数的参数和recv函数的返回值都是bytes类型

附：

在Python3 中，bytes和str的互相转换方式是：

```python
str.encode('utf-8')
bytes.decode('utf-8')
```

### 常用查找方法

| 方法         | 说明                         | 结果 |
| ------------ | ---------------------------- | ---- |
| len(a)       | 字符串长度                   |      |
| a.startswith | 以指定字符串开头             |      |
| a.endswith   | 以指定字符串结尾             |      |
| a.find       | 第一次出现指定字符串的位置   |      |
| a.rfind      | 最后一次出现指定字符串的位置 |      |
| a.count      | 指定字符串出现次数           |      |
| a.isalnum()  | 所有字符全是字母或数字       |      |
| strip()      | 去除首尾指定信息             |      |
| lstrip       | 去除字符串左边指定信息       |      |
| rstrip       | 去除字符串右边指定信息       |      |



### 大小写转换

| 示例         | 说明                                 | 结果 |
| ------------ | ------------------------------------ | ---- |
| a.capitalize | 产生新的字符串，首字母大写           |      |
| a.title      | 产生新的字符串，每个单词都首字母大写 |      |
| a.upper      | 产生新的字符串，所有字符全转成大写   |      |
| a.lower      | 产生新的字符串，所有字符串全转为小写 |      |
| a.swapcase   | 产生新的，所有字母大小写转换         |      |



### 格式排版

center()、ljust()、rjust() 三个函数用于对字符串实现排版

```python
a = "SXT"
a.center(10, "*")
'***SXT****'
a.center(10)
'   SXT    '
a.ljust(10, "*")
'SXT*******'
```



其他方法

1. isalnum()	 是否为字母或数字
2. isalpha()      检测字符串是否有字母组成（含汉字）
3. isdigit() 检测字符串是否只由数字组成
4. isspace() 检测是否为空白符
5. isupper() 是否为大写字母
6. islower() 是否为小写字母



**空白符=> 制表符、换行符、空格**



### format() 基本用法

format 代替以前的%

Python2.6开始

```python
print("{0} = {1}".format("1", "2"))

print("{name} = {age}".format(name=123, age=123))
```

我们可以通过{索引}/{参数名}，直接映射参数值，实现对字符串的格式化，非常方便



**填充与对齐**

填充常跟对齐一起使用

^、<、>分别是居中、左对齐、右对齐，后面带宽度

:号后面带填充的字符，只能是一个字符，不指定的话默认是用空格填充

```python
>>> "{:*>8}".format("245")
'*****245'
>>> “我是{0}, 我喜欢数字{1:*^8}".format(“高琪", "666")
'我是高琪，我喜欢数字**666***'
```



#### == 与 is 的比较

意义

`==` 用来比较两个值是否相等

`is` 用来表示两个变量是否同一个



### 可变字符串

在Python中欧个，字符串属于不可变对象，不支持原地修改，如果需要修改其中的值，只能创建新的字符串对象。但是，经常我们确实需要原地修改字符换，可以使用io.StringIO对象或array模块

```python
import io
s = "hello.txt"
sio = io.StringIO(s)
sio

sio.getvalue()
sio.seek()
sio.write("g")
sio.getvalue()
```



乘法操作

列表、元组等复制 [10, 20, 30] * 3 ==> [10, 20, 30, 10, 20, 30, 10, 20, 30]



Python 不支持自增和自减



### 序列

​	序列是一种数据存储方式，用来存储一系列的数据。在内存中，序列就是一块用来存放多个值的内存空间。

​	由于Python一切皆模块、对象

**序列中存储的整数对象的地址，而不是整数对象的值。**

a = [10, 20, 30]

a存的是列表的地址

列表里面存的是10，20，30的地址

#### 字符串

字符对象的序列



#### 列表

任意对象的序列

列表：可以存储任意数目、任意类型的数据集合、连续的内存空间

| 方法               | 要点     | 描述                              |
| ------------------ | -------- | --------------------------------- |
| list.append(x)     | 增加元素 | 元素x增加到列表list尾部           |
| list.extend(alist) | 增加元素 | 列表alist所有元素加到列表list尾部 |
| list.insert        | 增加元素 | 在列表指定位置index处插入元素x    |

基本语法[]、list()、range() 创建

```python
a = list("gaoqi")
range(10)
a = list(range(10)) -> [0, 1, ..., 9]
```

range() 创建整数列表

range([start, ] end [, step])

```python
list(range(3, -10, -1))
list(range(15, 3, -1))
list(range(3, 15, 2))
```



推导式生成列表

```python
a = [x*2 for x in range(5)]
a = [x*2 for x in range(100) if x%9 == 0] #通过if过滤元素
a = [0, 18,36, ... , 198]
```

列表的元素增加和删除

当列表删除和增加涉及到列表元素的大量移动，效率较低。O(n)

**append() 方法**

​	**原地修改列表对象是真正的列表尾部添加新的元素，速度最快，推荐使用**

**+运算符操作**

​	并不是真正的尾部添加元素，而是创建新的列表对象，将原列表的元素和新列表的元素依次复制到新的列表对象中。这样，会涉及大量的复制操作，对于操作大量元素不建议使用

**extend() 方法**

将目标列表的所有元素添加到本列表的尾部，属于原地操作，不创建新的列表对象。

**insert() 插入元素**

造成大量复制O(n)

remove()、pop()、del() 删除非尾部元素都会发生后面元素的移动

乘法扩展

```python
a= ['sxt', 100]
b = a*3
b
['sxt', 100, 'sxt', 100, 'sxt', 100]
```



**index() 获得指定元素首次出现的索引位置。语法是: index(value, [start, [end]])**

start、end 指定了搜索范围



**count() 获得指定元素在列表总出现次数**



**成员资格判断**

in 判断



**切片操作**

slice操作可以让我们快速的提取子列表或修改。

标准格式为：[起始偏移量 start:终止偏移量 end:[步长 step]]

步长省略时顺便可以省略第二个冒号



列表的遍历

```python
for obj in listObj:
	print(obj)
```



**列表排序**

修改原列表，不建新列表的排序

```python
a.sort() # 默认升序排序

a.sort(reverse=True) # 降序排列

import random
random.shuffle(a)

```



建新列表排序

sorted() 排序

```python
a = sorted(a) # 默认升序
a = sorted(a, reverse=True)
```



**reversed() 返回迭代器**

时间换空间

reversed() 不对原列表做任何修改，知识返回一个逆序排列的迭代器对象



**max、min、sum**





Python列表大小可变，根据需要随时增加或缩小

​	字符串和列表都是序列类型，一个字符串是一个字符序列，一个列表是任何元素的序列。所有二者用法几乎一模一样.





#### 元组

tuple

列表属于可变序列，可以任意修改列表中元素。

元组属于不可变序列，不能修改元组中的元素。

元祖没有增删改元素方法。

支持一下操作:

- 索引访问
- 切片操作
- 连接操作
- 成员关系操作
- 比较运算操作
- 计数：元素长度、最大值最小值



**tuple创建**

```python
tuplea = ()

tuplea = tuple(元素对象)

tuplea = (20, ) # 一定逗号 
```



切片完还是它本身

列表和元组本质区别不能修改

​	**列表关于排序的方法list.sorted() 是修改原列表，元组没有该方法。如果要对元组排序，只能使用内置函数sorted(tupleObj)，并生成新的列表对象**



​	**zip(列表1，列表2，...) 将多个对象对应位置的元素组成为元组，并返回这个zip对象**

```python
a = [10, 20, 30]
b = [40, 50, 60]
c = [70, 80, 90]
d = zip(a, b, c)
list(d)
[(10, 40, 70), (20, 50, 80), (30, 60, 90)]
```



序列解包

```python
x, y, z = (20, 30, 10)
[a, b, c] = [10, 20, 30]
```



生成器推导式创建元祖

s = (x*2 for x  in range(5))

​	我们通过生成器对象，转换成列表或者元祖。也可以使用生成器对象的____next____() 方法来进行遍历，或者直接作为迭代器对象来使用。不管什么方式使用，**元素访问结束后，如果需要重新访问其中的元素，必须重新创建该生成器对象。**



```python
s = (x*2 for x  in range(5))
s
tuple(s)
list(s) # 只能放问一次元素，第二次就为空。需要再生成一次
s
```



```python
s = (x*2 for x in range(5))
s.__next__()
s.__next__()
```



元组总结

- 元组的核心特点是：不可变序列
- 元组的访问和处理速度比列表快
- 与整数和字符串一样，元组可以作为字典的键，列表则永远不能做为字典的键使用



#### 字典

字典是“键值对” 的无序可变序列

可以通过{}、dict() 来创建字典对象

列表、字典、集合这些可变对象不能作为键，并键不可重复



```python
for x in d.values():
	print x
for x in d.keys():
	print x
for x, y in d.items():
	print x, y
```



### setdefault

```python
color_idx = {}
color_idx.setdefault(Y[i][0], [])
```



##### **通过zip() 创建字典对象**

```python
k = ['name', 'age', 'job']
v = ['gaoqi', 18, 'teacher']
d = dict(zip(k, v))
d
id2word = dict(zip(word2id.values(), word2id.keys()))
```

```python
a = dict([("name", "gaoqi"), ("age", 18)])
>>> a
{'name': 'gaoqi', 'age': 18}
```

##### fromkeys

通过fromkeys 创建值为空的字典

```python
a = dict.fromkyes(['name', 'age', 'job'])
>>> a
{'name':None, 'age':None, 'job':None}
```



​	通过get() 方法获得"值"。推荐使用，优点是：指定键不存在，返回None；也可以设定指定键不存在时默认返回的对象。推荐使用get() 获取"值对象"

```python
a.get('name')
a.get('sex', '一個男人')
a.get('name', '不存在')
```



列出所有键值对

a.items()

遍历

```python
for k, v in a.items():
	print k
	print v
```



列出所有的键，列出所有的值

```python
a.keys()
# put in list
b.values()
# put in list
```



**键值对的个数**

len()



检测一个"键"是否在字典中

```python
"name" in a

True
```



**添加、修改、删除**

添加

a['address']  = 123



​	update() 将新字典中所有键值对全部添加到旧字典对象上。如果key有重复，则直接覆盖。

```python
a = {'name':'yeshenyong'}
b = {'name':'hazayan'}
a.update(b)
>>> a
{'name', 'hazayan'}
```

删除

del() 方法、clear() 删除所有键值对、pop() 删除指定键值对并返回对应“值对象”

```python
a = {'name':'gaoqi', 'age':18, 'job':nono}
del(a['name'])
{'age':18, 'job':nono}
b = a.pop('age')
>>> b
18
```



**popitem()**

​	随机删除和返回该键值对。字典是“无需可变序列”，因此没有第一个元素、最后一个元素的概念；popitem 弹出随机的项，因为字典没有“最后的元素”或者其他有关顺序的概念。若想一个个移除并处理项，这个方法很有效

```python
a = {'name':'gaoqi', 'age':18, 'job':nono}
a.popitem()
('job', 'nono')
a
a.popitem()
('age', 18)
a.popitem()
('name', 'gaoqi')
```



字典序列解包

```python
s = {'name':'gaoqi','age':18,'job':'teacher'}
name, age, job = s # 默认对键进行操作
>>> name
'name'
name. age, job = s.items() # 对键值对进行操作
>>> name
('name', 'gaoqi')
name, age, job = s.values() # 对值进行操作
>>> name
'gaoqi'
```



字典核心底层原理

哈希表bucket

总结：

1. 键必须可hash
   1. 数字、字符串、元组都是可散列的
   2. 自定义对象需要支持下面三点
      1. 支持hash() 函数
      2. 支持通过____eq____() 方法检测相等性
      3. 若 a == b 为真，则hash(a) == hash(b) 也为真
2. 字典在内存中开销巨大，典型的空间换时间
3. 键查询速度很快
4. 往字典里面添加新建可能导致扩容，导致hash表中键的次序变化







#### 集合

本质上就是字典

无值的字典

```python
a = {1, 3, 'sxt'}
b = {'he', 'it', 'sxt'}
a | b
a & b
a - b
a.union(b)
a.intersection(b)
a.differencce(b)
```



赋值语句不可出现在if-else 中

三元条件运算符

print(num if int(num) < 10 else “数字太大")



### 优化手段

for 循环尽量提取局部变量，让寄存器记住

其他优化手段：

- 连接多个字符串，使用join() 而不使用+
- 列表进行元素插入和删除，尽量在列表尾部操作



**使用zip() 迭代**

多个列表、元组并行遍历，并在最短的遍历完停止



### 推导式创建序列



避免冗长代码



#### 列表推导式

```python
[x for x in range(1, 5)]
[x*2 for x in range(1, 20) if x%5==0]
[(row, col) for row in range(1, 10) for col in range(1, 10)]
```



#### 字典推导式

```python
my_text = 'i love you, i love sxt, ilove gaoqi'
char_count = {c:my_text.count(c) for c in my_text}
print(char_count)
```



#### 集合推导式

```python
{x for x in nrage(1, 100) if x%9 == 0}
```



#### 生成器推导式（生成元组）

​	元组没有推导式，只产生一个生成器对象，一个生成器智能云算一次。第一次迭代可以得到数据，第二次迭代发现数据已经没有了。



### 函数

def.____doc____

```python
>>> def initialize1(a, b):
...   '''u baby'''
...   return None
...
>>> print(initialize1.__doc__)
u baby
```





#### 返回值

return 返回值要点：

- 如果函数体包含return 语句，则结束函数执行返回值
- 如果函数体不包含return 语句，则返回None值
- 要返回多个返回值，使用列表，元组，字典，集合将多个值“存起来”即可



打印局部值和全局值

```python
def one():
	print(locals())
	print(globals())
```



#### Python 传参



**传递可变对象的时候，为引用传递（列表、字典、集合）**

**传递不可变对象的时候，也是引用传递，不过由于不可变对象无法修改，系统会新建一个对象（int、float、字符串、元组、布尔值）**



位置参数

默认值参数

命名参数

```python
def test(a, b, c=10):
	print(test)
test(10, 8, 10)
test(10, 8)
test(b = 10, c = 20, a = 9)
```



##### 可变参数

可变参数指的是：可变数量的参数。分两种情况

1. *param（一个星号），将多个参数收集到一个“元组“ 对象中
2. **param（两个星号），将多个参数收集到一个”字典“对象中

```python
>>> def f1(a, b, *c):
...   print(a, b, c)
...
>>> f1(10, 9, 12,121,1221)
10 9 (12, 121, 1221)
>>> def f2(a, b,**c):
...   print(a, b,c)
...
>>> f2(10, 12, name='ye', age=18)
10 12 {'name': 'ye', 'age': 18}
```



强制命名参数

带星号的可变参数后面添加新的参数，必须是强制命名参数

```python
def f1(*a, b, c):
	print(a, b, c)
	
f1(2, b=3, c=4)
```



#### 嵌套函数(内部函数)

嵌套函数：

​	在函数内部定义的函数

嵌套函数定义

```python
def f1():
	print("f1 running")
	def f2():
		print("f2 running")
		
	f2()
f1()
```

1. 封装隐藏
2. 贯彻DRY（don't repeat yourself）
3. 闭包



#### nonlocal 关键字

nonlocal 用来声明外部的局部变量

global 	用来声明全局变量

```python
def f1():
	print("f1 running")
    b = 10
	def f2():
        nonlocal b
        print(b)
        b=20
        print(b)
		print("f2 running")
		
	f2()
f1()
```





### 浅拷贝和深拷贝



内置函数：copy（浅拷贝）、deepcopy（深拷贝）

浅拷贝：不拷贝子对象的内容，只是拷贝子对象的引用

深拷贝：会连值对象的内存也全部拷贝一份，对子对象的修改不会影响源对象



浅拷贝.py

```python
>>> import copy
>>> a = [10, 20, [5, 6]]
>>> b = copy.copy(a)
>>> print("a:", a)
a: [10, 20, [5, 6]]
>>> print("b:", b)
b: [10, 20, [5, 6]]
>>> b.append(30)
>>> b[2].append(7)
>>> print("浅拷贝")
浅拷贝
>>> print("a:", a)
a: [10, 20, [5, 6, 7]]
>>> print("b:", b)
b: [10, 20, [5, 6, 7], 30]
```

深拷贝.py

```python
>>> import copy
>>> a = [10, 20, [5, 6]]
>>> b = copy.deepcopy(a)
>>> print("a:", a)
a: [10, 20, [5, 6]]
>>> print("b:", b)
b: [10, 20, [5, 6]]
>>> b.append(30)
>>> b[2].append(7)
>>> print("深拷贝")
深拷贝
>>> print("a:", a)
a: [10, 20, [5, 6]]
>>> print("b:", b)
b: [10, 20, [5, 6, 7], 30]
```



传递不可变对象时。不可变对象里面包含的子对象是可变的。则方法内修改这个可变对象，原对象也发生变化

```python
>>> a = (10, 20, [5,6])
>>> print("a:", id(a))
a: 2043885258456
>>> def test01(m):
...   print("m:", id(m))
...   m[2][0] = 888
...   print(m)
...   print("m:", id(m))
...
>>> test01(a)
m: 2043885258456
(10, 20, [888, 6])
m: 2043885258456
>>> print(a)
(10, 20, [888, 6])
```



#### 对象的浅拷贝和深拷贝

- 变量赋值操作
- 浅拷贝
- 深拷贝



```python
import copy
class phone:
    def __init__(self, cpu, screen):
        self.cpu = cpu
        self.screen = screen
class cpu:
    pass
class screen:
    pass
c1 = cpu()
s1 = screen()
m1 = phone(c1, s1)
# 赋值
m2 = m1
print(m1, m1.cpu, m1.screen)
print(m2, m2.cpu, m2.screen)
# 浅拷贝
m2 = copy.copy(m1)
print(m1, m1.cpu, m1.screen)
print(m2, m2.cpu, m2.screen)
# 深拷贝
m2 = copy.deepcopy(m1)
print(m1, m1.cpu, m1.screen)
print(m2, m2.cpu, m2.screen)
>>
<__main__.phone object at 0x000001B0D1124208> <__main__.cpu object at 0x000001B0D1106F88> <__main__.screen object at 0x000001B0D1106FC8>
<__main__.phone object at 0x000001B0D1124208> <__main__.cpu object at 0x000001B0D1106F88> <__main__.screen object at 0x000001B0D1106FC8>
<__main__.phone object at 0x000001B0D1124208> <__main__.cpu object at 0x000001B0D1106F88> <__main__.screen object at 0x000001B0D1106FC8>
<__main__.phone object at 0x000001B0D1124088> <__main__.cpu object at 0x000001B0D1106F88> <__main__.screen object at 0x000001B0D1106FC8>
<__main__.phone object at 0x000001B0D1124208> <__main__.cpu object at 0x000001B0D1106F88> <__main__.screen object at 0x000001B0D1106FC8>
<__main__.phone object at 0x000001B0D1124F88> <__main__.cpu object at 0x000001B0D1149748> <__main__.screen object at 0x000001B0D1149708>
```



### lambda 表达式

声明匿名函数

```python
f = lambda a,b,c:a+b+c
print(f)
print(f(2,3,4))
g = [lambda a:a*2, lambda b:b*3, lambda c:c*4]
print(g[0](6), g[1](7), g[2](8))
```

​	

#### eval() 函数

将字符串str当成有效的表达式来求值并返回计算结果

语法：eval(source[, globals[, locals]]) -> value

参数：

- source：一个Python表达式或函数compile() 返回的代码对象
- globals：可选。必须是dictionary
- locals：可选。任意映射对象

动态语言

​	静态语言不可能有eval类似的函数，EVAL是动态语言的东西，可以使用LUA脚本来实现

```python
s = "print('abcde')"
eval(s)

a = 10
b = 20
c = eval("a+b")
print(c)

dict1 = dict(a=100, b=200)
d = eval("a+b", dict1)
print(d)
```





### 异常处理

- 概念

​		异常就是不正常，当Python检测到一个错误时，解释器就无法继续执行下去了，反而出现了一些错误的提示，这就是所谓的异常



​	正常解决一堆 `if-else ` 覆盖，代码逻辑复杂，业务重心偏移

解决方法最好：异常处理方案（目的使代码编程更简单）

```python
try:
	可能出现问题的代码
except:
	如果出现问题，会执行该代码块
```

```python
a = input("input a")
b = input("input b")

try:
    a = int(a)
    b = int(b)
    c = a / b
    print(c)
except:
    print('error')
print('123')
```



#### 多个 except

> except 异常1:
>
> except 异常2:
>
> 多个异常顺序要求

若没有捕获异常，则报错（可用父类Exception 捕获）

```python
a = input("input a")
b = input("input b")

try:
    a = int(a)
    b = int(b)
    c = a / b
    print(c)
except ValueError:
    print('error')
except ZeroDivisionError:
    print('error1')
except Exception:
    print('其他异常')
print('123')
```



第二种解决方式（放元组之间）

> expect 异常 as e:
>
> ​	print(type(e))
>
> ​	# 错误信息
>
> ​	print(a.args)
>
> ​	print("遇到异常")

使用元组存储多个异常的时，多个异常之间没有顺序要求

```python
except (异常1， 异常2， 异常3) as e:
	print(type(e))
	print('遇到异常')
```



#### try-except-else-finally

```python
try:
except 异常 as 变量:
else:
	没有异常，执行的代码
finally:
	最终一定要执行的代码
```

案例：将一些字符串数据写入到文件中

```python
# 这里有个小坑，open如果由于权限不足报错被try-except 捕获，到最后的finally 会报错(文件压根没打开)
try:
    fd = open('read.txt', 'w', encoding='utf-8')
    fd.write("yeye")
    fd.write("ysys")
    fd.write([1, 2, 3])
except Exception as e:
    print(e.args)
else:
    print("没有异常")
finally:
    # 最后一定要确保执行的代码
    fd.close()
    print("关闭文件蟹蟹使用")
```



#### 异常处理的传递机制

按照调用，一级一级往上查找异常处理，直到Python解释器没找到异常处理则报错

```python
def test01():
    print("test1开始")
    print(aa)
def test02():
    print("test2开始")
    test01()
    print("test2结束")
def test03():
    print("test3开始")
    try:
        test02()
    except:
        pass
    print("test3结束")
test03()
```



#### 抛出自定义异常

- 自定义异常
- 以及抛出自定义异常

```python
class GenderException(Exception):
	def __init__(self):
		super().__init__()
		self.errMsg = "性别只能设置男和女"
```



```Python
class GenderException(Exception):
    def __init__(self):
        super().__init__()
        self.errMsg = "性别只能设置男和女"

class student():
    def __init__(self, name, gender):
        self.name = name
        self.setGener(gender)
    def setGener(self, gender):
        if gender == '男' or gender == '女':
            self.__gender = gender
        else:
            raise GenderException()
    def getGender(self):
        return self.__gender
    def showInfo(self):
        print("name = %s, gender = %s" % (self.name, self.__gender))
try:
    stu = student("学生1", '123')
    stu.showInfo()
except Exception as e:
    print(type(e))
    print(e.errMsg)
```



#### 异常处理底层





### LEGB 规则

Python查找名称时，是按照LEGB 规则查找的：local -> Enclosed -> global -> build in

Local 指的是函数或者类的方法内部

Enclosed 指的是嵌套函数

Global 指的是模块中的全局变量

Built in 指的是Python为自己保留的特殊名称





### 构造函数

____new___ 方法用于创建对象，但我们一般无需重定义

____init___ 方法初始化对象



### @property 装饰器

@property 可以将一个方法的调用方式变成”属性调用“

```python
class Emploee:
	@property
	def salary(self):
		print("salary run...")
		return 1000
		
e1 = Emploee()
print(e1.salary)
```

@property 简化私有属性的访问方式

```python
class Student(object):
    def __init__(self, name, age):
        self.name = name
        self.__age = age
    def setAge(self, age):
        if isinstance(age, int):
            self.__age = age
        else:
            raise TypeError("类型错误")
    def getAge(self):
        return self.__age
    age = property(getAge, setAge)

s1 = Student("ysy", 18)
s1.age = 19
print(s1.age)
```





#### 属性私有化

1. xx 一般情况下使用的变量

2. _xx  `_PI=3.14` 在某个模块中，如果变量是 _xx 形式的，使用from import * 的方式，无法使用

3. __xx 私有属性/私有方法（其实就是改名了）（名字重整（改名））

   比如 __age 的访问变成，`_Class__age` 访问

   私有方法也是一致

4. ________xx________

   主要用于方法

   `__init__`、`__del__` 等自定义方法避免与方法重名

5. xx__

​		用来区分变量名/方法名

```python
class Emploee:
	def __init__(self, name, salary):
		self.__name = name
		self.__salary = salary
emp1 = Emploee("ye", 1000)
emp1.salary=-2000


class Emploee:
	@property
	def salary(self):
		return self.__salary
    @salary.setter
    def salary(self, salary):
        self.__salary = salary
		
e1 = Emploee()
print(e1.salary)
e1.salary = -2000
print(e1.salary)
```



### 类默认实现方法

#### ______slot______



#### ______new______



#### ______hash______





### 继承与重写

Python支持多重继承，一个子类可以继承多个父类



#### 访问私有成员

如果在类定义中没有指定父类，则默认父类是object类。也就是说，object是所有类的父类，里面定义了一些所有类共有的默认实现，比如：____new____()

```python
class Persion():
	pass
class Student(Person):
	pass

class Persion():
	def __init(self, name, age):
        self.name = name
        self.age = age

class Student(Person):
	def __init__(self, name, age, score):
        Person.__init__(self, name, age) # 必须显示的调用父类的构造化方法，不然解释器不会去调用
        self.score = score


#Student-> Person ->object 类的继承关系
print(Student.mro())

s = student("ye", 19, 81)
print(dir(s))

# 访问私有成员
print(s._Person__age)

class person():
    def __init__(self, age, name):
        self.__age = age
        self.name = name


class people(person):
    def __init__(self, age, name, score):
        person.__init__(self, age, name)
        self.score = score


s = people(18, "ye", 81)
print(dir(s))
print(s._person__age)
```



类成员的继承和重写

1. 成员继承：子类继承了父类除构造方法之外的所有成员。
2. 方法重写：子类可以重新定义父类中的方法，这样就会覆盖父类的方法，也称为“重写”



#### mro() 函数

通过类的方法mro() 或者类的属性____mro____ 可以输出这个类的继承层次结构

```python
class A:pass
class B(A):pass
class C(B):pass
```



### object 根类

object 类是所有类的父类，因此所有类都有object 类的属性和方法。显然有必要深入研究一下object 类的结构。对于我们继续学习Python 很有好处。

**dir() 查看对象属性**

为了深入学习对象，我们先学习内置函数dir()，他可以让我们方便查看对象的所有属性。

**重写____str____() 方法**

object 有一个____str____() 方法，用于返回一个对于“对象的描述”，对应于内置函数str() 经常用于print() 方法，帮助我们查看对象的信息。____str____() 可以重写。



```python
class Person:
	def __init__(self, name):
		self.name = name
	def __str__(self):
		return "名字是：{0}".format(self.name)
p = Person("ye")
print(p) # 名字是：ye （如果不加，显示Person.object）
```



### 多重继承

​	Python 支持多重继承，一个子类可以有多个“直接父类”。这样，就具备了“多个父类”的特点。但是由于，这样会被“类的整体层次”搞得异常复杂，尽量避免使用。

```python
class A:pass
class B:pass
class C(B, A):pass
```

极有可能变成网状结构



Python支持多继承，如果父类中有相同名字的方法，在子类没有指定父类名时，解释器将“从左到右” 按顺序搜索。

MRO（method resolution order）：方法解析顺序。



### super()

通过super（）获得父类定义

在子类中，如果想要获得父类的方法时，我们可以通过super（）来做。super（）代表父类的定义，不是父类对象

```python
class A:
    def say(self):
        print("A recalled!!!")

class C:
    def say(self):
        print("C recalled!!!")

class B(C, A):
    def say(self):
        #A.say(self)
        super().say()
        print("B recalled!!!")

b = B()
```



### 多态

指的是同一个方法调用由于对象不同可能产生不同的行为。

1. 多态是方法的多态，属性没有多态
2. 多态的存在有2个必要条件：继承、方法重写

```python
class man():
    pass

class Chinese(man):
    def eat(self):
        print("chinese use chopsticks!!")
class English(man):
    def eat(self):
        print("english use knives!!!")

def manEat(m):
    if isinstance(m, man):
        m.eat()
    else:
        print("不能吃饭")

manEat(English())
```



#### 特殊方法和运算符重载

Python 的运算符实际上是通过**调用对象的特殊方法**实现的。

| 运算符    | 特殊方法                            | 说明       |
| --------- | ----------------------------------- | ---------- |
| 运算符+   | ____add____                         | 加法       |
| 运算符-   | ____sub____                         | 减法       |
| <, <=, == | ____lt____, ____le____, ____eq____  | 比较运算符 |
| >, >= ,!= | ____gt____,____ge____,____ne____    | 比较运算符 |
| \|, ^, &  | ____or____,____xor____, ____and____ | 逻辑       |

常见的特殊方法统计如下

| 方法                       | 说明       | 例子           |
| -------------------------- | ---------- | -------------- |
| ____init____               | 构造方法   | 对象创建       |
| ____del____                | 析构方法   | 对象回收       |
| ____repr____， ____str____ | 打印，转换 | print(a)       |
| ____call____               | 函数调用   | a()            |
| ____getattr____            | 点号运算   | a.xxx          |
| ____setattr____            | 属性赋值   | a.xxx = value  |
| ____getitem____            | 索引运算   | a[key]         |
| ____setitem____            | 索引赋值   | a[key] = value |
| ____len____                | 长度       | lena(a)        |



我们可以重写上面的特殊方法，即实现了“运算符的重载”

```python
class person:
    def __init__(self, name):
        self.name = name

    def __add__(self, other):
        if isinstance(other, person):
            return "{0} + {1}".format(other.name, self.name)
        else:
            return "error"

    def __mul__(self, other):
        if isinstance(other, int):
            return other * self.name
        else:
            return "error"

    def __rmul__(self, other):
        if isinstance(other, int):
            return other * self.name
        else:
            return "error"


p = person("ye")
print(p + p)
print(p * 2)
print(2 * p)
```



### 特殊属性

```python
class A:
    pass
class B:
    pass
class C(B, A):
    def __init__(self, nn):
        self.nn = nn
    def cc(self):
        print("cc recalled")

classc = C(3)
print(classc.__dict__)
print(classc.__class__)
print(C.__bases__)
print(C.mro())
print(A.__subclasses__())
>>
{'nn': 3}
<class '__main__.C'>
(<class '__main__.B'>, <class '__main__.A'>)
[<class '__main__.C'>, <class '__main__.B'>, <class '__main__.A'>, <class 'object'>]
[<class '__main__.C'>]
```



### 组合

"is-a"、"has-a"	分别为继承和组合



```
namedtuple
```



### 单例模式

```python
import threading
class MySingleton:
    _instance_lock = threading.Lock()

    def __init__(self):
        pass
    def __new__(cls, *args, **kwargs):
        if not hasattr(MySingleton, "_instance"):
            with MySingleton._instance_lock:
                if not hasattr(MySingleton, "_instance"):
                    MySingleton._instance = object.__new__(cls)
        return MySingleton._instance
obj1 = MySingleton()
obj2 = MySingleton()
print(obj1)
print(obj2
```



### yield 关键字



### ____all____ 的使用

Python3环境下不提倡使用

Python2经常

`MyMath.py`

```python
__all__ = ['add']
def add(a, b):
	return a + b
def sub(a, b):
    return a - b
```

```python
from MyMath import *
# * 代表__all__中所有声明方法, 若没声明__all__, 则默认所有
```



### 包

- 概念

包是一个分层次的文件目录结构，它定义了一个由模块及子包和子包下的子包等组成的Python 的应用环境，包中要包含一个______init______.py模块

也可以理解为文件夹，前提，文件中包含一个______init______.py

作用

- 将模块归类，方便整理
- 防止模块名冲突



- 新的模块名

包名.模块名



- 包中模块的导入方式

import package1.module1

or

from package1 import module1



```python
import package1.MyMath
from package1.MyMath import *
```



#### ______init______.py

______init______.py 的作用

- 类中的______init____ __初始化方法

- 包中的______init______.py 初始化模块



> 首次使用包中的模块时，______init______.py 模块会被执行一次



**__init__.py 中可以存放什么？**



可以存放同普通模块一样的代码

- 变量、类、函数... 都是OK的（但是不这样做）

一般会写一些辅助的代码，让你更方便的使用模块

```python
# 一定意义上的简化
# __init__.py
import MyMath
```

```python
# test.py
import package1 # => 等同于import package1.MyMath
```



```python
# 一定意义上的简化
# __init__.py
from MyMath import *
```

```python
# test.py
from package1 import * # => 等同于from package1.MyMath import *
```



```python
# __init__.py
from graphlearn.python.data.decoder import Decoder
from graphlearn.python.data.feature_spec import FeatureSpec
from graphlearn.python.data.feature_spec import SparseSpec
from graphlearn.python.data.feature_spec import DenseSpec
from graphlearn.python.data.feature_spec import MultivalSpec
from graphlearn.python.data.state import NodeState, EdgeState
from graphlearn.python.data.topology import Topology
from graphlearn.python.data.values import Values
from graphlearn.python.data.values import Nodes, Edges
from graphlearn.python.data.values import SparseNodes, SparseEdges
from graphlearn.python.data.values import Layer, Layers
from graphlearn.python.data.values import SubGraph

__all__ = [
    "Decoder",
    "SparseSpec",
    "DenseSpec",
    "MultivalSpec",
    "NodeState",
    "EdgeState",
    "Topology",
    "Values",
    "Nodes",
    "Edges",
    "SparseNodes",
    "SparseEdges",
    "Layer",
    "Layers",
    "SubGraph"
]

```



### 模块的发布

- 模块的发布

  - 为什么要发布

  - `sys.path` （导入目标模块）

    ```python
    import sys
    result = sys.path
    print(type(result))
    for i in result:
    	print(i)	# 类似C++ 动态库搜索路径一般
    ```

    解决方法：

    1. 将模块所在路径，手动加入到`sys.path` 中

    2. 将自定义模块，发布到系统目录

       - 确定发布的模块（目录结构）

         | -- setup.py

         | -- package1

         ​		|

         ​			-- 自定义模块MyMath

       - setup 的编辑工作
         setup()

       - 构建模块

         python setup.py build

       - 发布模块

         python setup.py sdist

    ```python
    import sys
    sys.path.append("URL") # 路径分隔符分两种方式：1. '/' 2. '\\'
    ```

    setup.py

    ```python
    from distutils.core import setup
    setup(name='自定义压缩包', version='1.0', description='描述', author='Devi', py_modules=['package1.MyMath', ])
    ```

    

- 模块的安装

  1. 通过命令完成安装（推荐）更安全（https://www.bilibili.com/video/BV1FT4y1Z7yi?p=116）
     - 找到之前的发布的压缩包，解压操作
     - python setup.py install（安装到系统路径） / pythono setup.py install -prefix='url'（类似gcc）
  2. 暴力安装
     - 直接将要安装的包，以及模块，复制对应的系统路径
