# docker镜像

### 什么是Docker镜像？

**Docker** 镜像也是一个压缩包，只是这个压缩包不只是可执行文件，环境部署脚本，它还包含了**完整的操作系统**。因为大部分的镜像都是基于某个操作系统来构建，所以很轻松的就可以构建本地和远端一样的环境。

这就很牛皮了，如果我们的应用是在 **Centos7** 上部署，我们只需要将项目环境部署在基于 **Centos7** 的环境中，然后无论在哪里去解压这个压缩包，都可以保证环境的一致性。在整个过程中，我们根本不需要进行任何的配置，因为这个压缩包可以保证：**本地的环境和云端是一致的**，这也是 **Docker** 镜像的精髓

开发者体验到了 **Docker** 的便利，从而很快宣布 **Paas** 时代的结束，不过对于大规模应用的部署，**Docker** 能否实现在当时还是个问号

就在 2014 年的 **DockerCon** 上，紧接着发布了自研的「**Docker swarm**」，**Docker** 就这样 一度奔向高潮，即将就到达了自己梦想之巅。

> 为什么会推出Docker Swarm?

虽然 **Docker** 通过「**容器**」完成了对 **Paas** 的「**降维打击**」，但是 **Docker** 的目的是：如何让更多的开发者将项目部署在自己的项目上，从技术，商业，市场多方位的争取开发者的群体，为此形成自家的  **Paas** 平台做铺垫

**Docker** 项目虽然很受欢迎，就目前看来只是一个创建和启动容器的小工具。需要应该清楚的一点是，用户最终部署的还是他们的网站，服务甚至云计算业务。所以推出一个完整的整体对外提供集群管理功能的 **Swarm** 势在必行，这个项目中的最大亮点即直接使用了 **Docker** 原生的 **API** 来完成对集群的管理。

对于**单机**项目，只需要执行下面一条语句即可实现容器

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1IwmziaNZ7mShksr9ugRtFPa9IL5ummjJdXBH1x1tNSrHicM8bxb7mYsZ7YWBg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

对于**多机**的项目，只需要执行

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1IwmziasM8y7Wv5kqRRMbT4PiaIu1XpbL9HaQXTGMvSdfnZAKevcRtXicS48FWA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

你看，从单机切换到多机，使用的方法也就参数不同而已，所以这样一个原生的「**Docker容器集群管理**」一发布就受到大家的青睐。随着自身生态的逐渐完善，借助这一波浪潮并通过各种并购来强大自己的平层能力

要说最成功的案例，非 **Fig** 项目莫属。之所以这么屌，是因为作者提出了「**容器编排**」的概念。

> 什么是容器编排？

其实这也不是什么新鲜内容，比如在 **Linux** 中的 **Makefile**和常见的 **SHELL** 脚本，它是一种通过工具或者配置来完成一组虚拟机或者关联资源的定义、配置、创建等工具。

> 容器的编排是怎么样的呢

我们先以 Fig 为例，假设开发人员小黑，现在要部署一个项目，其中包含了应用容器 **A**，数据库容器 **B**，负载容器 **C**，这个时候 **Fig** 只需要将三个容器定义在一个配置文件，然后指定他们的关联关系，执行下面的命令即可

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1IwmziasuUDiahwwBU8VIBO47Hic4Mibw4ozMc1HXuMZn9z2dMwkp7jBxURc0ESg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

当然也可以在 **Fig** 的配置文件中配置各种容器的副本，然后加上 **Swarm** 的集群管理功能，这样不就是 **Paas** 了么。只是这个项目被收购以后，修改名字为 **compose** 了，后续也会的 **compose** 进行详细的阐述

### Docker能做什么

**Docker** 是一个用于开发，发布，运行应用的程序于一体的开放平台。如果我们需要将货物整齐的摆放在船上且互不影响，那么一种可行的方案即通过集装箱进行标准化，我们将各种货品通过集装箱打包，然后统一的放在船上进行运输，Docker其实就是这样一个将各种软件进行打包成集装箱，然后分发。



![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1IwmziaCGM6al2JuwrPq3DwKUWEVDxOEDDef3D6bPrHmFwk9ico9FbgwUXRJPQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

运行上述命令，**Docker** 首先会检查本地是否有 **hello-world** 这个镜像，如果发现本地没有这个镜像，**Docker** 就会去 **Docker Hub** 官方仓库下载此镜像，然后运行它。最后我们看到该镜像输出 "Hello from Docker!" 并退出。

### Docker核心概念

**Docker** 的操作主要围绕**镜像**，**容器**，**仓库**三大核心概念

#### 镜像	容器	仓库

#### 镜像

> ​	什么是镜像？

一句话说即镜像是 **Docker** 容器启动的先决条件，因为镜像会提供容器运行的一些基础文件和配置文件，是容器启动的基础。说白了，要启动容器，需要镜像来提供一些基础环境。

> ​	使用的镜像的方式有哪些？

- 自定义创建镜像。首先找一个基础镜像，比如此镜像是 **Centos**，然后在此镜像基础上自定义需要的内容。举个例子，基础镜像为 **Centos**，先安装 **Nginx** 服务，然后部署咱们的应用程序，最后做一些自定义的配置，这样一个镜像就完成了，此镜像的操作系统是 **Centos**，其中包含了 **Nginx** 服务
- 从仓库寻找别人已经做好的镜像。直接去 **Docker hub **或者其他公开仓库 下载即可

#### 容器

> ​	什么是容器？

容器是镜像的运行实体。镜像是静态的**只读文件**，可是容器是要**运行**的，需要可写文件层。所以容器运行着真正的应用进程，所以自然会有创建，运行，停止，暂停和删除五种状态

既然容器是直接运行的运行程序，那它是有自己的**命名空间**嘛？

容器有自己独立的命名空间和资源限制，意味着在容器内部，你无法看到主机上面的进程，环境变量等信息，这就是容器和物理机上的进程本质区别

#### 仓库

> ​	什么是仓库？

镜像仓库类似于代码仓库，用来分发和存储镜像，分为**公共镜像**和**私有镜像**。**Docker hub** 是 **Docker** 的官方公开镜像仓库，很多的官方镜像都可以在上面找到，但是访问很慢，所以可以找国内的镜像源，当然后面我们也会自己搭建一个私有镜像仓库



#### **Docker 重要的组件**

我们去 Docker 默认安装路径先看看有哪些组件

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1IwmziaxGGhPOeVGJGib224Fb7k35ibQEGhDwhJc8mueK9icX4OKdeK0Vle8x1Xg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

- runc：是一个用来运行容器的轻量级工具
- contained：是容器标准化后的产物，从 **Dockerd** 剥离出来，**contained** 通过 **contained-shim** 启动并管理**runc**，可以说 **contained** 是真正管理容器的生命周期

#### docker 各个组件之间的关系

启动一个容器

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1IwmziaW6d3n5OHaRtHRYRQODDmuEaFy2Oz4m6Ms0BibSJWFUicibq7cxnLA2BVw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

启动完成，通过下面命令查看 docker 的 pid

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1IwmziamvnUDAfbIgFXrX5ZzEe9OibbUy1ChkeSRI7PNuYMibU0giahuzlmeibic8Q/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

此时发现其 pid 为 4428，随后我们查看进程之间的关系

通过 pstree 查看进程之间的关系

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1Iwmziaek4p09QnE4ibZicqt1xwictcicqqdvKicedaY4psWI3QcxgZvYnS1s4KmGw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)



> ​	docker-proxy

docker-proxy 用来将容器启动的端口映射到主机，方便主机的访问。

假设目前启动一个 **nginx** 容器并将容器的 **80** 端口映射到主机的 **8080** 端口

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1Iwmziax6c4JT7zmBqxvEgtrDDb3lRm1fltzWkwRgfbpzPHCwONjUUkHQkvYA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

查看容器 IP

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1IwmziapJ3vBQnP9uHeHdriaRnABKMHmzBYcNjwsficicbqKIaicsaopJuazKmNMw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

此时使用 ps 查看主机是否有 docker-proxy 进程

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1Iwmziab7A4sxWE1C47NphIg7UzKnvUNsOpUHyZJEojbibZDrZj7Dv7TwgheKg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

可以发现当进行端口映射的时候，**docker** 为我们创建了一个 **docker-proxy** 进程，并且通过参数将容器的 **IP** 和端口传递给 **docker-proxy**，然后 **proxy** 通过 **iptables** 完成 **nat** 的转发

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1IwmziaedEmRl9cmeVrHlvy6fmF9KgicdiaLQraCAASuFIkjpSyeyg1IWwyic0Tw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

从最后一句可以看出，当我们主机访问 8080 端口的时候，**iptable** 将流量会转发给 `172.17.0.2` 的 80 ，从而实现主机上直接访问容器的业务

使用 **curl** 访问一下 **nginx** 容器

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1Iwmziah1lhSG318ict4Eyp4MHnvGMBc9sQBFLFK3rSibMJanrz885QTpATzCCQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

#### contained组件

containerd

contained主要负责容器的生命周期管理，同时还会负责一些其他的功能

> 主要负责那些功能？	

镜像的管理

接收dockerd的请求

管理存储相关资源

管理网络资源

> ​	containerd-shim

`containerd-shim` 的意思是垫片，类似于拧螺丝时夹在螺丝和螺母之间的垫片。`containerd-shim` 的主要作用是将 `containerd` 和真正的容器进程解耦，使用 `containerd-shim` 作为容器进程的父进程，从而实现重启 `containerd` 不影响已经启动的容器进程。

### Docker镜像使用

来，继续，我们看看镜像是什么。镜像是一个只读的镜像模版且包含了启动容器所需要的文件结构内容。镜像不包含动态数据，构建完成将不会改变

> 对于镜像都有哪些操作?	

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1IwmziaCaMtorI6Hnlyg3wf8Z9r1bTImEF53mILzgtd93o1YuFR9koL75KBuw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

对于镜像的操作分为：

- 拉取镜像：通过 **docker pull** 拉取远程仓库的镜像
- 重命名镜像：通过 **docker tag** 重命名镜像
- 查看镜像：通过 **docker image ls** 查看本地已经存在的镜像
- 删除镜像：通过 **docekr rmi** 删除没有用的镜像
- 构建镜像
- 第一种是通过 **docker build** 命令基于 **dockerfile** 构建镜像，推荐
- 第二种是通过 **docker commit** 基于运行的容器提交为镜像

#### 拉取镜像

拉取镜像直接使用 docker pull 命令即可，命令的格式如下

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1IwmziarsBsQdPhVPePjHQ8TKgR1V9XPtTauQevET7MvHiadq1tcH1SBzf1vRQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

- **registry** 为注册的服务器，docker 默认会从官网 **docker.io** 上拉取镜像，当然可以将 **registry** 注册为自己的服务器
- **repository** 为镜像仓库，**library** 为默认的镜像仓库
- **image** 为镜像的名称
- **tag** 为给镜像打的标签

现在举个例子，这里有个镜像叫做 **busybox**，这个镜像集成了上百个常用的 **Linux** 命令，可以通过这个镜像方便快捷的查找生产环境中的问题，下面我们一起操作一波

```dockerfile
docker pull busybox
```

首先会在本地镜像库查找，如果不存在本地库则直接去官网拉取镜像。拉取完镜像后随即查看镜像

```
docker images ls
```

如果要查看指定的镜像，则使用docker image ls命令进一步查询

#### 重命名镜像

采用打标签的方式重命名，格式如下

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1IwmziabF8zxz8ibeicRkwsIhnA9v0PKFazMQSauaBymbA3oIe7eTLg2sYGKe5A/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)



#### 构建镜像

如何自己构建自己镜像呢

之前说过，有两种方式，一种是通过 **docker commit** 的方式，一种是 **docker build** 的方式。首先看看使用容器提交镜像的方式

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1IwmziauLV8vsNHDxhekibFthHK6lIgrruuSDhOra4TGD4VGGdqk7nXDN8G7qg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

此时启动了一个busybox容器并进入到容器，并在容器中创建一个文件，并写入内容

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1IwmziaJTtcesJAKeAEjjrfia2NSaYUlDId9iaky1IGG8wCaIIWLRG74rkHvJjw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

此时就在当前目录下创建了一个hello.txt文件并写入了内容。现在新建另外一个窗口，然后提交为一个镜像

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1IwmziaaCHk0QnKicrrNoXzPBybfsrLuc9bMMloKE8yxvES9v7dHIIiaLG7U2EA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

然后使用 docker image ls 查看发现确实生成了镜像

#### dockerfile

我们先看一个dockerfile就知道如何用了

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1IwmziaEY5MIK41tRqQeX3eK8YTNKPPgNwicKvL5Idrh4XEicr51jyIovVyYScw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

- 首先第一行表示基于什么镜像构建

- 第二行是拷贝文件`nginx`.`repo` 到容器内的 `/etc/yum.repos.d`

- 第三行为容器中运行 `yum install` 命令，安装 `nginx` 命令到容器

- 第四行为生命容器使用 80 端口对外开放

- 第五行定义容器启动时的环境变量`HOST=mynginx`，容器启动后可以获取到环境变量 `HOST` 的值为 `mynginx`。

- 第六行定义容器的启动命令，命令格式为`json` 数组。这里设置了容器的启动命令为 `nginx` ，并且添加了 `nginx` 的启动参数 `-g 'daemon off;'` ，使得 `nginx` 以前台的方式启动

  > 基本操作已经会了，现在我们看看镜像的实现原理

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1Iwmziav8eBJ4N23xicCmxKDcZpSQAwicR0yicoQWILL3iaRpVyYmKBbaCMQCvfnA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

- 第一行：创建一个 `busybox` 镜像层
- 第二行：拷贝本机 `test` 文件到镜像内
- 第三行 在`tmp` 文件夹创建一个目录 `testdir`

为了清楚的看见镜像的存储结构，通过 `docker build` 构建镜像

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1IwmziaeTL0sgB62TVEQD4W2okDVicTVsYqu76OjpV4stHgmyhbsiayVgDNsZaw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

> ​	容器的生命周期是怎么样的?

容器的生命周期一共有五个状态分别为

- created  初建状态
- running  运行状态
- stopped 停止状态
- opaused 暂停状态
- deleted  删除状态



通过 **docker cretate** 进入容器的初建状态，然后通过 **docker start** 进入运行状态，通过 **docker stop** 进入停止状态，运行状态的容器可以通过 **docker pause** 让其变为暂停状态，为了详细的查看这些过程，我们实操一下

- 创建并启动容器

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1IwmziaLAW1CbqqgHqvOTRiaqtL6Z88fg0xqXAKcxjmkkwibIQUibtZNrgjIG9KQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

通过 **docker create** 创建的容器处于停止的状态，使用 **docker start busybox** 进入启动状态

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1IwmziacicuPEIhoKLOic1jDezx1YMFmpYYg2aXROC5CIibKaD0q3ClwdRhICbtw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

当使用 **docker run** 创建并启动容器的时候，**docker** 后台的执行逻辑为

- 首先检查本地是否有 **busybox** 镜像，不存在则取 **dockerhub** 中拉取
- 使用 **busybox** 镜像启动一个容器
- 分配文件系统，并在镜像的只读层外创建一个读写层
- 从 **docker ip** 池分配个 **ip** 给容器
- 运行镜像



> ​	进入交互模式

同时使用`-it` 参数可以让我们进入交互模式，容器内部和主机是完全隔离的。另外由于此时的 `sh` 为 1 号进程，所以如果通过 `exit` 退出 `sh`，那么容器也就退出，所以对于容器而言，杀死容器中的主进程，那么容器也就会被杀死

通过 **docker stop** 停止容器，其原理是给运行中的容器给 `sigterm` 信号，如果容器为 1 号进程接受并处理`sigterm`，则等待 1 号进程处理完毕后就退出，如果等待一段时间后还是没有处理，则会通过发送 **sigkill** 命令强制终止容器

#### 进入容器

> ​	如何进入容器？

想要进入容器，有三种方案，分别是 `docker attach`，`docker exec`，`nsenter` 等

使用 docker attach 方式进入容器

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1IwmziabxQnxG5ReIN4f2hianwrynmdRTBOXg3hPLoRicggUdjDJ0icX9GneOficQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

通过 `docker ps -a` 查看当前的进程信息

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1IwmzianLCCuUFGjepNicymLakMibekwssUn3ibtMstohJ8cAvKK4G2FproKoCCw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)



使用 `docker exec`进入容器

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1IwmziasyDFAWlIr1W4VCcko1LCmCNfdRliatp2CARazsuYyloeBSNFn9U06lw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

奇怪的发现居然是两个`sh` 进程，主要是因为，当我们使用 `docker exec`方式进入容器的时候，会单独启动一个 `sh` 进程，此时的多个窗口都是独立且不受干扰，也是非常常用的方式



#### 删除容器

> ​	使用docker rm的方式删除容器

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1IwmziaFW6nViclLmJcHLEu7xdVKf5WZay5AicJ5RpgibspBoz1hneGxR9Qk3Bzg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)



如果此时，容器正在运行，那么需要添加`-f`的方式停止正在运行的容器

> ​	如果想要导出容器怎么操作呢

这简单，不过在导出之前先进入容器创建一个文件

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1IwmziaibEE6T1SLnh1BaSKuGxmpibd2ib66OsWDogADFaVZL5IWicWiaVT91OSibIA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

然后导出为文件

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1IwmziabrCBeS60eGchxGbvL4RIbwcS5XbQsbb8fyPFhqwkPWA3LYWeMicXgzg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

此时会在当前目录生成一个 `busybox.tar` 文件，此时就可以将其拷贝到其他的机器上使用

> ​	导入容器

通过 `docker import`的方式导入，然后使用`docker run`启动就完成了容器的迁移

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1IwmziaOYwjLjw9IY66RmH9LicYBvt0n6hnGbPw9n7aQGTic5HMvXdpyNSoK2qg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

此时容器名称为 `busybox：test`，然后我们使用 `docker run` 启动并进入容器

此时发现之前在`/tmp`创建的目录也被迁移了过来



### 仓库

容器的基本操作应该都会了，那么我们应该如何去存储和分发这些镜像，这就需要介绍下仓库；

我们可以使用**共有**镜像仓库分发，也可以搭建**私有**的仓库



此时假设我的账户是 xiaolantest，创建一个 busybox 的仓库，随后将镜像推送到仓库中。

第一步：拉取 busybox 镜像

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1Iwmzia2Zw3wvvhvFhg54UYpLy2j1wMibHpHx2XLTTzYzg4m8DR6C9ibQ29vGOw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

第二步：推送镜像之前先登录镜像服务器(注意用户名密码哦)，出现 login Succeeded表示登录成功

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1IwmziaDwbU1Yy1ZtKBZ5XwwbcPW3AgMVQL68Wc2YdxPavc6JZF9sQKeNAlwg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

第三步：推送之前还要做一件事，重新对镜像命名，这样测能正确的推动到自己创建的仓库中

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1Iwmzia40e5DTESffQ3iabPLzaNO8ScJ7gr6QL6SS8SyRyYCX4rDrncLACGIpA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

第四步：docker push 到仓库中

![图片](https://mmbiz.qpic.cn/mmbiz_png/WUrZckMPh56fB9QRPvF9vUFy1w1IwmziaIRYibvYL4Yo2tia4A3jLAjYibS0ReIwWf15EVDM6sRzCFdyU6alRzYiaTw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

> 





















