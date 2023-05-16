# tf.feature_column





参考自：

[tf.feature_column - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/118381625)

[tf.feature_column的特征处理探究 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/73701872)



## **1. 背景**

**tf.estimator**是tensorflow的一个高级API接口，它最大的特点在于兼容分布式和单机两种场景，工程师可以在**同一套代码结构下即实现单机训练也可以实现分布式训练**，正是因为这样的特点，目前包括阿里在内的很多公司都在使用这一接口来构建自己的深度学习模型



特征预处理是几乎所有机器学习模型所必须的一个过程，常见的特征预处理方法包括：**连续变量分箱化**、离散变量one-hot、离散指标embedding等，tensorflow给我们提供了一个功能强大的特征处理函数tf.feature_column，它通过对**特征处理将数据输入网络**并交由estimator来进行训练，本文通过实际的数据输出来直观地介绍与展现tf.feature_column的基本用法



> 连续变量分箱化：比如，age 可分为[0, 10)、[10, 20), ..., [90, +∞)，age=14 属于第一个bucket



## 2. 数据处理

特征数据主要包括**categorical**和**dense**两类，处理方法是使用tensorflow中的feature_column接口来进行定义，如下图，总共有九种不同的函数，分别有五种Categorical function、三种numerical function 加上一种bucketized_column可属于任何一种，categorical column中的 with_identity（独一性）其实和 dense column中的indicator_column没有区别，都是**类别特征的one-hot**表示，**但是其属于不同的特征类别，前者属于categorical后者属于dense**，对于estimator编写的不同网络而言，其可接受的one-hot类型不同，这里在实际操作中需要注意转换

![img](https://pic3.zhimg.com/v2-72716a0ab2971fea8cadb98cbceeddfa_r.jpg)



#### 2.1 categorical column

##### **2.1.1 categorical_column_with_identity**

categorical_column_with_identity：**把numerical data转乘one hot encoding**

![img](https://pic4.zhimg.com/v2-3145e7209e6120e2485b462dafe6627b_r.jpg)

只适用于值为整数的类别型变量，实际输出如下：

```python
import tensorflow as tf
sess=tf.Session()

#特征数据
features = {
    'birthplace': [[1],[1],[3],[4]]
}

#特征列
birthplace = tf.feature_column.categorical_column_with_identity("birthplace", num_buckets=3, default_value=0)
birthplace = tf.feature_column.indicator_column(birthplace)
#组合特征列
columns = [
    birthplace
]

#输入层（数据，特征列）
inputs = tf.feature_column.input_layer(features, columns)

#初始化并运行
init = tf.global_variables_initializer()
sess.run(tf.tables_initializer())
sess.run(init)
v=sess.run(inputs)
print(v)

In [33]:
[[0. 1. 0.]
 [0. 1. 0.]
 [0. 0. 1.]
 [1. 0. 0.]]
```

如图输出为birthplace的one-hot结果，num_buckets用于确定每一个one-hot向量的最大特征类别数



##### **2.1.2 categorical_column_with_vocabulary_list or categorical_column_with_vocabulary_file**

- categorical_column_with_vocabulary_list or categorical_column_with_vocabulary_file**：**根据单词的序列顺序，把单词根据index转换成one hot encoding

![img](https://pic2.zhimg.com/80/v2-700a75b4daebec26db1f3ab4ae9add5d_720w.webp)

- 主要用于处理非整数型的类别特征，两个函数的区别在于处理变量类别的多少，数量前者对应类别少的情况，所有可能的类别可以直接输入，后者对应类别多的情况，所有可能的类别可以存在一个文件中输入，实际输出如下：

```python
import tensorflow as tf
sess=tf.Session()

#特征数据
features = {
    'sex': ['male', 'male', 'female', 'female'],
}

#特征列
sex_column = tf.feature_column.categorical_column_with_vocabulary_list('sex', ['male', 'female'])
sex_column = tf.feature_column.indicator_column(sex_column)
#组合特征列
columns = [
    sex_column
]

#输入层（数据，特征列）
inputs = tf.feature_column.input_layer(features, columns)

#初始化并运行
init = tf.global_variables_initializer()
sess.run(tf.tables_initializer())
sess.run(init)

v=sess.run(inputs)
print(v)

In [33]:
[[1. 0.]
 [1. 0.]
 [0. 1.]
 [0. 1.]]
```

- 如图输出为sex的one-hot结果，其后跟着的list用于定义该变量的所有类别。



##### **2.1.3 categorical_column_with_hash_bucket**

- categorical_column_with_hash_bucket：对于处理包含大量文字或数字类别的特征时可使用hash的方式，这能快速地建立对应的对照表，缺点则是会有哈希冲突的问题。**（推荐领域通常使用，较为通用做法，在不定长情况下）**

![img](https://pic4.zhimg.com/v2-b8f3f29807e2c98e53549015ed58aaf7_r.jpg)

- hash_bucket_size的大小一般设置为**总类别数的2-5倍**，该函数适用于不能确定所有类别样式的类别变量，实际输出如下：

```python
import tensorflow as tf
sess=tf.Session()

#特征数据
features = {
    'department': ['sport', 'sport', 'drawing', 'gardening', 'travelling'],
}

#特征列
department = tf.feature_column.categorical_column_with_hash_bucket('department', 4, dtype=tf.string)
department = tf.feature_column.indicator_column(department)
#组合特征列
columns = [
    department
]

#输入层（数据，特征列）
inputs = tf.feature_column.input_layer(features, columns)

#初始化并运行
init = tf.global_variables_initializer()
sess.run(tf.tables_initializer())
sess.run(init)

v=sess.run(inputs)
print(v)

In [33]:
[[0. 1. 0. 0.]
 [0. 1. 0. 0.]
 [0. 1. 0. 0.]
 [1. 0. 0. 0.]
 [0. 1. 0. 0.]]
```

- 如上，输出为department的one-hot结果，对于不同类的department出现了哈希冲突的情况。



##### **2.1.4 crossed_column**

- crossed_column**：**特征交叉，在有些情况下，特征独自编码与多维特征交叉后的特征特性会有不一样的结果。
- **该函数不能对hash映射之后的特征进行交叉**，实际输出如下：

交叉列可以把多个特征合并成为一个特征，比如把经度longitude、维度latitude两个特征合并为地理位置特征location。
如下图，我们把Atlanda城市范围的地图横向分成100区间，竖向分成100区间，总共分割成为10000块小区域。（也许接下来我们需要从数据分析出哪里是富人区哪里是穷人区）

![img](https://pic4.zhimg.com/80/v2-04eec92ff5c0115df69b30b9b56b28c3_720w.webp)

演示代码

```python
import tensorflow as tf

featrues = {
        'longtitude': [19,61,30,9,45],
        'latitude': [45,40,72,81,24]
    }

longtitude = tf.feature_column.numeric_column('longtitude')
latitude = tf.feature_column.numeric_column('latitude')

longtitude_b_c = tf.feature_column.bucketized_column(longtitude, [33,66])
latitude_b_c  = tf.feature_column.bucketized_column(latitude,[33,66])

column = tf.feature_column.crossed_column([longtitude_b_c, latitude_b_c], 12)

indicator = tf.feature_column.indicator_column(column)
tensor = tf.feature_column.input_layer(featrues, [indicator])

with tf.Session() as session:
    session.run(tf.global_variables_initializer())
    session.run(tf.tables_initializer())
    print(session.run([tensor]))
```

上面的代码中进行了分箱操作，分成～33，33～66，66～三箱，运行得到下面输出

```python
[array([[0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 1.],
       [0., 0., 0., 0., 0., 0., 0., 0., 0., 1., 0., 0.],
       [0., 0., 1., 0., 0., 0., 0., 0., 0., 0., 0., 0.],
       [0., 0., 1., 0., 0., 0., 0., 0., 0., 0., 0., 0.],
       [0., 0., 0., 0., 0., 1., 0., 0., 0., 0., 0., 0.]], dtype=float32)]
```



##### **2.1.5 embedding_column**

embedding_column：把categorical的data，借由lookup table的方式找寻对应的feature vector来表示。假设有81个单词，若使用categorical_column_vocbulary_list，然后转乘numerical的indicator_column，则需要81维度来描述此些单词，这样极易造成数据的稀疏化，对于推荐系统而言，大维度的稀疏数据对于结果的影响较大，因此我们考虑使用embedding_column，来对数据进行压缩，实际输出如下：

当我们遇到成千上万个类别的时候，独热列表就会变的特别长[0,1,0,0,0,....0,0,0]。embedding layer可以解决这个问题，它不再限定每个元素必须是0或1，而可以是任何数字，从而使用更少的元素数表现数据。
如下图，我们最初的数据可能是4个单词比如dog、spoon、scissors、guitar，然后这些单词被分类特征列Categorical处理成为数字0、32、79、80，接下来我们可以使用指示列来处理成为独热的01列表（图中假设我们有81种单词分类），也可以按照嵌入Embeding列来处理成小数元素组成的3元素数列。

```python
import tensorflow as tf
sess=tf.Session()

#特征数据
features = {
    'sex': [0, 1, 0, 0, 1],
    'department': ['sport', 'sport', 'drawing', 'gardening', 'travelling'],
}

#特征列
department = tf.feature_column.categorical_column_with_vocabulary_list('department', ['sport','drawing','gardening','travelling'], dtype=tf.string)
sex = tf.feature_column.categorical_column_with_identity('sex', num_buckets=2, default_value=0)
sex_department = tf.feature_column.crossed_column([department,sex], 16)
sex_department_emb = tf.feature_column.embedding_column(sex_department, 5, combiner='sqrtn')

#组合特征列
columns = [
    sex_department_emb
]

#输入层（数据，特征列）
inputs = tf.feature_column.input_layer(features, columns)

#初始化并运行
init = tf.global_variables_initializer()
sess.run(tf.tables_initializer())
sess.run(init)

v=sess.run(inputs)
print(v)

In [33]:
[[ 0.2852817   0.52436996 -0.0064039   0.503416    0.7205597 ]
 [-0.5937389  -0.8212001  -0.08457068 -0.08960886  0.3914484 ]
 [ 0.11903824  0.4949713   0.35738686  0.01798844  0.6499385 ]
 [ 0.3089911  -0.4591549   0.687267    0.32874516 -0.6242199 ]
 [-0.5937389  -0.8212001  -0.08457068 -0.08960886  0.3914484 ]]
```

如上，输出为交叉特征的embading向量，embedding_column的输入为处理之后的特征变量，可以是categorical也可以是dense的

嵌入列中的小数只在train训练的时候自动计算生成，能够有效增加训练模型的效率和性能，同时又能便于机器学习从数据中发现潜在的新规律。
为什么嵌入Embeding的都是[0.421,0.399,0.512]这样的3元素列表，而不是4元5元？实际上有下面的参考算法：

![img](https://pic3.zhimg.com/80/v2-b56d427e43e7df16b2fe0c5785cb28d6_720w.webp)

embedding layer的dimension等于category开4次方，也就是3的4次方等于81种类

#### 2.2 Dense column

##### **2.2.1 numeric_column**

- numeric_column：该函数主要用于处理连续型变量，即可以是float类型也可以是int类似，从table中读取对应的(key)column，并把它转成dtype的格式，实际情况如下：（真实值传递）

```python
import tensorflow as tf
sess=tf.Session()

#特征数据
features = {
    'sale': [1.2, 2.3, 1.2, 1.5, 2.2]
}

#特征列
sale = tf.feature_column.numeric_column("sale", default_value=0.0)
#组合特征列
columns = [
    sale
]

#输入层（数据，特征列）
inputs = tf.feature_column.input_layer(features, columns)

#初始化并运行
init = tf.global_variables_initializer()
sess.run(tf.tables_initializer())
sess.run(init)

v=sess.run(inputs)
print(v)

In [33]:
[[1.2]
 [2.3]
 [1.2]
 [1.5]
 [2.2]]
```

##### **2.2.2 bucketized_column**

- bucketized_column: 该函数将连续变量进行分桶离散化，输出one-hot的结果，方便连续值指标与分类变量进行交叉特征构建，

![img](https://pic3.zhimg.com/v2-3ba0d5bcecd86642c39598b848ec534e_r.jpg)



- 实际情况如下：

```python
import numpy as np
import tensorflow as tf
sess=tf.Session()

#特征数据
features = {
    'sale': [0.1, 0.2, 0.5, 1.0, 0.2]
}

#特征列
step_val = 1.0 / 2
boundaries = list(np.arange(0, 1, step_val))
sale = tf.feature_column.bucketized_column(tf.feature_column.numeric_column('sale',default_value=0.0), boundaries=boundaries)
#组合特征列
columns = [
    sale
]

#输入层（数据，特征列）
inputs = tf.feature_column.input_layer(features, columns)

#初始化并运行
init = tf.global_variables_initializer()
sess.run(tf.tables_initializer())
sess.run(init)

v=sess.run(inputs)
print(v)

In [33]:
[[0. 1. 0.]
 [0. 1. 0.]
 [0. 0. 1.]
 [0. 0. 1.]
 [0. 1. 0.]]
```



## 3. 总结

本文通过直观的数据输出来展现tf.feature_column的特征处理过程，方便大家理解这个函数，tensorflow作为目前最常用的深度学习框架，有着很多高级的API，这些接口都可以极大方便我们算法工程师的工作，tf.estimator不仅可以很好地处理特征，同时它将train、evaluatete、predict都集成到了一起，大家平时可以多使用该接口



