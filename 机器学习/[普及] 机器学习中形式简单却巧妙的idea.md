# 机器学习中形式简单却巧妙的idea



[(7 条消息) 机器学习中有哪些形式简单却很巧妙的idea？ - 知乎 (zhihu.com)](https://www.zhihu.com/question/347847220/answer/3099048589)



### Label Smoothing

![img](https://pica.zhimg.com/80/v2-79d513b2b9993223cdc4a53a38d4c2d9_1440w.webp?source=1940ef5c)

![img](https://picx.zhimg.com/80/v2-736fe8a9d9f79dd0db990ba3a64a9c8a_1440w.webp?source=1940ef5c)

[abel smoothing](https://www.zhihu.com/search?q=abel smoothing&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra={"sourceType"%3A"answer"%2C"sourceId"%3A3099048589})将hard label转变成[soft label](https://www.zhihu.com/search?q=soft label&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra={"sourceType"%3A"answer"%2C"sourceId"%3A1799536256})，使网络优化更加平滑。标签平滑是用于[深度神经网络](https://www.zhihu.com/search?q=深度神经网络&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra={"sourceType"%3A"answer"%2C"sourceId"%3A1799536256})（DNN）的有效正则化工具，该工具通过在均匀分布和hard标签之间应用加权平均值来生成soft标签。它通常用于减少训练DNN的[过拟合](https://www.zhihu.com/search?q=过拟合&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra={"sourceType"%3A"answer"%2C"sourceId"%3A3099048589})问题并进一步提高分类性能。

```python
targets = (1 - label_smooth) * targets + label_smooth / num_classes
```

![img](https://picx.zhimg.com/80/v2-f6614a7035b1823d35aa6a4e14aeeefd_1440w.webp?source=1940ef5c)





### **Skip Connection**

一种网络结构，提供[恒等映射](https://www.zhihu.com/search?q=恒等映射&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra={"sourceType"%3A"answer"%2C"sourceId"%3A1799536256})的能力，保证模型不会因网络变深而退化（有点像残差）

```python
F(x) = F(x) + x
```



### Focal Loss

### Dropout

### **Normalization**

Batch Normalization 于2015年由 Google 提出，开 Normalization 之先河。其规范化针对单个神经元进行，利用网络训练时一个 mini-batch 的数据来计算该神经元的均值和方差,因而称为 Batch Normalization。

```python
x = (x - x.mean()) / x.std()
```

### relu

![img](https://pic1.zhimg.com/80/v2-c984bb0e4d58d0b7f7a5ba90cf2006cd_1440w.webp?source=1940ef5c)