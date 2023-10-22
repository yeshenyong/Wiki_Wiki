[NLP（六）：GPT 的张量并行化（tensor parallelism）方案 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/603908668)

> 有内存碎片，比如 4个18 GB的模型 未必能部在8张10 G 2080T 卡上
>
> 但是如果模型Tensor Parallelism 只分配两张卡，就可以部上去
>
> > 前者速度 6.5s / request 
> >
> > 后者速度 9s / request

![img](https://pic2.zhimg.com/80/v2-89b2451aed2a25f36b953f4e84d58ee5_720w.webp)



简而言之就是把一个变量分散到多个设备并共同完成某个或多个计算操作。对于单个 Tensor/Op 很大 或者模型很大（如GPT3, chatGPT等）的情况，Tensor parallelism 的重要性非常明显

实现 Tensor parallelism 的前提是计算设备需要处于互联状态，如上图所示，以GPU为例，因产品形态不同，有全连接和部分连接两种状态



### Embedding 的并行

![img](https://pic2.zhimg.com/80/v2-fb5c003699a4470777cf7a9f47cf1a2d_720w.webp)



### Self-attention 的并行

![img](https://pic4.zhimg.com/80/v2-63bf52a47547fb48b5aa844d55e3ea43_720w.webp)



### MLP 的并行

### LM-head 的并行

![img](https://pic4.zhimg.com/80/v2-d7a123db9a6f000b53e2598e771879ff_720w.webp)

### CrossEntropyLoss 的并行

前边讨论的主要是**推理**过程中的并行过程，如果在训练过程中还需要考虑loss的并行，为此笔者曾专门进行过详细讨论，有兴趣可参考下文，本文在此不予赘述

