[elasticSearch 是什么？工作原理是怎么样的？ (qq.com)](https://mp.weixin.qq.com/s/RUQXIyN95hvi2wM3CyPI9w)



自个：对于`elasticSearch`  对 `lucene` 搜索进行**高性能**、**高扩展**、**高可用**的封装提出问题

Q1：根据文章写到位了 `Index Name` 数据过多，引入了 `Shard` 的理念对数据进行分片，即每个 `Shard` 存储不同的内容（暂不考虑主从）。那进行扩容的时候，如何保证可扩展性？（读写数据 or 数据移动吗？成本过高）



Q2：在 `Primary shard`  和 `Replica shard` 之间同步当中，在 **协调节点(Coordinate Node)** 写入 `Primary shard` 的之后挂了（还未同步至 `Replica Shard`）

1. `Coordinate Node` 如何感知，并且切换至 `Replica Shard`
2. 当条数据，在写入 `Primary Shard` 之后才挂的，是不是就意味着这条数据 `Coordinate Node` 会继续请求 `Primary Shard` 获取，导致丢失（没有类似 `Redis` 的内存磁盘的恢复功能）
3. 当 `Primary Shard` 挂了之后， 协调节点如何识别，并且重新选举？期间读的数据是否就为空？



Q3：对于高性能的折中，`segament` 概念多读取导致多请求的问题与何时合并问题探究？合并如何感知，如何迭代新 `segament` 



- es 中用于分类消息的 `Index Name`，其实就是 kafka 的 `topic`。
- es 中用于对 Index Name 数据分片的 `Shard`，其实就是 kafka 中拆分 topic 数据的 `Partition`。
- es 中用于分散部署多个 shard 的 `Node`, 其实就相当于 kafka 中的 `broker`



![图片](https://mmbiz.qpic.cn/sz_mmbiz_jpg/icMUEqOiagpkgsLKfsjlIX6D9HaAFGuY8WmgIb7CHgibylAlSlNEKbV8VkBwIqna4ntqHS43wUAHiaPSOu4glFF0rg/640?wx_fmt=jpeg&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)



![图片](https://mmbiz.qpic.cn/sz_mmbiz_jpg/icMUEqOiagpkgsLKfsjlIX6D9HaAFGuY8WocWFpDO4s81F4l7nYibpI6YhaOg9cNG9ZG2kN4uoTaf87F2piaxj7CzQ/640?wx_fmt=jpeg&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

![图片](https://mmbiz.qpic.cn/sz_mmbiz_jpg/icMUEqOiagpkgsLKfsjlIX6D9HaAFGuY8Wfia0fI1ICwcSP7IBgibiapOGC7icB7chER6dFNoH9iataTfhSpOmrfCicc0Q/640?wx_fmt=jpeg&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

![图片](https://mmbiz.qpic.cn/sz_mmbiz_jpg/icMUEqOiagpkgsLKfsjlIX6D9HaAFGuY8Whv01zEibMPDLJCETGR1SxUpeomq7jq4PXD7GGgyA7e1w9tEicZzmv3qg/640?wx_fmt=jpeg&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

![图片](https://mmbiz.qpic.cn/sz_mmbiz_jpg/icMUEqOiagpkgsLKfsjlIX6D9HaAFGuY8W8yufZqLomcJeUeoZp6ByQ8qGEp6Lrs8OqoT5a3OBQrMu8G0fbicNvhA/640?wx_fmt=jpeg&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

![图片](https://mmbiz.qpic.cn/sz_mmbiz_jpg/icMUEqOiagpkgsLKfsjlIX6D9HaAFGuY8W44ut06icaHoncgq6UgiaXwpJ02QxnMw8iah3rHv4hsbzChwzWx7VdIAcg/640?wx_fmt=jpeg&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)