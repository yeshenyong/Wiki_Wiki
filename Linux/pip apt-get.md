# pip apt-get

### pip

pip 是 Python 包管理工具，该工具提供了对Python 包的查找、下载、安装、卸载的功能。

默认位置：<python安装目录>\lib\site-packages

### apt-get

Linux 包管理工具

Advanced Package Tool，又名apt-get，是一款适用于[Unix](https://baike.baidu.com/item/Unix)和[Linux](https://baike.baidu.com/item/Linux)系统的[应用程序管理器](https://baike.baidu.com/item/应用程序管理器/16063616)

#### apt-get update

#### apt-get install

```sh
apt-get update						// 更新源文件，并不会做任何安装升级操作

apt-get upgrade						// 升级所有已安装的包

apt-get install packagename				// 安装指定的包

apt-get install packagename --only-upgrade		// 仅升级指定的包

apt-get install packagename --reinstall   		// 重新安装包

apt-get -f install   					// 修复安装

apt-get build-dep packagename				// 安装相关的编译环境

apt-get source packagename  				// 下载该包的源代码

apt-get dist-upgrade 					// 升级系统

apt-get dselect-upgrade 				// 使用 dselect 升级

```



#### apt-get remove

删除

```sh
apt-get remove packagename				// 删除包  　　

apt-get remove packagename -- purge 			// 删除包，包括删除配置文件等 

apt-get autoremove packagename --purge 			// 删除包及其依赖的软件包+配置文件等（只对6.10有效，推荐使用）
```





#### apt-get clean

#### apt-get autoclean

#### apt-get check

```sh
apt-get clean 						// 清理无用的包 

apt-get autoclean 					// 清理无用的包 

apt-get check 						// 检查是否有损坏的依赖
```





#### 危机场景

### 场景

看到很多 `Dockerfile` 都是把 `apt-get update` 和 `apt-get install` 写在同一个 `RUN` 指令中的

```dockerfile
RUN apt-get update && apt-get install -y \
        package-bar \
        package-baz \
        package-foo
```

而不是

```dockerfile
RUN apt-get update
RUN apt-get install -y \
        package-bar \
        package-baz \
        package-foo
```

只知道这样写在同一个 `RUN` 中会减少 `layer` 层，缩减构建镜像的大小.

但看到有一些文章提到，只有写在同一个 `RUN` 中才会对后面的 `apt-get install` 生效，却没有细说原因.

因此产生了一个疑惑，分开写也应该会对后面的 `apt-get install` 生效才对啊，因为镜像的构建是一层一层的，后面的层会基于前面的层.
也就是说，`RUN apt-get update` 会单独构建一层，并且会对后面 `RUN apt-get install` 的层产生作用才对.

### 分析

假设有一个这样的 `Dockerfile`

```dockerfile
FROM ubuntu:14.04
RUN apt-get update
RUN apt-get install -y curl
```

构建镜像之后，所有的层都会在 Docker 的缓存中.
假设后来修改 `apt-get install` 添加额外的包

```dockerfile
 FROM ubuntu:14.04
 RUN apt-get update
 RUN apt-get install -y curl nginx
```

Docker 将初始和修改的指令视为相同，并会重用之前构建的缓存.
因此，不会执行 `apt-get update`，因为构建直接使用之前的缓存版本.
由于 `apt-get update` 没有运行，所以构建安装的 `curl` 和 `nginx` 包很可能是过时的版本.

所以，把 `apt-get update` 和 `apt-get install` 写在同一个 `RUN` 中以获取最新版本的包，而且还减少了 `layer` 层.