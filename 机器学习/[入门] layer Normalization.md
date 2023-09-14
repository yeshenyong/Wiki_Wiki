# layer Normalization

[模型优化之Layer Normalization - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/54530247)

![img](https://pic1.zhimg.com/80/v2-c039daa05cd9d5c3936c4513422690b0_1440w.webp)



> batch Normalization：以batch 为单位
>
> layer Normalization：以特征/单层MLP 神经元为单位

LN是和BN非常近似的一种归一化方法，**不同的是BN取的是不同样本的同一个特征**，**而LN取的是同一个样本的不同特征**。在BN和LN都能使用的场景中，**BN的效果一般优于LN，原因是基于不同数据，同一特征得到的归一化特征更不容易损失信息。**

但是有些场景是不能使用BN的，例如batchsize较小或者在RNN中，这时候可以选择使用LN，LN得到的模型更稳定且起到正则化的作用。RNN能应用到小批量和RNN中是因为LN的归一化统计量的计算是和batchsize没有关系的。



![img](https://pic4.zhimg.com/80/v2-81635298166c39d5d6342a9524086883_1440w.webp)