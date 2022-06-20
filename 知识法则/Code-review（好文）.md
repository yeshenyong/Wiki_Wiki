# Code Review 漫谈

摘自：https://github.com/lao5-team/code_review

> 已给原作者点小心心

## 【通用】一次 review 的工作量

过多改动是阻碍有效、快速 review 的最大敌人。

我遇到的“大型 code review”很可能有两种我们都不希望看到的结果：

- 拖很久完成 review，但是 reviewer 因为受不了持久战做出妥协。
- 拖很久不了了之，没有 review 完而直接提交。

**最佳实践：**

- **一次 review 的改动最好在 200 行以内，最多不超过 1000 行。**
- 一次 review 包含尽可能少的 commit。
  - **如果功能/需求复杂，可以按照合理的粒度拆分成多个 commit。**
- **一次 review 不要包含不相关的 commit。**

## 【通用】部分代码完成度低、要求低

> **案例 1**
>
> “这是测试，这么写省事儿，没必要关注安全问题吧。”
>
> **案例 2**
>
> “这是测试，这段代码不需要写注释了吧。”
>
> **案例 3**
>
> “这是例子，不需要考虑性能和安全性了吧。”

我经常遇到一种情况是：降低对 example、demo、test、benchmark 的要求。

这会造成以下不良影响：

- 代码会在开发者中传播、传承，糟糕的代码会增加各个环节的成本。
- example 和 demo 中的代码片段会在使用者中传播，错误和劣化的代码也会像病毒一样传播。

**最佳实践：**

- **请用一贯的高标准、严要求完成所有代码。**

## 【通用】commit history

本小节以 git 为例。

> **案例 1**
>
> 某同事的所有 commit message 都是“update”。
>
> reviewer 不关注 commit message。
>
> 该项目 commit history 中存在大量连续的“update”。
>
> **案例 2**
>
> 某同事发起 code review，针对 reviewer 的问题，每改动一次就创建一个带有相同 message 的 commit。
>
> 该项目 commit history 中存在大量连续的有相同 message 的 commit。
>
> **案例 3**
>
> 某项目的开发模式是“创建分支 - 在分支开发 - 合并到 master”。
>
> 该项目成员从不 rebase master，开发完成后用 “merge” 的方式合并到 master。
>
> 该项目 commit history 中存在大量分叉和“merge commit”。

**最佳实践：**

- **请培养意识：commit history 和代码同等重要，commit history 也是给人看的，commit history 有时候还需要回溯。**
- **请写好 commit message。**
- 请维护好 commit tree。
  - **尽量保持 commit tree 是线性的。**
  - **优先用“rebase and merge”或“squash and merge”的方式合并分支。**

## 【通用】有意义的 diff

无意义的 diff 是阻碍有效、快速 review 的另一大敌人。

什么是无意义的 diff 呢？

> **案例 1**
>
> 某项目代码格式不统一，某天项目负责人决定统一格式风格，要求项目成员配置代码格式化工具。
>
> 某同事修改了几十行代码，保存时代码格式化工具对代码格式化，产生上百行 diff。
>
> **案例 2**
>
> 某项目将 `.proto` 文件和生成的 `.pb.cc` 和 `.pb.h` 文件提交到代码库。
>
> 某同事给该 `.proto` 文件添加一个字段，产生几十行 diff。
>
> **案例 3**
>
> 某项目将二进制测试数据提交到代码库。
>
> 某同事更新二进制测试数据。
>
> **案例 4**
>
> 某同事重构了某模块，重构前后的代码差异巨大。

前两个案例中，有意义 diff 藏于大量无意义 diff 中。

第三个案例中，二进制文件不方便 diff。

第四个案例中，改动有意义，但 diff 无意义。

无意义的 diff 就是不 diffable（suitable for processing by a diff program in order to show differences）的 diff。

**最佳实践：**

- 工作中，会因为种种原因产生不 diffable 的改动。
  - **不合理的，请让它们变得合理。**
  - **合理的，特事特办。**

针对上面案例：

- 案例 1：从工具层面保持项目统一的格式风格。如果要修改格式风格，统一修改并直接提交。
- 案例 2：不提交生成的 protobuf 代码。
- 案例 3：提交生成“二进制测试数据”的代码，而不是“二进制测试数据”本身。
- 案例 4：不关注 diff，review 重构后的代码。

## 【通用】如何挖“坑”

“对那些临时的, 短期的解决方案, 或已经够好但仍不完美的代码使用 TODO 注释. ”

每个 TODO 注释都是一个“坑”。我们允许“坑”的存在，但不允许“暗坑”的存在。

要注意的是，TODO 注释的格式是：

```
// TODO(who): to do what.
// TODO(who): when to do what.
```

TODO 注释一定要带上挖“坑”人，还可以带上填“坑”时间，让 TODO 不会变成 NEVERDO。

## 【通用】专有名词

任何项目所在领域和项目所采用的技术领域中，存在大量专有名词。

Doc work（文档、issue、代码注释等）中应该特别注意这些专有名词。

**最佳实践：**

- **专有名词有官方写法的，请在 doc work 中使用官方写法。**
- **专有名词没有统一写法的，请在 doc work 中使用更流行的写法。**
- **以上只针对 doc work。**

一些专有名词的官方写法：

```
CentOS
deepx
GitHub
GitLab
HDFS
iOS
LZ4
macOS
Redis
Snappy
TensorFlow
Visual Studio Code
Xcode
```

以上专有名词的不严谨写法：

```
centos
DeepX
github
Github
gitlab
Gitlab
hdfs
Hdfs
ios
lz4
Lz4
macos
MacOS
redis
snappy
tensorflow
Tensorflow
visual studio code
xcode
XCode
```

## 【一致性】概述

好的**一致性**和**可读性**是优秀代码最重要的两个要素，是 code review 应该重点关注的。

我没有资格给一致性下定义。

我将列举一系列缺乏一致性的案例，欢迎对号入座。

> **案例 1**
>
> 写中文文档时，一会用全角标点，一会用半角标点。
>
> 文件换行符一会用 `\n`，一会用 `\r\n`。
>
> 非 ASCII 文件一会用 UTF-8 编码，一会用 GBK 编码。
>
> 一会用 tab 缩进，一会用 2 个空格缩进，一会用 4 个空格缩进。
>
> 代码格式化，但是文档中的代码没有格式化。
>
> **案例 2：命名不一致**
>
> 表达数量时，一会用 `size`，一会用 `count`，一会用 `number`。
>
> 表达长度时，一会用 `size`，一会用 `length`。
>
> 表达计算时，一会用 `compute`，一会用 `calculate`。
>
> 一会用 `center`，一会用 `centre`。
>
> **案例 3：命名不对称**
>
> ```
> void set_attribute(const std::string& attribute) {
>   // 应该 set `attribute_`，实际 set `biz_attribute_`。
>   biz_attribute_ = attribute;
> }
> // begin vs end
> // start vs stop
> auto start = std::chrono::steady_clock::now();
> // ...
> auto end = std::chrono::steady_clock::now();
> auto duration = end - start;
> # 没错，你不是一个人，Python 标准库也不一致。
> # begin vs end
> # start vs stop
> str.startswith
> str.endswith
> // in vs out
> // input vs output
> void Foo(const std::string& in, std::string* output);
> struct BenchmarkStat {
>   // succeed vs fail
>   // success vs failure
>   double success = 0;
>   double fail = 0;
>   // ...
> };
> ```
>
> **未完待续**

## 【可读性】缩写

**最佳实践：**

- **不使用奇怪的缩写。**
- **使用约定俗成的缩写。**
- **尽量不缩写短单词。**
- **当前上下文中不会引起歧义的缩写，是好缩写。**
- **考虑在某范围内创造缩写，但请三思如何缩写，因为它们将影响深远。**

坏的缩写：

```
thread -> thd
client -> clt
label -> lab
```

不太好的缩写：

```
count -> cnt
manager -> mgr
```

好的缩写：

```
advertisement -> ad
document -> doc
implement -> impl
initialize -> init
personalization -> p13n
reference -> ref
```

下面代码片段的 `var` 和 `stddev` 不会有人误解：

```
double mean = 0.0;
double var = 0.0;
// compute mean & var
double stddev = sqrt(var);
```

下面代码片段的 `K` 和 `V` 不会有人误解：

```
template <typename K, typename V>
class Map;
```

## 【可读性】日志案例 1

关于日志，我遇到的一个典型问题是：失败后的错误日志揣测失败原因。

v1 打开文件失败时，打印错误日志“文件不存在”：

```
// v1
const char* file = ...;
int fd = open(file, ...);
if (fd == -1) {
  CARBON_ERROR("%s does not exist.", file);
}
```

事实上，打开文件失败的原因有很多，常见的有：

1. 文件不存在。
2. `file` 是一个目录。
3. 没有读权限（读模式）。
4. 没有写权限（写模式）。
5. 打开了过多文件，即打开文件数超过 `ulimit -n`。

v2 是更好的做法：

```
// v2
const char* file = ...;
int fd = open(file, ...);
if (fd == -1) {
  int e = errno;
  CARBON_ERROR("Failed to open(\"%s\"), errno=%d(%s).", file, e, strerror(e));
}
```

**最佳实践：**

- **失败后的错误日志应该打印失败事件本身，不揣测失败原因。**
- **如果有更多上下文，请打印出来。**

## 【可读性】函数设计案例 1

某基础库要添加一个“开启/关闭 metrics 采集”的功能，考虑函数设计。

v1 和 v2，`Set` 和 `Enable` 都是动词，“动名动”、“动动名”的词性不适合做函数名。 `enable` 是动词，不适合做函数参数名。

```
// v1
void SetMetricsEnable(bool enable);
// v2
void SetEnableMetrics(bool enable);
```

v1 改进，不太好。 `SetMetricsEnabled` 更像一个 setter 函数。

```
// v1 改进
void SetMetricsEnabled(bool enabled);
```

v3，`enbaled` 为 `false` 时，是开启还是关闭？

```
// v3
void EnableMetrics(bool enabled);
```

v4 和 v5 都是好的函数声明。

```
// v4
void EnableMetrics();
void DisableMetrics();
// v5
void ToggleMetrics(bool enabled);
```

## 【扩展性 & 可读性】函数设计案例 2

`Foo` 处理 `in`，返回处理后的结果。 如果 `compressed` 为 `true`，`in` 是用 Snappy 压缩的。

```
std::string Foo(const std::string& in, bool compressed = false);
```

有一天我们发现 LZ4 比 Snappy 压缩比更高、性能更好，然而 `Foo` 已经无法扩展了。

`Bar` 比 `Foo` 有更好的扩展性，它不但支持好了现状，而且可以扩展到未来。

```
enum COMPRESSION_TYPE {
  COMPRESSION_TYPE_NONE = 0,
  COMPRESSION_TYPE_SNAPPY = 1,
  COMPRESSION_TYPE_LZ4 = 2,
};
std::string Bar(const std::string& in,
                int compression_type = COMPRESSION_TYPE_NONE);
```

我们再看看它们的调用代码，可读性一目了然：

```
out = Foo(in, false);
out = Foo(in, true);
out = Bar(in, COMPRESSION_TYPE_NONE);
out = Bar(in, COMPRESSION_TYPE_SNAPPY);
out = Bar(in, COMPRESSION_TYPE_LZ4);
```

**最佳实践：**

- **设计函数时，考虑用 `int/enum` 取代 `bool` 做参数，它们有更好的扩展性和可读性。**

## 【扩展性 & 可读性】函数设计案例 3

`Init` 根据若干参数初始化某模块。

```
// v1
bool Init(const std::string& model_dir,
          int session_pool_max_size = 96,
          int intra_op_parallelism_threads = 1,
          int inter_op_parallelism_threads = -1,
          bool enable_warmup = true,
          bool enable_session_run_timeout = true);
```

该模块迭代到第二版，引入 CUDA 和 OpenCL 能力，添加对应参数 `enable_cuda` 和 `enable_opencl`。

如此迭代下去，该模块还将引入多少参数呢？

```
// v2
bool Init(const std::string& model_dir,
          int session_pool_max_size = 96,
          int intra_op_parallelism_threads = 1,
          int inter_op_parallelism_threads = -1,
          bool enable_warmup = true,
          bool enable_session_run_timeout = true,
          bool enable_cuda = false,
          bool enable_opencl = false);
```

第二版可以这样改进：通过 string map 传递所有参数，彻底解决参数扩展性问题。

```
// v2 改进
template <typename T>
bool GetOption(const std::unordered_map<std::string, std::string>& options,
               const std::string& name, T* option);
template <typename T>
bool GetOption(const std::unordered_map<std::string, std::string>& options,
               const std::string& name, T* option, const T& default_option);

bool Init(const std::unordered_map<std::string, std::string>& options) {
  std::string model_dir;
  int session_pool_max_size;
  int intra_op_parallelism_threads;
  int inter_op_parallelism_threads;
  bool enable_warmup;
  bool enable_session_run_timeout;
  bool enable_cuda;
  bool enable_opencl;
  if (!GetOption(options, "model_dir", &model_dir) ||
      !GetOption(options, "session_pool_max_size", &session_pool_max_size,
                 96) ||
      !GetOption(options, "intra_op_parallelism_threads",
                 &intra_op_parallelism_threads, 1) ||
      !GetOption(options, "inter_op_parallelism_threads",
                 &inter_op_parallelism_threads, -1) ||
      !GetOption(options, "enable_warmup", &enable_warmup, true) ||
      !GetOption(options, "enable_session_run_timeout",
                 &enable_session_run_timeout, true) ||
      !GetOption(options, "enable_cuda", &enable_cuda, false) ||
      !GetOption(options, "enable_opencl", &enable_opencl, false)) {
    return false;
  }

  // ...
  return true;
}
```

我们再看看 `Init` 的调用代码，可读性一目了然：

```
// v1
bool ok = Init("mandatory_model_dir", 96, 1, -1, true, true);
// v2
bool ok = Init("mandatory_model_dir", 96, 1, -1, true, true, false, true);
// v2 改进
std::unordered_map<std::string, std::string> options;
options["model_dir"] = "mandatory_model_dir";
options["session_pool_max_size"] = "96";
options["intra_op_parallelism_threads"] = "1";
options["inter_op_parallelism_threads"] = "-1";
options["enable_warmup"] = "true";
options["enable_session_run_timeout"] = "true";
options["enable_cuda"] = "false";
options["enable_opencl"] = "false";
bool ok = Init(options);
```

**最佳实践：**

- **设计函数时，尽量避免过多参数，考虑用特殊方式处理大量参数的传递。**

## 【扩展性 & 可读性】分支优化案例 1

考虑需求“用 2 个开关控制 4 盏灯”：

- 开关 1 关闭，开关 2 关闭，则打开 1 号灯。
- 开关 1 开启，开关 2 关闭，则打开 2 号灯。
- 开关 1 关闭，开关 2 开启，则打开 3 号灯。
- 开关 1 开启，开关 2 开启，则打开 4 号灯。

v1 可读性稍差，但逻辑清晰，可以接受。

如果需求变成“用 3 个开关控制 8 盏灯”或者更多，还套用 v1 的代码结构，可读性将非常差，也容易出错。

```
// v1
if (switch2.close) {
  if (switch1.close) {
    turnOn(light1);
  } else {
    turnOn(light2);
  }
} else {
  if (switch1.close) {
    turnOn(light3);
  } else {
    turnOn(light4);
  }
}
```

用”查表“的思想优化 v1，得到 v2。

```
// v2
Light[] lights = new Ligth[]{light1, light2, light3, light4};
int index = (switch2.close ? 0 : 2) + (switch1.close ? 0 : 1);
turnOn(lights[index]);
```

v2 可以扩展到“用 3 个开关控制 8 盏灯”的需求。

```
// v2 扩展
Light[] lights = new Ligth[]{light1, light2, light3, light4, light5, light6, light7, light8};
int index = (switch3.close ? 0 : 4) + (switch2.close ? 0 : 2) + (switch1.close ? 0 : 1);
turnOn(lights[index]);
```