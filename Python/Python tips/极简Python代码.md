# 极简的Python代码



**1.重复元素判定**

以下方法可以检查给定列表是不是存在重复元素，它会使用 set() 函数来移除所有重复元素。

```python
def all_unique(lst):
    return len(lst) == len(set(lst))
x = [1,1,2,2,3,2,3,4,5,6]
y = [1,2,3,4,5]
all_unique(x) # False
all_unique(y) # True
```



**2.字符元素组成判定**

检查两个字符串的组成元素是不是一样的。

```python
from collections import Counter

def anagram(first, second):
    return Counter(first) == Counter(second)
anagram("abcd3", "3acdb") # True
```



**3.内存占用**

下面的代码块可以检查变量 variable 所占用的内存。

```python
import sys 
variable = 30 
print(sys.getsizeof(variable)) # 24
```



**4.字节占用**

下面的代码块可以检查字符串占用的字节数。

```python
def byte_size(string):
    return(len(string.encode('utf-8')))

byte_size('😀') # 4
byte_size('Hello World') # 11
```



**5.打印 N 次字符串**

该代码块不需要循环语句就能打印 N 次字符串。

```python
n = 2; 
s ="Programming"; 

print(s * n);
# ProgrammingProgramming
```



**6.大写第一个字母**

以下代码块会使用 title() 方法，从而大写字符串中每一个单词的首字母。

```python
s = "programming is awesome"

print(s.title())
# Programming Is Awesome
```



**7.分块**

给定具体的大小，定义一个函数以按照这个大小切割列表。

```python
from math import ceil

def chunk(chunk_list, size):
    return list(
        map(lambda x: chunk_list[x * size:x * size + size],
            list(range(0, ceil(len(chunk_list) / size)))))

chunk([1,2,3,4,5],2)
# [[1,2],[3,4],5]
```





**8.压缩**

这个方法可以将布尔型的值去掉，例如(False, None, 0, "")，它使用 filter() 函数。

```python
def compact(com_list):
    return list(filter(bool, com_list))
compact([0, 1, False, 2, '', 3, 'a', 's', 34])
# [ 1, 2, 3, 'a', 's', 34 ]
```





**9.解包**

如下代码段可以将打包好的成对列表解开成两组不同的元组。

```python
array = [['a', 'b'], ['c', 'd'], ['e', 'f']]
transposed = zip(*array)
print(transposed)
# [('a', 'c', 'e'), ('b', 'd', 'f')]
```



**10.链式对比**

我们可以在一行代码中使用不同的运算符对比多个不同的元素

```python
a = 3
print( 2 < a < 8) # True
print(1 == a < 2) # False
```





**11.逗号连接**

下面的代码可以将列表连接成单个字符串，且每一个元素间的分隔方式设置为了逗号。

```python
hobbies = ["basketball", "football", "swimming"]

print("My hobbies are: " + ", ".join(hobbies))
# My hobbies are: basketball, football, swimming
```



**12.元音统计**

以下方法将统计字符串中的元音(‘a’, ‘e’, ‘i’, ‘o’, ‘u’)的个数，它是通过正则表达式做的。

```python
import re

def count_vowels(str):
    return len(len(re.findall(r'[aeiou]', str, re.IGNORECASE)))

count_vowels('foobar') # 3
count_vowels('gym') # 0
```





**13.首字母小写**

如下方法将令给定字符串的第一个字符统一为小写。

```python
def decapitalize(string):
    return str[:1].lower() + str[1:]

decapitalize('FooBar') # 'fooBar'
decapitalize('FooBar') # 'fooBar'
```





**14.展开列表**

该方法将通过递归的方式将列表的嵌套展开为单个列表。

```python
def spread(arg):
    ret = []
    for i in arg:
        if isinstance(i, list):
            ret.extend(i)
        else:
            ret.append(i)
    return ret

def deep_flatten(lst):
    result = []
    result.extend(
        spread(list(map(lambda x: deep_flatten(x) if type(x) == list else x, lst))))
    return result

deep_flatten([1, [2], [[3], 4], 5]) # [1,2,3,4,5]
```



**15.列表的差**

该方法将返回第一个列表的元素，其不在第二个列表内。如果同时要反馈第二个列表独有的元素，还需要加一句 set_b.difference(set_a)。

```python
def difference(a, b):
    set_a = set(a)
    set_b = set(b)
    comparison = set_a.difference(set_b)
    return list(comparison)

difference([1,2,3], [1,2,4]) # [3]
```



**16.文件打开**

```python
import sys 
import re 
WORD_RE = re.compile(r'\w+') 
index = {} 
with open(sys.argv[1], encoding='utf-8') as fp: 
     for line_no, line in enumerate(fp, 1): 
         for match in WORD_RE.finditer(line): 
             word = match.group() 
             column_no = match.start()+1 
             location = (line_no, column_no) 
             # 这其实是一种很不好的实现，这样写只是为了证明论点
             occurrences = index.get(word, []) ➊
             occurrences.append(location) ➋
             index[word] = occurrences ➌
# 以字母顺序打印出结果
for word in sorted(index, key=str.upper): ➍
     print(word, index[word])
```







