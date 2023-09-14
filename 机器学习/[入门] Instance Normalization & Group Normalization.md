# Instance Normalization & Group Normalization

[BN、LN、IN、GN的简介 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/91965772)

神经网络中有各种归一化算法：Batch Normalization (BN)、Layer Normalization (LN)、Instance Normalization (IN)、Group Normalization (GN)。

从公式看它们都差不多：无非是减去均值，除以标准差，再施以线性映射

Batch Normalization 的一个缺点是**需要较大的 batchsize 才能合理估训练数据的均值和方差(横向计算)，这导致内存很可能不够用**，同时它也**很难应用在训练数据长度不同的 RNN 模型上**。Layer Normalization (LN) 的一个优势是不需要批训练，在单条数据内部就能归一化。

![img](https://pic2.zhimg.com/80/v2-24779aaa6b8fde97e5e19bbda99f3775_1440w.webp)



LN是指对同一张[图片](https://www.zhihu.com/search?q=图片&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra={"sourceType"%3A"answer"%2C"sourceId"%3A607608890})的同一层的所有通道进行Normalization操作。

![img](https://picx.zhimg.com/80/v2-f8df6b9a4c8b4d39a3521f3738a9dff4_1440w.webp?source=1940ef5c)

### 

GN是指对同一张图片的同一层的某几个（**不是全部**）通道一起进行Normalization操作。这几个通道称为一个Group。

![img](https://pica.zhimg.com/80/v2-4d4320b6e285c108f8e11bea650a3d32_1440w.webp?source=1940ef5c)





