# Graph 和 GraphDef

摘自：http://t.zoukankan.com/gnivor-p-13747024.html

从前端(python) 描述神经网络的结构，到后端在多机和分布式系统上部署，到底层 Device(CPU、GPU、TPU)上运行，都是基于图来完成.

然而在实际使用过程中遇到了三对API，

```python
[1] - tf.train.Saver()/saver.restore()

[2] - export_meta_graph/Import_meta_graph

[3] - tf.train.write_graph()/tf.Import_graph_def()
```

它们都是用于**对图的保存和恢复**.

同一个计算框架，为什么需要三对不同的API呢？他们保存/恢复的图在使用时又有什么区别呢？

初学的时候，常常闹不清楚他们的区别，以至常常写出了错误的程序，经过一番研究，本文中对Tensorflow中围绕Graph的核心概念进行了总结.



## Graph

首先介绍一下关于 TensorFlow 中 `Graph` 和它的序列化表示 `Graph_def`.

在 TensorFlow 官方文档中，Graph 被定义为 “一些 Operation 和 Tensor 的集合”.

例如表达如下的一个计算的 python代码，

```python
import tensorflow as tf

a = tf.placeholder(tf.float32)
b = tf.placeholder(tf.float32)
c = tf.placeholder(tf.float32)
d = a*b+c
e = d*2
```

就会生成相应的一张图，在Tensorboard中看到的图大概如图：

![img](https://img2020.cnblogs.com/blog/689056/202009/689056-20200928212234042-973085424.png)



其中，每一个圆圈表示一个 Operation(输入处为Placeholder)，椭圆到椭圆的边为Tensor，箭头的指向表示了这张图 Operation 输入输出 Tensor 的传递关系.

在真实的 TensorFlow 运行中，Python 构建的“图Graph” 并不是启动一个 Session 之后始终不变的. 因为 TensorFlow 在运行时，真实的计算会被分配到多CPUs，或 GPUs，或 ARM 等，以进行高性能/能效的计算. 单纯使用 Python 肯定是无法有效完成的.

实际上，**TensorFlow 是首先将 python 代码所描绘的图转换(即“序列化”)成 Protocol Buffer，再通过 C/C++/CUDA 运行 Protocol Buffer 所定义的图**.



## GraphDef

从 python Graph中序列化出来的图就叫做 GraphDef (这是一种不严格的说法，先这样进行理解).

而 GraphDef 又是由许多叫做 NodeDef 的 Protocol Buffer 组成. 在概念上 NodeDef 与(Python Graph 中的) Operation 相对应.

如下就是 GraphDef 的 ProtoBuf，由许多node 组成的图表示. 这是与上文 Python 图对应的 GraphDef：

```nginx
node {
  name: "Placeholder"    # 注：这是一个叫做 "Placeholder" 的node
  op: "Placeholder"
  attr {
    key: "dtype"
    value {
      type: DT_FLOAT
    }
  }
  attr {
    key: "shape"
    value {
      shape {
        unknown_rank: true
      }
    }
  }
}
node {
  name: "Placeholder_1" # 注：这是一个叫做 "Placeholder_1" 的node
  op: "Placeholder"
  attr {
    key: "dtype"
    value {
      type: DT_FLOAT
    }
  }
  attr {
    key: "shape"
    value {
      shape {
        unknown_rank: true
      }
    }
  }
}
node {
  name: "mul"          # 注：一个 Mul（乘法）操作
  op: "Mul"
  input: "Placeholder" # 使用上面的node（即Placeholder和Placeholder_1）
  input: "Placeholder_1" # 作为这个Node的输入
  attr {
    key: "T"
    value {
      type: DT_FLOAT
    }
  }
}
```

以上三个 NodeDef 定义了两个 Placeholde r和一个Multiply.

Placeholder 通过 attr(attribute的缩写)来定义数据类型和 Tensor 的形状.

Multiply 通过 input 属性定义了两个 placeholder 作为其输入.

无论是 Placeholder 还是 Multiply 都没有关于输出(output)的信息.

其实 Tensorflow 中都是通过 Input 来定义 Node 之间的连接信息.

那么既然 tf.Operation 的序列化 ProtoBuf 是 NodeDef，那么 tf.Variable 呢？在这个 GraphDef 中只有网络的连接信息，却没有任何 Variables呀？

没错，Graphdef 中不保存任何 Variable 的信息，所以如果从 graph_def 来构建图并恢复训练的话，是不能成功的.

如，

```python
with tf.Graph().as_default() as graph:
  tf.import_graph_def("graph_def_path")
  saver= tf.train.Saver()
  with tf.Session() as sess:
    tf.trainable_variables()
```

其中` tf.trainable_variables()` 只会返回一个空的list.`tf.train.Saver()` 也会报告 no variables to save.

然而，在实际线上 inference 中，通常就是使用 GraphDef. 但，GraphDef 中连 Variable都没有，怎么存储 weight 呢？

原来, GraphDef 虽然不能保存 Variable，但可以保存 Constant. 通过 tf.constant 将 weight 直接存储在 NodeDef 里，tensorflow 1.3.0 版本也提供了一套叫做 freeze_graph 的工具来自动的将图中的 Variable 替换成 constant 存储在 GraphDef 里面，并将该图导出为 Proto.

`tf.train.write_graph()`/`tf.Import_graph_def()` 就是用来进行 GraphDef 读写的API. 那么，我们怎么才能从序列化的图中，得到 Variables呢？这就要学习下一个重要概念，MetaGraph.



## MetaGraph

[Meta](https://www.tensorflow.org/versions/r1.1/programmers_guide/meta_graph）进一步解释说，Meta) Graph在具体实现上就是一个 MetaGraphDef (同样是由 Protocol Buffer来定义的). 其包含了四种主要的信息，根据Tensorflow官网，这四种 Protobuf 分别是:

```sh
[1] - MetaInfoDef，存一些元信息(比如版本和其他用户信息)
[2] - GraphDef， MetaGraph 的核心内容之一
[3] - SaverDef，图的Saver信息（比如最多同时保存的check-point数量，需保存的Tensor名字等，但并不保存Tensor中的实际内容）
[4] - CollectionDef，任何需要特殊注意的 Python 对象，需要特殊的标注以方便import_meta_graph 后取回(如 train_op, prediction 等等)
```

在以上四种 ProtoBuf 里面，[1] 和 [3] 都比较容易理解，[2] 刚刚总结过. 这里特别要讲一下 Collection(CollectionDef是对应的ProtoBuf).

​	那么，从 Meta Graph 中恢复构建的图可以被训练吗？是可以的. TensorFlow 的官方文档 https://www.tensorflow.org/api_guides/python/meta_graph 说明了使用方法. 这里要特殊的说明一下，Meta Graph 中虽然包含 Variable 的信息，却没有 Variable 的实际值. 所以, 从Meta Graph 中恢复的图，其训练是从随机初始化的值开始的. 训练中 Variable的实际值都保存在 checkpoint 中，如果要从之前训练的状态继续恢复训练，就要从checkpoint 中 restore. 进一步读一下 Export Meta Graph 的代码，可以看到，事实上variables 并没有被 export 到 meta_graph 中.



## 总结

TensorFlow 三种 API 所保存和恢复的图是不一样的.

简而言之，TensorFlow 在前端 Python 中构建图，并且通过将该图序列化到 ProtoBuf GraphDef，以方便在后端运行. 在这个过程中，图的保存、恢复和运行都通过 ProtoBuf 来实现. GraphDef，MetaGraph，以及Variable，Collection 和 Saver 等都有对应的 ProtoBuf 定义. ProtoBuf 的定义也决定了用户能对图进行的操作. 例如用户只能找到 Node的前一个Node，却无法得知自己的输出会由哪个Node接收.



















