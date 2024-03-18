# LLaMa Factory 笔记







### 参数模块

- num_samples：每个数据集最大样本数
- Gradient accumulation：gradient_accumulation_steps是梯度累积次数，累积几次，原本的loss就要除以几，这是为了对多个批次的数据的梯度做累积。

> 举个例子来说，本来batchsize是16，那么一次梯度更新用的就是16条数据。但是根据上面的代码，只有step是gradient_accumulation_steps倍数的时候，参数才会更新，梯度才会重置。假如gradient_accumulation_steps是4，那么其实就是利用了64条数据，取64条数据的平均梯度来更新数据

#### LoRA 参数

- Dora： = L2(LoRA + linear)
- alpha：LoRA alpha 类似权重放大功能
- LoRA rank：LoRA 矩阵的秩 r
- LoRA modules(optional)：应用 LoRA 的模块名称。使用英文逗号分隔多个名称（q\k\v\o(ffn)）all 是全加
- 



