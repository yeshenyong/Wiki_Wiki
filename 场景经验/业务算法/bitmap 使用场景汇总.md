# bitmap 使用场景汇总

真实于工作中使用（包括曾经尝试）



（一些背景经过例子处理）

### 场景一

背景：

对热点电影Top 5万的Tags 与用户请求画像的Tags 求差集，以做热点的兴趣探索

1. Top 5 万的电影中，每个电影都有2 ~ 5个Tags
2. 用户请求画像有10 ~ 50 个Tags



正常实现

```cpp
const int movie_nums = 50000;
vector <MoiveInfo> movie_container;

unordered_set<string> user_tags;

for (const auto& moive) {
    auto tags = moive.tags();
    int flag = true;
    for (auto& tag) {
        if (user_tags.find(tag) != user_tags.end()) {
            flag = false;
            break;
        }
    }
    if (flag) result.emplace_back(movie);
    // ...
}
```



如果用户画像丰富、基本要做`unordered_set`  的几万次 `find` 操作

解决方法：

1. 多线程处理后，合并（最后采取的方法）
2. `bitmap`  近线缓存优化查询次数



讲第二种

构造bitmap

前提：5万个movie，每个movie 有2 ~ 5个tags

方法：构造每个tags 不属于的映射，用5 万个bit 存5 万个movie

> 例子：（暂定三部电影 ，只需要三个bit）
>
> 泰坦尼克号 爱情 浪漫
>
> 环太平洋 动作 机甲
>
> 变形金刚 动作 机甲
>
> map["爱情"] = 011	# 只有环太平洋和变形金刚没有爱情标签
>
> map["浪漫"] = 011	# 只有环太平洋和变形金刚没有浪漫标签
>
> map["动作"] = 100	# 只有泰坦尼克号没有动作和机甲标签
>
> map["机甲"] = 100	# 只有泰坦尼克号没有动作和机甲标签

计算方法：

> 沿用上述例子
>
> 1. 用户有爱情标签
>
>    > 直接取map["爱情"] 标签，里面就是包括除了爱情标签的电影
>
> 2. 用户有动作和爱情标签
>
>    > 取map["动作"] 和 map["爱情"] 并进行按位与操作，得出的结果就是没有这两个标签的电影
>    > 011 & 100 = 000

内存：为了存 5万个movie，需要用5 万个bit，5 万个movie 如果有10 万个tags （排除 map 需要的开销）

（50000 / 8）*  100000 ≈ 625M

计算次数：

如果一个用户平均32个 Tags与最多60 个Tags，那么只用32 ~ 60 次的 5万个bit 按位与（对于用户Tags 较为丰富得到来说算是减少计算次数的）



缺点：

不好扩展、容易埋坑

1. 以后可能不止50000 个又或许5000个 movie
2. 只适合一个场景，多一个场景就要多一个 `bitmap` （内存爆炸）





