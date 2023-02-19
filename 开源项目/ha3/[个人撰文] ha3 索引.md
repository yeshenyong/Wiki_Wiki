# ha3 索引



正排索引

配置：https://github.com/alibaba/havenask/wiki/%E6%AD%A3%E6%8E%92%E7%B4%A2%E5%BC%95%E5%8E%8B%E7%BC%A9

压缩

多值排重

默认情况下，多值 Attribute 的值是按照文档顺序依次排列的（offset 是递增的）。实际应用中发现，多值存在着大量的重复，即很多个文档具有相同的 Attribute 值。打开多值排重功能，可以去除索引中重复值，从而缩小最终产出的索引大小。 此功能，除多值 Attribute 和 单值 String 外，还适用于 Section Attribute。 由于排重的过程，需要引入排重词典的等辅助数据结构，会增加 Build 和 Merge 的内存开销，所以，如果字段重复度很低时，不建议配置

等值压缩

实际应用中，单值 attribute 和多值 attribute 的 offset 值在按照某个字段全局排序后，相等的值经常连续出现，如果对连续出现的相等值数组进行压缩能够获得较大的存储空间收益。等值压缩就是通过用更少的 Bit 位来代表这些重复值，来压缩索引的。 此功能，适用于单值和多值 Attribute 的 Offset 文件。对于多值 Attribute （包括单值 String) 和 Section Attribute 可以同时使用多值排重和等值压缩

自适应Offset

由于每个多值 Attribute 都是单独存储的，Schema 中多少多值 Attribute 就要产生多少 Offset 文件，如果每个 Offset 都使用 8Bit 长度，将是一种极大的浪费。因此，我们支持了一种叫做自适应 Offset 的功能，当产生的 Data 文件长度不足 4GB 时，使用 4Bit 存储。 无需配置

pack attribute存储方式

可以将同一个文档的一组attribute进行打包按行存储。 在attribute数据使用过程中，由于紧密的存储对cpu数据缓存更加友好（减少cache miss），对一篇文档中的打包存储的attribute字段数据连续读取在性能表现上明显优于字段按列存储的方式

倒排索引

配置与介绍：https://github.com/alibaba/havenask/wiki/%E5%80%92%E6%8E%92%E7%B4%A2%E5%BC%95%E7%B1%BB%E5%9E%8B

摘要索引

配置：https://github.com/alibaba/havenask/wiki/%E6%91%98%E8%A6%81%E7%B4%A2%E5%BC%95

> 摘要索引将一片文档对应的信息存储在一起，通过docID可以定位该文档信息的存储位置，从而为用户提供摘要信息的存储获取服务。摘要索引的结构和正排索引类似,只是完成的功能不同 

向量索引

配置：https://github.com/alibaba/havenask/wiki/%E5%90%91%E9%87%8F%E7%B4%A2%E5%BC%95%E9%85%8D%E7%BD%AE

> 向量检索是指将图片或者内容等以向量的形式表达，并建立向量索引库，查询时支持输入一个或多个向量来根据向量距离返回topK个近似结果。
> 问天引擎深度集成了[proxima](https://github.com/alibaba/proxima)核心库，使问天引擎具有了向量检索的能力。问天引擎的向量索引支持图索引、聚类索引等多种主流的向量索引类型，并具有高性能、低成本的特点。以hnsw索引为例，在sift 1M和deep 10M公开测试集上，使用16core 64G的机器，问天引擎的向量索引可以达到数千qps和10ms以内的延迟。同时问天引擎的向量索引可以采用非全内存的模式加载，从而可以大大减少了对机器内存的开销，提升单机索引规模，降低机器成本















