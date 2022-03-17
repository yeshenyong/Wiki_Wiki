# Zset 实现排行榜功能

1.前言

之前写过一篇博客，讲解的是Redis的5种数据结构及其常用命令，当时有读者评论，说希望了解下这5种数据结构各自的使用场景，不过一直也没来得及写。

碰巧，在3月份找工作面试时，有个面试官先问了我Redis有哪几种数据结构，在我讲完后，面试官又问了我以下问题：

如何用Redis实现微信步数排行榜?

相信很多小伙伴都知道，可以使用 Redis 的有序集合 ZSET 来实现，本篇博客就基于此面试题，来讲解下 ZSET 的使用场景，以及微信步数排行榜的大致实现思路。

## 2. ZSET的使用场景

ZSET 的经典使用场景是用来实现排行榜，举几个常见的例子，比如百度热榜：



![img](https://pic2.zhimg.com/80/v2-f4b2b650c77857d13ec1cf28df7be5a9_720w.jpg)



比如微博热搜榜：



![img](https://pic3.zhimg.com/80/v2-812de561447ddb40005b90f9903f27e6_720w.jpg)



比如微信步数排行榜：



![img](https://pic3.zhimg.com/80/v2-30a8e36c264455746fe8c8d09eb52d82_720w.jpg)



3个场景的实现思路基本一致，接下来，我们以微信步数排行榜为例，了解下如何使用Redis的 ZSET ，实现微信步数排行榜。

## 3. 微信步数排行榜的大致实现思路

注意事项：本文的重点是Redis的 ZSET 的使用，因此只是分析了微信步数排行榜的大致实现思路，实际实现肯定比文中分析的复杂的多。

首先，我们来分析下微信步数排行榜的需求：

1. 排行榜是以**日期**为单位的，**历史日期的排行榜是可以查看的**
2. **排行榜可能并不会显示所有好友的步数**，**比如我的微信有349位好友，但排行榜从来没有显示过这么多，假设最多只显示步数前200的好友**
3. **步数是异步更新的**，**所以每隔一段时间步数同步后，排行榜都会变化**
4. 排行榜中，**好友头像和微信昵称可以理解为不变的（变动的几率小，就像热搜榜中的标题和Url），但步数和点赞数是可变的**

基于以上分析的需求，大致实现思路如下：

1. 使用Redis的 ZSET 数据结构
2. 设置key时，基于微信号和日期，比如我的微信是zwwhnly，今天的日期是2020-06-01，那么key就可以设计为：StepNumberRanking:zwwhnly:20200601
3. 设置value时，将好友的昵称作为成员member，将好友的步数作为分值score，如下所示：
4. 使用Redis的 HASH 数据结构，其中key为第2步的key+第3步的成员member，value分别存储好友头像、昵称、步数、点赞数，如下所示：
5. 获取微信步数排行榜时，分为以下2步：1)先查询出微信步数排行榜中的好友昵称，也就是查询StepNumberRanking:zwwhnly:20200601的值2）根据获取到的好友昵称，查询好友步数信息，也就是查询StepNumberRanking:zwwhnly:20200601:yst的值

## 4. 使用到的Redis命令

上面分析出了大致的实现思路，接下来我们讲解下使用到的Redis命令。

### 4.1 ZADD

执行如下命令初始化微信步数排行榜，以上面图片中的9个好友为例，分2次初始化：

```text
ZADD StepNumberRanking:zwwhnly:20200602 25452 yst 23683 zq 23599 ljx 20391 yyq 19628 XxZz

ZADD StepNumberRanking:zwwhnly:20200602 18261 lxx 16636 zcc 16555 clc 16098 fl
```



![img](https://pic4.zhimg.com/80/v2-9d72b3b5718b7fb8d4a79af1d897bea7_720w.jpeg)



执行完的效果如下图所示：



![img](https://pic3.zhimg.com/80/v2-e15c7d68afdf35239cde5e7bde6d85e2_720w.jpg)



可以看到，默认是以score正序排列的，也就是步数从少到多排列。

### 4.2 HMSET

因为展示步数排行榜时，需要展示昵称、头像、步数、点赞数，所以可以借助于Redis的 HASH数据结构来存储，这时就要用到 HMSET 命令：



![img](https://pic4.zhimg.com/80/v2-0e29dce8fa811a33989fcc794497c4e3_720w.jpg)



执行完的效果如下图所示：



![img](https://pic3.zhimg.com/80/v2-3c54e44a1da6df59a4bf195bccf1ab5a_720w.jpg)





![img](https://pic1.zhimg.com/80/v2-f96b15096e313ce38b167bf6d7faaa50_720w.jpeg)



### 4.3 ZINCRBY

每隔一段时间，好友的步数是会更新的，此时可以使用 ZINCRBY 命令来更新好友步数，假设我们只更新步数位于前2位好友的步数，给他们的步数增加10，就可以执行以下命令：

```text
ZINCRBY StepNumberRanking:zwwhnly:20200602 10 yst

ZINCRBY StepNumberRanking:zwwhnly:20200602 10 zq
```



![img](https://pic1.zhimg.com/80/v2-00fd199b1f43e8babd9c43b4c68ab2b8_720w.jpg)



执行完的效果如下图所示：



![img](https://pic3.zhimg.com/80/v2-d12d5f158065976eee349660a03134da_720w.jpg)



更新完排行榜里的步数后，不要忘记执行 HMSET 命令更新好友的步数：



![img](https://pic3.zhimg.com/80/v2-09fdb394ab9bba0fdf265f3595bf5e4e_720w.jpeg)





![img](https://pic1.zhimg.com/80/v2-bfa59c672db9b7ef25563fa49e0f59e8_720w.jpg)





![img](https://pic1.zhimg.com/80/v2-f6f70f8e66d3cc74505b3e94f9f10960_720w.jpeg)



### 4.4 HINCRBY

当我们在步数排行榜里给好友点赞时，可以使用 HINCRBY 命令，把上图中的likeNum加1：

```text
HINCRBY StepNumberRanking:zwwhnly:20200602:zq likeNum 1
```



![img](https://pic1.zhimg.com/80/v2-bd8701945d231b4114745410604c6570_720w.jpeg)





![img](https://pic2.zhimg.com/80/v2-fe1f261b46cc62235e718bc002e100e9_720w.jpg)



### 4.5 ZRANGE

在所有的数据就绪后，剩下的就是查询了，我们可以使用 ZRANGE 命令获取排行榜里的好友信息：

```text
ZRANGE StepNumberRanking:zwwhnly:20200602 0 -1
```



![img](https://pic3.zhimg.com/80/v2-1721c3c64dc47c6db0ab8b1e96d62882_720w.jpg)



可以看出，查询出的好友信息是按步数从少到多排序的，而排行榜应该按步数从多到少排序，这就用到了下面的 ZREVRANGE 命令。

### 4.6 ZREVRANGE

ZREVRANGE 命令和 ZRANGE 命令类似，不过是按score倒序的，刚好符合排行榜的场景。

比如执行命令：

```text
ZREVRANGE StepNumberRanking:zwwhnly:20200602 0 -1 WITHSCORES
```



![img](https://pic1.zhimg.com/80/v2-2aef838a3cb3fcaa140cb7a1f3133618_720w.jpg)



可以看出，查询出的好友信息按步数从大到小排序，刚好就是在排行榜要展示的顺序。

不过，排行榜一般都不展示所有的数据，这里我们的数据比较少，如果只获取步数top5的好友，就可以执行如下命令：

```text
ZREVRANGE StepNumberRanking:zwwhnly:20200602 0 4 WITHSCORES
```



![img](https://pic3.zhimg.com/80/v2-dc3c2ecb19a0e1718a341c1e4757c006_720w.jpg)



如果你要获取top200，就将上面的4修改为199。

### 4.7 HGETALL

获取到了排行榜里的好友信息，最后一步就是获取这些好友的步数、点赞数、头像、昵称这些信息，也就是我们之前使用 HASH 数据结构存储的信息，此时我们可以使用 HGETALL 命令，如下所示：

```text
HGETALL StepNumberRanking:zwwhnly:20200602:yst
```



![img](https://pic2.zhimg.com/80/v2-38d888ba0aa2e62fa217ca41a92af1dd_720w.jpg)



如果对这些命令不是很熟悉，可以看下我之前发布的一篇博客： **Redis系列(二)：Redis的5种数据结构及其常用命令** 。

## 5. 总结

Redis的 ZSET 数据结构非常适合用在排行榜的场景，比如百度热搜、微博热搜榜、游戏排行榜、微信步数排行榜，面试官肯定不会问你ZSET都有哪些命令，每个命令的细节等等，但问你如何使用Redis实现微信步数排行榜，就可以了解到你对Redis数据结构的掌握程度。

所以，学习好Redis的5种数据结构的基础很重要，但更重要的是要知道这些数据结构如何使用，每种数据结构用在什么场景最为合适，毕竟要学以致用嘛。

注：如果觉得本篇博客有任何错误或者更好的建议，欢迎留言，我会及时跟进并更正博客内容！

















