# Effective STL， 30条有效使用STL的经验

摘自：https://mp.weixin.qq.com/s/TZU2-LkePp4YsUnZN_HvoA



3.确保容器中的对象拷贝正确而高效

a)大家应该都知道，容器中存放的都是对象的拷贝，想要拷贝正确那就实现拷贝构造函数和拷贝赋值运算符

b)想要更高效，可以使容器包含指针而不是对象，也可考虑智能指针



4.调用empty而不是检查size()是否为0

a)empty对所有的标准容器都是常数时间操作，而对一些list实现，size耗费线性时间

> 用处不大



7.慎重选择删除元素的方法

**a)要删除容器中有特定值的所有对象**

i.如果容器是vector、string或deque，则使用erase-remove习惯用法

ii.如果容器是list，则使用list::remove

iii.如果容器是一个标准关联容器，则使用它的erase成员函数

**b)要删除容器中满足特定条件的所有对象**

i.如果容器是vector、string或deque，则使用erase-remove_if习惯用法

ii.如果容器是list，则使用list::remove_if

iii.如果容器是一个标准关联容器，则使用remove_copy_if和swap，或者写一个循环来遍历容器中的元素，记住当把迭代器传给erase时，要对它进行后缀递增

**c)要在循环内部做某些操作**

i.如果容器是一个标准序列容器，则写一个循环来遍历容器中的元素，记住每次调用erase时，要用它的返回值更新迭代器

ii.如果容器是一个标准关联容器，则写一个循环来遍历容器中的元素，记住当把迭代器传给erase时，要对迭代器做后缀递增。

**d)返回值更新迭代器示例**

```C++
for (auto i = c.begin(); i != c.end();) {
    if (xxx) {
        i = c.erase(i);    
    }
    else ++i;
}
```

**e)迭代器后缀递增示例**

```cpp
for (auto i = c.begin(); i != c.end();) {
    if (xxx) {
        c.erase(i++);    
    }
    else ++i;
}
```



9.vector等容器考虑使用reserve来避免不必要的重新分配

**a)这种动态扩容的容器每次扩容都会大体经历4步：**

i.分配一块大小为当前容量的某个倍数的新内存。大多数实现中，vector和string的容器每次以2的倍数增长

ii.把容器的所有元素从旧的内存移动或者拷贝到新的内存中

iii.如果有拷贝，析构掉旧内存中的对象

iv.如果有拷贝，释放旧内存

**b)明确size()、capacity()、resize()、reserve()四个成员函数的具体含义**

**c)reserve能使重新分配的次数减少到最低限度，避免重新分配和迭代器失效带来的开销，两种方式：**

i.若能明确知道或预计容器最终有多少元素，可使用reverse，预留适当大小的空间

ii.先预留足够大的空间，然后，当把所有数据都加入以后，再去除多余的空间。



10.使用swap技巧除去多余的容量

**a)vector****().swap(a)**

**b)a.clear()**

**c)以上两种都是清空容器的方法，swap相对于clear一般更合适一些**



16.考虑用排序的vector替代关联容器

a)在排序的vector中存储数据可能比在标准关联容器中存储同样的数据要耗费更少的内存。

b)由于Page Fault，通过二分搜索来查找一个排序的vector可能比查找一个标准关联容器要更快一些

c)对于排序的vector，最不利的地方在于它必须保持有序，这对vector来说，代价是很高的。所以，在查找操作几乎从不跟插入删除操作混在一起时，使用排序的vector才更合适。



17.当效率至关重要时，请在map::operator[]与map::insert之间谨慎做出选择

a)当向map中添加元素时，优先选用insert而不是operator[]

b)当更新map中的值时，优先选用operator[]



20.对于逐个字符的输入, 请考虑使用istreambuf_iterator

a)istreambuf_iterator性能一般优于istream_iterator

b)istreambuf_iterator不会跳过任何字符

```cpp
istream inputFile("xxx.txt");
string str(istreambuf_iterator<char>(inputFile), istreambuf_iterator<char>());
```



21.容器的插入, 要确保目标空间足够大

a)灵活使用reverse和back_inserter、front_inserter和inserter返回的迭代器。



23.如果确实需要删除元素，则需要在remove这一类算法之后调用erase

a)erase-remove这块应该大家都知道

b)list是个例外，list的remove就是erase

c)remove指针时注意释放掉对应的内存，防止内存泄漏



24.了解哪些算法使用排序的区间作为参数

**a)某些算法为了性能考虑，需要使用排序的区间作为参数**

**b)如果传递了没有排序的区间进去，会导致错误的结果**

**c)要求排序区间的STL算法：**

i.binary_search

ii.lower_bound

iii.upper_bound

iv.equal_range

v.set_union

vi.set_intersection

vii.set_difference

viii.set_symmetric_difference

ix.merge

x.inplace_merge

xi.includes

d)下面的算法不一定要求排序区间，但通常和排序区间一起使用

i.unique

ii.unique_copy







