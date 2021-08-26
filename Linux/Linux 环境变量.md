# Linux 环境变量

#### 查看当前环境变量

```shell
echo $PATH
```

LD_LIBRARY_PATH

```shell
echo $LD_LIBRARY_PATH
```



#### 修改环境变量

##### 方法一

但是退出当前终端后就失效

```shell
export PATH=PATH:/XXX
export LD_PRELOAD=/root/graph-learn-builder/third_party/tcmalloc/libtcmalloc.so.2.7
```

##### 方法二

修改 ~/.bashrc 或 ~/.bash_profile 或系统级别的/etc/profile

1. 在其中添加例如export PATH=/opt/ActivePython/bin:$PATH
2. source .bashrc （source命令也成为“点命令”，也就是一个点符号(.)）

source命令通常用于重新执行刚修改的初始化文件，使之立即生效，而不必注销并重新登陆



#### LD_LIBRARY_PATH：动态库的查找路径

##### 动态库查找路径设置

法1

同理，但是退出当前中断后就失效

```shell
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/online/loyoen/cpp_/sdk/lib/
```

法2

修改 `~/.bashrc` 或 `~/.bash_profile` 或系统级别的 `/etc/profile`

在其中同修改环境变量同理，添加例如

export LD_LIBRARY_PATH=/opt/ActiveP/lib:$LD_LIBRARY_PATH

```
source .bashrc 
```

> ​	source命令通常用于重新执行刚修改的初始化文件，使之立即生效，而不必注销并重新登陆

法3（貌似ldconfig需要慎用，还是建议第二种方法吧）

这个没有修改LD_LIBRARY_PATH但是效果是一样的，实现动态库的查找

```shell
sudo vim /etc/ld.so.conf
```

在下面添加需要增加的动态库路径

```shell
/usr/local/mysql/lib
```

保存后执行

```shell
sudo ldconfig
```

生效



> ​	（ldconfig 命令的用途,主要是在默认搜寻目录(/lib和/usr/lib)以及动态库配置文件/etc/ld.so.conf内所列的目录下,搜索出可共享的动态链接库(格式如前介绍,lib*.so*),进而创建出动态装入程序(ld.so)所需的连接和缓存文件.缓存文件默认为/etc/ld.so.cache,此文件保存已排好序的动态链接库名字列表.）













