# 以线性回归为例，深入理解tensorflow的Operation、Tensor、Node的区别

摘自：https://blog.csdn.net/qq_27825451/article/details/105854164

在使用[tensorflow](https://so.csdn.net/so/search?q=tensorflow&spm=1001.2101.3001.7020)的时候，常常会被**Operation、Tensor、Op_name、tensor_name**等等概念搞混淆，本文专门通过一个简单的例子来深入讲解他们之间的区别于本质，并且如何在tensorboard中进行查看。

## 线性回归的完整实例

本文以一个两层[神经网络](https://so.csdn.net/so/search?q=神经网络&spm=1001.2101.3001.7020)来实现线性回归，代码如下：

```python
import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
 
#使用numpy生成200个随机点
x_data=np.linspace(-0.5,0.5,200)[:,np.newaxis]
noise=np.random.normal(0,0.02,x_data.shape)
y_data=np.square(x_data)+noise
 
#定义两个placeholder存放输入数据
x=tf.placeholder(tf.float32,[None,1],name="model_input")
y=tf.placeholder(tf.float32,[None,1],name="model_output")
 
#定义神经网络中间层
Weights_L1=tf.Variable(tf.random_normal([1,10]),name="Dense1_weights")
biases_L1=tf.Variable(tf.zeros([1,10]),name="Dense1_bias")    #加入偏置项
Wx_plus_b_L1=tf.matmul(x,Weights_L1)+biases_L1
L1=tf.nn.tanh(Wx_plus_b_L1,name="Dense1_output")   #加入激活函数
 
#定义神经网络输出层
Weights_L2=tf.Variable(tf.random_normal([10,1]),name="Dense2_weights")
biases_L2=tf.Variable(tf.zeros([1,1]),name="Dense2_bias")  #加入偏置项
Wx_plus_b_L2=tf.matmul(L1,Weights_L2)+biases_L2
prediction=tf.nn.tanh(Wx_plus_b_L2,name="Dense2_ouput")   #加入激活函数
 
#定义损失函数（均方差函数）
loss=tf.reduce_mean(tf.square(y-prediction),name="loss_function")
#定义反向传播算法（使用梯度下降算法训练）
optimizer = tf.train.GradientDescentOptimizer(0.1).minimize(loss,name="optimizer")
 
# tensorboard
loss_scaler = tf.summary.scalar('loss',loss)
merge_summary = tf.summary.merge_all()
 
# 模型保存
saver = tf.train.Saver()
 
gpu_options = tf.GPUOptions()
gpu_options.visible_device_list = "1"
gpu_options.allow_growth = True   
config = tf.ConfigProto(gpu_options = gpu_options)
with tf.Session(config=config) as sess:
    #变量初始化
    sess.run(tf.global_variables_initializer())
    writer=tf.summary.FileWriter('./tensorboard/linear_regression',graph=sess.graph)
    
    #训练2000次
    for i in range(2000):
        opti,loss_,merge_summary_ = sess.run([optimizer,loss,merge_summary],feed_dict={x:x_data,y:y_data})
        writer.add_summary(merge_summary_,global_step=i)
        if i%100==0:
            print(f"now is training {i+1} batch,and loss is {loss_} ")
    save_path = saver.save(sess,"./linear_model/linear_model.ckpt")
    print(f"model have saved in {save_path} ")
```



运行之后，会得到权重保存文件checkpoint，以及tensorboard日志文件，现在打开tensorboard，我们可以看到如下面的graph结构：

![img](https://img-blog.csdnimg.cn/20200430092437409.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzI3ODI1NDUx,size_16,color_FFFFFF,t_70)

在原来的理解中，声明的变量比如a=tf.Variable()这应该是一个[tensor](https://so.csdn.net/so/search?q=tensor&spm=1001.2101.3001.7020)，而进行的操作比如tf.add()应该才是operation，后面发现这种理解是错误的，

我们可以发现几个问题，现在总结如下：

> （1）声明的占位符placeholder，比如上面的model_input，是一个节点node，对应的是operation，用椭圆符号表示；
> （2）声明的变量，把比如上面的Dense1_weights，本质上也是一个node，虽然用的是圆角矩形（namespace），将其双击展开，发现里面包含了3个节点node；

**总结：**

> - **常量、占位符、变量声明、操作函数**本质上都是节点node，都是一种操作operation；
> - 只有在节点之间流动的箭头才表示的是tensor，实现箭头表示有tensor的流动，虚线箭头表示节点之间具有依赖关系；



graph的组成由两部分组成，即**节点node和边tensor**，但是我们并没有直接创建tensor，既没有直接创建边啊，比如有一个变量，如下：

```python
x = tf.random_normal([2, 3], stddev=0.35, name = "weights")
```

我们常常说创建了一个（2,3）的tensor，但是实际上在graph中又是一个节点[node](https://so.csdn.net/so/search?q=node&spm=1001.2101.3001.7020)，这到底是怎了区分和理解呢？

可以这样理解：

> **`Tensor`可以看做一种符号化的句柄，指向操作节点（node）的运算结果，在执行后返回这个节点运算得到的值，在graph中的表现来看，就是这个节点运算之后输出的边，用一个带箭头的边来表示，这样个人觉得比较好理解。**



## 底层概念与实现

这一节主要讨论一下一些东西，即Graph、GraphDef、Node、NodeDef、Op、OpDef等概念

#### **2.1 Graph与GraphDef**

**（1）tf.Graph类的定义**

```python
关键属性：
collections
 
关键方法：
add_to_collection
add_to_collections
as_default
as_graph_def
clear_collection
control_dependencies
create_op
with g.device
finalize
get_all_collection_keys
get_collection
get_collection_ref
get_name_scope
get_name_scope
 
get_operation_by_name
get_operations
get_tensor_by_name
```

> 在我们恢复模型的时候，要预测模型，需要知道模型的输入与输出的名称，就需要用到两个方法
> get_operation_by_name
> get_tensor_by_name

那怎么区分***\*“tensor_name”和“operation_name”\****这两个概念呢？后面会讲到。

**（2）tf.GraphDef类的定义——仅在tensorflow1.x中有**

```sh
四个属性
library: FunctionDefLibrary library
node: repeated NodeDef node（graph中所有的节点定义）
version: int32 version
versions: VersionDef versions
```

在恢复模型的时候，我们遍历图中的所有节点，使用的语句为：

```python
 tensor_name_list = [tensor.name for tensor in graph.as_graph_def().node]# 得到当前图中所有节点的名称
```



#### **2.2 Node和NodeDef**

tensorflow中python接口没有提供显示Node定义，都是通过NodeDef来实现的，另外NodeDef只在tensorflow1.x版本中，它有几个常用的属性：

```sh
Attributes:
device: 该节点所在的设备
input: 该节点的输入节点
name: string，名称
op: 该节点的Op(operation)
```



#### **2.3 Op和OpDef**

**（1）tf.Operation类**

它有一些常见的属性，如下：

```sh
属性
control_inputs
device
graph
inputs
name
node_def
op_def
outputs
type
```

常见的一些方法如下：

```sh
方法
colocation_groups()
get_attr(name)
run(feed_dict=None,session=None)
values()
```



## 如何获取node_name和tensor_name

前面说了，本质上graph的组成是节点（node，即operation）和边（tensor），而tensor是依赖于每一个节点的输出值的，到底怎么去获取节点名称和张量名称呢？

#### **3.1 先看下面的例子：**

```python
import tensorflow as tf
 
print(tf.__version__)
 
a = tf.constant([1], name = 'a') # 创建两个自己命名的常量
b = tf.constant([2], name = 'b')
aa = tf.constant([3])            # 创建两个使用默认名称的常量
bb = tf.constant([4])
x = tf.Variable(initial_value=[1,2,3],name="x")   # 创建两个命名的变量
y = tf.Variable(initial_value=[3,2,1],name="y")
 
# 创建三个操作
a_b = tf.add(a, b, name = "a_add_b")
aa_bb = tf.add(aa, bb)
x_y = tf.add(x,y,name="x_add_y")
 
# 会话GPU的相关配置
gpu_options = tf.GPUOptions()
gpu_options.visible_device_list = "1"
gpu_options.allow_growth = True   
config = tf.ConfigProto(gpu_options = gpu_options)
with tf.Session(config=config) as sess:
    print('a tensor name is : %s, Op name is : %s' %(a.name, a.op.name))
    print('b tensor name is : %s, Op name is : %s' % (b.name, b.op.name))
    print('aa tensor name is : %s, Op name is : %s' %(aa.name, aa.op.name))
    print('bb tensor name is : %s, Op name is : %s' % (bb.name, bb.op.name))
    print('x tensor name is : %s, Op name is : %s' % (x.name, x.op.name))
    print('y tensor name is : %s, Op name is : %s' % (y.name, y.op.name))
    print('a_b tensor name is : %s, Op name is : %s' % (a_b.name, a_b.op.name))
    print('aa_bb tensor name is : %s, Op name is : %s' % (aa_bb.name, aa_bb.op.name))
    print('x_y tensor name is : %s, Op name is : %s' % (x_y.name, x_y.op.name))
    # 每个操作节点(Op Node)是一个 NodeDef 对象，包含 name、op、input、device、attr 等属性
    print("======================================================================")
    # 遍历图中的所有节点
    node_name_list = [node.name for node in tf.get_default_graph().as_graph_def().node]
    for node_name in node_name_list:
        print(node_name)
```

运行结果如下：

```sh
a tensor name is : a:0, Op name is : a
b tensor name is : b:0, Op name is : b
aa tensor name is : Const:0, Op name is : Const
bb tensor name is : Const_1:0, Op name is : Const_1
x tensor name is : x:0, Op name is : x
y tensor name is : y:0, Op name is : y
a_b tensor name is : a_add_b:0, Op name is : a_add_b
aa_bb tensor name is : Add:0, Op name is : Add
x_y tensor name is : x_add_y:0, Op name is : x_add_y
======================================================================
a
b
Const
Const_1
x/initial_value  # x是一个变量，变量在graph中是圆角矩形，里面可以展开成一个字图subgraph，里面又包含一些子节点node
x
x/Assign
x/read
y/initial_value
y
y/Assign
y/read
a_add_b
Add
x_add_y
```

总结如下：

> （1）我们所声明的不管是常量、变量、占位符、操作函数，本质上都是节点node，即operation
>
> （2）操作节点的名称，即node_name为  variable.op.name .命名规则遵循如果是显示指定了name参数，那就是这个指定的参数名称，如果是没有显示指定，则会是常量使用Const,加法操作是Add,如果存在多个使用默认名称的，那么遵循在后面添加一个后缀数字的方法，如
>
> Const、Const_1、Const_2、Const_3...依次下去
> Add、Add_1、Add_2、Add_3、Add_4、...依次下去
> （3）张量名称，tensor_name为 variable.name ，即tensor_name的一般格式为： "<op_name>:<output_index>"
>
> 如上面的 a:0、 b:0、Const:0、Const_1:0、x:0、y:0 等等，为什么要这样做，在每一个node_name后面再添加一个新的index呢？



#### **3.2 为什么tensor_name的格式为 "<op_name>:<output_index>"**

实际上是因为，TensorFlow中自己所创建的节点node完全可以同名称，如果是两个节点名称相同，那我就没办法光通过node_name来区分不同的tensor了，如下所示

创建两个同名称的占位符

```python
data = tf.placeholder(tf.float32, [None, 28*28], name='data')
label = tf.placeholder(tf.float32, [None, 10], name='data')
```

因为他们的  op.name 均是相同的所以，output_index就变得十分重要，他表示是同名变量的第几个。

但是实际上，在tensorflow后面的版本中，已经有所更改，

既不允许有同名节点，也就不会有同名tensor了，当自己声明的两个node同名称时，会在后面默认追加数字，1、2、3、等等，如下：

```python
import tensorflow as tf
 
print(tf.__version__)
 
data = tf.placeholder(tf.float32, [None, 28*28],name="data")
label = tf.placeholder(tf.float32, [None, 10],name="data")
 
with tf.Session() as sess:
    print('data tensor name is : %s, Op name is : %s' %(data.name, data.op.name))
    print('label tensor name is : %s, Op name is : %s' % (label.name, label.op.name))
    print("======================================================================")
    # 遍历图中的所有节点
    node_name_list = [node.name for node in tf.get_default_graph().as_graph_def().node]
    for node_name in node_name_list:
        print(node_name)
'''
data tensor name is : data:0, Op name is : data
label tensor name is : data_1:0, Op name is : data_1
======================================================================
data
data_1
'''
```

## 全文总结：

> （1）注意区分node（operation）与tensor的本质区别
>
> （2）注意tensor_name与node_name的区别，注意tensor_name的格式：“node_name:index”
>
> （3）注意tensor_name与node_name的命名规则，特别是出现了同名operation的时候怎么处理
>
> （4）建议：编写神经网络的时候，为了便于管理各个变量，权重、偏置、输入、输出等等，最好起一个易于辨识的名称，不要使用默认名称，这样跟方便实用。