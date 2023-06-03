# 大道至简：算法工程师炼丹Trick手册

转自：[大道至简：算法工程师炼丹Trick手册 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/352971645)

## **Focal Loss**

![img](https://pic1.zhimg.com/80/v2-6741afcf4819774e313ae3a2cac2d190_1440w.webp)

针对类别不平衡问题，用预测概率对不同类别的loss进行加权。Focal loss对CE loss增加了一个调制系数来降低容易样本的权重值，使得训练过程更加关注困难样本。

```python
loss = -np.log(p) 
loss = (1-p)^G * loss
```

## Dropout

![img](https://pic2.zhimg.com/80/v2-2f3a5e2842073ea699fad5d7b3c5d785_1440w.webp)

随机丢弃，抑制过拟合，提高模型鲁棒性。

## **Normalization**

Batch Normalization 于2015年由 Google 提出，开 Normalization 之先河。其规范化针对单个神经元进行，利用网络训练时一个 mini-batch 的数据来计算该神经元的均值和方差,因而称为 Batch Normalization。

```python3
x = (x - x.mean()) / x.std()
```

## relu

![img](https://pic2.zhimg.com/80/v2-c984bb0e4d58d0b7f7a5ba90cf2006cd_1440w.webp)

用极简的方式实现非线性激活，缓解梯度消失。

```text
x = max(x, 0)
```

## **Cyclic LR**

![img](https://pic3.zhimg.com/80/v2-0a0bb3099d71845a290e481447995c72_1440w.webp)

每隔一段时间重启学习率，这样在单位时间内能收敛到多个局部最小值，可以得到很多个模型做集成。

```text
scheduler = lambda x: ((LR_INIT-LR_MIN)/2)*(np.cos(PI*(np.mod(x-1,CYCLE)/(CYCLE)))+1)+LR_MIN
```

## **With Flooding**

![img](https://pic4.zhimg.com/80/v2-dfff1c8958c99433be3d98a19d0dee9f_1440w.webp)

当training loss大于一个阈值时，进行正常的梯度下降；当training loss低于阈值时，会反过来进行梯度上升，让training loss保持在一个阈值附近，让模型持续进行“random walk”，并期望模型能被优化到一个平坦的损失区域，这样发现test loss进行了double decent。

```text
flood = (loss - b).abs() + b
```

## Group Normalization

![img](https://pic4.zhimg.com/80/v2-bc3a9f69253ac87e0950500119d47c87_1440w.webp)

Face book AI research（FAIR）吴育昕-恺明联合推出重磅新作Group Normalization（GN），提出使用Group Normalization 替代深度学习里程碑式的工作Batch normalization。一句话概括，Group Normbalization（GN）是一种新的深度学习归一化方式，可以替代BN。

```text
def GroupNorm(x, gamma, beta, G, eps=1e-5):
    # x: input features with shape [N,C,H,W]
    # gamma, beta: scale and offset, with shape [1,C,1,1]
    # G: number of groups for GN
    N, C, H, W = x.shape
    x = tf.reshape(x, [N, G, C // G, H, W])
    mean, var = tf.nn.moments(x, [2, 3, 4], keep dims=True)
    x = (x - mean) / tf.sqrt(var + eps)
    x = tf.reshape(x, [N, C, H, W])
    return x * gamma + beta
```

## **Label Smoothing**

![img](https://pic2.zhimg.com/80/v2-79d513b2b9993223cdc4a53a38d4c2d9_1440w.webp)

![img](https://pic3.zhimg.com/80/v2-736fe8a9d9f79dd0db990ba3a64a9c8a_1440w.webp)

abel smoothing将hard label转变成soft label，使网络优化更加平滑。标签平滑是用于深度神经网络（DNN）的有效正则化工具，该工具通过在均匀分布和hard标签之间应用加权平均值来生成soft标签。它通常用于减少训练DNN的过拟合问题并进一步提高分类性能。

```text
targets = (1 - label_smooth) * targets + label_smooth / num_classes
```

![img](https://pic2.zhimg.com/80/v2-f6614a7035b1823d35aa6a4e14aeeefd_1440w.webp)

## Wasserstein GAN

![img](https://pic1.zhimg.com/80/v2-5061e1f191f791e1d3ce8a9093530fd8_1440w.webp)

- 彻底解决GAN训练不稳定的问题，不再需要小心平衡生成器和判别器的训练程度
- 基本解决了Collapse mode的问题，确保了生成样本的多样性
- 训练过程中终于有一个像交叉熵、准确率这样的数值来指示训练的进程，数值越小代表GAN训练得越好，代表生成器产生的图像质量越高
- 不需要精心设计的网络架构，最简单的多层全连接网络就可以做到以上3点。

## **Skip Connection**

一种网络结构，提供恒等映射的能力，保证模型不会因网络变深而退化。

```text
F(x) = F(x) + x
```

























