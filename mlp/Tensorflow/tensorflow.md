# tensorflow



梯度下降、全局最优、局部最优

https://www.bilibili.com/video/BV1Lx411j7ws?p=3

一个视频搞定



特征提取、特征（features）、代表特征（feature representation）

代表特征太多了人类可能看不懂，所以才会觉得神经网络是黑盒

迁移学习（transform learning），可能有重复其他项目神经网络训练步骤的加入

https://www.bilibili.com/video/BV1Lx411j7ws?p=4





tensorflow

张量飞









### tf.keras

本质上还是tensorflow

只是命名上参考了keras上的命名



### 为什么使用tensorflow

#### GPU加速

```python
gpu_a = tf.random.normal([10000, 1000])
gpu_b = tf.random.normal([1000, 2000])
c = tf.matmul(gpu_a, gpu_b)
# run time:
# cpu = 3.38s
# GPU = 0.0006s（对深度学习并行运算的优化）
with tf.device('/cpu:0'):
    
with tf.device('/gpu:0'):
```



#### 自动求导

#### 神经网络API





### 线性回归

linear regulation

```
y = w*x + b
```

考虑噪声，噪声需要符合高斯分布



确定目标、定义损失函数

问题变成如何去minimize 这个损失函数的极小值

#### gradient descent

梯度下降











