# [入门算法] transformer B 站入门



原视频链接:https://www.bilibili.com/video/BV1Di4y1c7Zm



## Encoder

1. 输入部分
2. 注意力机制
3. 前馈神经网络

### 位置编码

> > NLP 领域
>
> 1. 位置嵌入是有用的
> 2. 绝对位置信息
> 3. 相对位置信息
> 4. 可学习位置信息
>
> > 我爱你
> >
> > 相对于 "你"（绝对值2，相对于"我"，相对值为1）
> >
> > 绝对位置信息 + 相对位置信息

![img](https://pic1.zhimg.com/v2-8c63aaf7e71b94fdb5d6df89abdaf118_r.jpg)

### 注意力机制

注意力机制公式：
$$
Attention(Q,K,V) = softmax(\frac{QK^T} {\sqrt d_k)}) V
$$


Q、K、V 三个矩阵为必须（就是三个可学习的矩阵），K 向量维度的平方根（为了梯度稳定）

#### self-attention

> x1 之前可以是经过位置编码之后嘚

![img](https://pic3.zhimg.com/v2-15142b393f03a309c926754f00307d46_r.jpg)

![img](https://pic2.zhimg.com/v2-42ccd93ac7540619b02ef03faef21c15_r.jpg)

![img](https://pic3.zhimg.com/v2-8a98e66c20fb25e96e1f690309ae6166_r.jpg)

![img](https://pic3.zhimg.com/v2-1701b674a3e09ae91301d6cd9727f912_r.jpg)

![img](https://pic3.zhimg.com/v2-3577071e71ccfa49a4f60f4a5187f0ce_r.jpg)

![img](https://pic4.zhimg.com/v2-0190eb46d1c46efc04926821e69fd377_r.jpg)

#### 多头self-attention

第一个方面，他扩展了模型关注不同位置的能力，这对翻译一下句子特别有用，因为我们想知道“it”是指代的哪个单词。

![img](https://pic1.zhimg.com/v2-dc386abf38141384c43918689b0bbb64_r.jpg)

第二个方面，他给了自注意力层多个“表示子空间”。对于多头自注意力机制，我们不止有一组Q/K/V权重矩阵，而是有多组（论文中使用8组），所以每个编码器/解码器使用8个“头”（可以理解为8个互不干扰自的注意力机制运算），每一组的Q/K/V都不相同。然后，得到8个不同的权重矩阵Z，每个权重矩阵被用来将输入向量投射到不同的表示子空间。

经过多头注意力机制后，就会得到多个权重矩阵Z，我们将多个Z进行拼接就得到了self-attention层的输出：

![img](https://pic2.zhimg.com/v2-1be30f537678c89b2768ed31ff5bb491_r.jpg)



上述我们经过了self-attention层，我们得到了self-attention的输出，self-attention的输出即是前馈神经网络层的输入，然后前馈神经网络的输入只需要一个矩阵就可以了，不需要八个矩阵，所以我们需要把这8个矩阵压缩成一个，我们怎么做呢？只需要把这些矩阵拼接起来然后用一个额外的权重矩阵与之相乘即可。

![img](https://pic4.zhimg.com/v2-7394f6eb418b403588b0ca5a6751749f_r.jpg)

最终的Z就作为前馈神经网络的输入。

疑问：

> 为什么要除于根号d_k
>
> 因为Q*K^T 可能存在很大的值，并且softmax 反向传播梯度很小，会存在梯度消失，控制方差为1



> 单头注意力机制与多头注意力机制
>
> > 多头走完，各自的softmax走完，再合在一起输出
>
> 1. 原始信息从一个空间，变成了多个空间（一套参数 => 多套参数），放在不同的空间，捕捉不同子空间信息，特征信息
> 2. 更好







### 残差网络

> 梯度消失一般情况下：链式连乘
>
> 残差网络
>
> 1. 缓解梯度消失的出现（不会出现0 的情况，因为后面会相加）
> 2. MLP 用了残差网络可以变得比较深的原因

残差网络ResNet

原始残差网络：

![img](https://pic3.zhimg.com/v2-39dabdf70ac19c0c92b86d53375d8c06_r.jpg)

在transformer 的运用：

![img](https://pic4.zhimg.com/v2-eafcf1c4c21becd37f3c7fcb4ec19723_r.jpg)

 其中Add代表了Residual Connection，是为了解决多层神经网络训练困难的问题，通过将一部分的前一层的信息无差的传递到下一层，可以有效的提升模型性能——因为对于有些层，我们并不确定其效果是不是正向的。加了残差连接之后，我们相当于将上一层的信息兵分两路，一部分通过我们的层进行变换，另一部分直接传入下一层，再将这两部分的结果进行相加作为下一层的输出。这样的话，其实可以达到这样的效果：我们通过残差连接之后，就算再不济也至少可以保留上一层的信息，这是一个非常巧妙的思路。



### Layer Normalization

> LN：同一样本，不同特征，归一化
>
> BN：不同样本，同一特征，归一化

> BN 效果差，所以不用（BN 在MLP 很差，不用）
>
> batch 小的时候，效果差
>
> > 使用batch 的均值和方差，来代表整个样本的均值和方差
>
> 在RNN 效果比较差

话外题

> Feature Scaling（量纲）
>
> 为了模型更快的去收敛，对特征一些处理



> 句向量，通常会做加权的词向量



## Decoder

Masked（very important）

> 需要对当前单词和之后的单词做mask
>
> > 这里的道理可以引用推荐的一些信息进行解释（不能引入未来信息作为训练数据，因为预测的时候并没有这个信息，这两者存在gap）
> >
> > 比如：
> >
> > 我知道了，I love you now！训练you 的时候 i、love、you、now 都传进来训练，但是预测的时候并没有you 和now。因为是一个个产生的，所以这两者就存在gap 了（预测的时候没有ground truth）相当于作弊了

![img](https://pic4.zhimg.com/v2-5e32534b9a651289cb3eb2b409d5996b_r.jpg)

![img](https://pic1.zhimg.com/v2-1d9129c9c0d5367591bd093f79155e40_r.jpg)

![img](https://pic4.zhimg.com/v2-8fbde14eac35db43cfe1734d4714a7db_r.jpg)

Transformer就介绍到这里了，后来的很多经典的模型比如BERT、GPT-2都是基于Transformer的思想。我们有机会再详细介绍这两个刷新很多记录的经典模型



面试题

摘自原作者知乎：[史上最全Transformer面试题系列（一）：灵魂20问帮你彻底搞定Transformer-干货！ - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/148656446)

太干货了！！

> 1. Transformer为何使用多头注意力机制？（为什么不使用一个头）
> 2. Transformer为什么Q和K使用不同的权重矩阵生成，为何不能使用同一个值进行自身的点乘？ （注意和第一个问题的区别）
> 3. Transformer计算attention的时候为何选择点乘而不是加法？两者计算复杂度和效果上有什么区别？
> 4. 为什么在进行softmax之前需要对attention进行scaled（为什么除以dk的平方根），并使用公式推导进行讲解
> 5. 在计算attention score的时候如何对padding做mask操作？
> 6. 为什么在进行多头注意力的时候需要对每个head进行降维？（可以参考上面一个问题）
> 7. 大概讲一下Transformer的Encoder模块？
> 8. 为何在获取输入词向量之后需要对矩阵乘以embedding size的开方？意义是什么？
> 9. 简单介绍一下Transformer的位置编码？有什么意义和优缺点？
> 10. 你还了解哪些关于位置编码的技术，各自的优缺点是什么？
> 11. 简单讲一下Transformer中的残差结构以及意义。
> 12. 为什么transformer块使用LayerNorm而不是BatchNorm？LayerNorm 在Transformer的位置是哪里？
> 13. 简答讲一下BatchNorm技术，以及它的优缺点。
> 14. 简单描述一下Transformer中的前馈神经网络？使用了什么激活函数？相关优缺点？
> 15. Encoder端和Decoder端是如何进行交互的？（在这里可以问一下关于seq2seq的attention知识）
> 16. Decoder阶段的多头自注意力和encoder的多头自注意力有什么区别？（为什么需要decoder自注意力需要进行 sequence mask)
> 17. Transformer的并行化提现在哪个地方？Decoder端可以做并行化吗？
> 18. 简单描述一下wordpiece model 和 byte pair encoding，有实际应用过吗？
> 19. Transformer训练的时候学习率是如何设定的？Dropout是如何设定的，位置在哪里？Dropout 在测试的需要有什么需要注意的吗？
> 20. 引申一个关于bert问题，bert的mask为何不学习transformer在attention处进行屏蔽score的技巧？



