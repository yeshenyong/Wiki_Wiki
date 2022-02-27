# Linux 权限

转载自：https://mp.weixin.qq.com/s/sUxX7sBoGsXXFMuqWGcUcg

### 使用者和群组

在 Linux 中，关于文件所有者分为三类，即**文件所有者、群组和其他人所属**，这里分别解释以下这三个概念

- 文件所有者

Linux 是一个多用户多任务系统，多用户就意味着有些用户创建的文件是否对其他用户可见，这是一种可见性问题，同时也是一种隐私性问题，为了考虑到每个人的隐私权，Linux 设计了文件所有者的角色。如果你有一些资料和文件的隐私性比较高，你就可以把文件设置成 "只有我自己可见" ，这就是文件所有者的作用。

- 群组

群组的这个概念用在团队开发中，用处比较多的就是为项目设置权限，比如你就职于一个银行的外包部门，你和其他外包部门共同为某个银行服务，所有的外包团体都使用一台服务器，这就会涉及到群组权限的问题，你们外包部门开发的项目不想让其他外包部门所看到，就会把该项目设置成群组可见。但是银行是总负责人，所有银行具有查看你们所有外包部门项目的权限，因此，你还需要设置银行的权限。

- 其他人所属

其他人和群组是相对的，其他人在群组之外，没有权限查看群组内文件的一种权限关系。

除了上面三个概念之外，还有一个权限级别最高的大佬，它就是 `root`，这个 root 权限是最高的。

### **Linux 文件权限**

在聊完上面使用者和群组的概念之后，接下来我们就来谈一下文件权限要如何设置的问题，这块内容是很重要的，因为这部分内容是很好解决 permission denied 问题的关键。

### 权限属性

首先登录 Linux 系统，使用 `su -` 可以切换成为 `root` 身份，然后执行 `ls -al` 会看到下面这些

![图片](https://mmbiz.qpic.cn/mmbiz_jpg/A3ibcic1Xe0iaTF3zEqPfX4f0aTXsNrwyBFGtSJwLdEc7W0EtwXaDeCO7W1JE1Hz4PgvV1R2L2D5QiaicXicZia7uZaKA/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1&wx_co=1)

一共有七列内容，这七列内容如下图所示。

![图片](https://mmbiz.qpic.cn/mmbiz_jpg/A3ibcic1Xe0iaTF3zEqPfX4f0aTXsNrwyBFM3L3xibBgP7pce4tarIZP8keAK8fiaTgcLdQ30CmHs1364Q0uEoyxqwQ/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1&wx_co=1)

> 学习的时候可以直接使用 root ，因为后续的 chgrp，chown 等指令都需要 root 来处理，但是工作中强烈建议不要使用 root 权限。
>
> 使用 exit 可以退出 root 身份。

上面这段指令中，ls 是 `list` 的意思，也就是列出，而选项 `-al` 则表示文件详细权限和属性。

- 权限，第一列表示的是权限，权限一共通过 10 个字符来表示，我们拿 home 权限为例，来列举各个字符表示的含义

![图片](https://mmbiz.qpic.cn/mmbiz_jpg/A3ibcic1Xe0iaTF3zEqPfX4f0aTXsNrwyBFniav1jYicIh5oqdZ3dI6hgIEjpwFses8y7caCkN1zTBibFcBfFCwGcKwQ/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1&wx_co=1)

第一个字符表示的是文件类型，文件类型有很多种，一般 *[d]* 表示的是目录，能用`cd`命令进入到这个目录中。可以看到图中几乎所有都是目录。

![图片](https://mmbiz.qpic.cn/mmbiz_jpg/A3ibcic1Xe0iaTF3zEqPfX4f0aTXsNrwyBFr3FqQ2uiak1moP1ueicG1ZEomMx5IY1LTzrtEobLRRkr5gUO1AKYRn5g/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1&wx_co=1)

如果是 *[-]* 则表示文件，如果是 *[l]* 则表示链接文件，如果是 *[b]* 则表示设备文件中的可随机存取设备，如果是 *[c]* 则表示为设备文件中的一次性读取设备（键盘、鼠标）。

接下来的九个字符分为三组，三个一组，分别表示所属人、所属群组、其他所有者权限，每组内的权限都是三个 `rwx` 的组合， *[r]* 表示可读， *[w]* 表示可写， *[x]* 表示可执行，这里需要注意的是，如果没有权限，就会变为 *-号。*

- 链接，这一列表示有多少文件名链接到这个节点（i-node）上，每个文件都会将它的权限和属性记录到文件系统的 `i-node` 上，不过，我们使用的目录树却是使用文件名来记录的，因此每个文件名都会关联到一个 i-node ，所以这个属性就是记录**有多少文件链接到了同一个 i-node** 上。

> 什么是 i-node ？
>
> i-node 的描述方式很像是我们之前聊过的 Socket，Socket 就是一个四元组，有时会加上协议类型变为五元组，如果你不太清楚我说的是什么，可以看下我的这篇文章 [原来这才是 Socket！](https://mp.weixin.qq.com/s?__biz=MzI0ODk2NDIyMQ==&mid=2247494554&idx=1&sn=6cfa7a5ac3bd443e7734b0a688b53294&chksm=e99a1e88deed979ed721e9885dcb4a0ac86bd4fd14eb76056832050762eeb9b8c7423870f2fc&token=1645879209&lang=zh_CN&scene=21#wechat_redirect)
>
> 我们知道，磁盘的最小存储单位是扇区，操作系统在读取扇区时，不会一个扇区接着一个扇区这样读取，因为效率太低，而是以**块**为单位进行读取，块是由多个扇区组成的。
>
> 文件中的数据都存储在扇区中，但是我们并不知道哪一块数据是我们需要的，为了存储一些文件的元信息，比如文件的创建者，创建日期，文件的大小，开发人员提出了 i-node ，也就是索引节点。一般来说，i-node 具有如下内容
>
> ![图片](https://mmbiz.qpic.cn/mmbiz_jpg/A3ibcic1Xe0iaTF3zEqPfX4f0aTXsNrwyBFXtWGubDxvg0HCTpTp09WJvicFe4Bxcb2icFicaWptpOCzmosEFjJmoDrA/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1&wx_co=1)
>
> 具体关于 i-node 的内容，我们后面还会再说。

- 然后第三列表示这个文件的所属人，由图可见，大部分文件的所属人都是 root 用户。
- 第四列表示这个文件的所属群组，在 Linux 系统下，你登录的账号会添加到一个或者多个所有人群组中，这一栏就表示对应的群组权限。
- 第五列表示文件大小，默认单位为字节（Bytes）。
- 第六列为创建这个文件的日期和最近修改日期，从图中可以看到，这个日期格式有可能不是我们想要的，如果要显示完整的日期格式，可以使用 *ls -l --full-time*，包括年、月、日、时间。

#### 修改系统默认语言

如果想要让系统默认的语系变为英文的话，那么你可以修改系统配置文件 `/etc/locale.conf`，首先我们可以查看一下系统都支持哪些语言。

![图片](https://mmbiz.qpic.cn/mmbiz_jpg/A3ibcic1Xe0iaTF3zEqPfX4f0aTXsNrwyBF5GHMcegFicatvWOFJiaKhS836AONo4wBaMz3ADMhU26wYawClricaHWkA/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1&wx_co=1)

修改默认语言，输入

```sh
 vi /etc/profile
```

在文档的最后输入

```sh
 export LANG="en_US.UTF-8"
```

就可以切换成为英文，如果想使用中文，可以输入

```sh
 export LANG="zh_CN.GB18030"
```

然后使用 `esc + :wq` 保存，保存之后使用

```sh
 source /etc/profile
```

即可完成设置。

- 第七列为文件名，有一类特殊的文件名，它表示着隐藏文件，如果文件名之前多一个 `.` ，那就表示隐藏文件。

### 权限的重要性

- 提供系统保护：非权限用户不能操作具有某些权限的功能和数据。
- 适合团队开发和数据共享：团队所有组成员和个人所属能够共享项目。

如果没有恰当的设置系统权限，可能会造成某些泄密事件或者其他不可忽视的后果，所以权限问题大家要引起重视，下面我们就来聊一聊如何设置系统权限。



### 改变系统权限和属性

我们现在知道文件权限对于一个系统安全的重要性了，现在就要聊一聊如何修改文件权限了。常用的修改文件权限的指令有

- chgrp ：改变文件所属群组
- chown：改变文件所有者
- chmod：改变文件权限

**chgrp**

chgrp 就是 change group 的缩写，我觉得李纳斯把缩写用到了极致，这也许是我们现在对于缩写这么流行的原因。chgrp 能够改变文件群组，不过，要改变群组的话，要被改变的群组名称要在 `/etc/group` 文件内存在才行，否则就会显示错误。

**chown**

既然 chgrp 能够改变文件群组，那么 chown 能够改变文件所有者，同样也需要注意的是，文件所有者必须是系统中存在的账号，也就是在 `/etc/passwd` 这个文件中有记录的使用者名称才可改变。除此之外，chown 还可以直接修改群组名称。

**chmod**

变更文件权限使用的是 chmod 这个指令，但是，权限的设置有两种方式，可以分别使用**数字**或者**符号**进行权限变更。

- 使用数字改变文件权限

Linux 文件基本权限有 9 种，分别是 owner/group/others 三种身份加自己的 read/write/execute 权限，这九个权限三个为一组，我们可以使用数字表示各个权限。

一般 r 表示 4；w 表示 2；x 表示 1，每种身份各自的权限是需要累加的，比如 rwx 就表示 4 + 2 + 1 = 7。比如我们最常见的 *chmod 777* 它就表示赋予所有的权限，也就是说谁都能看/写/执行，所以这种文件也存在极大的安全问题。使用数字改变文件权限是我们最常用的一种方式。

- 使用符号改变文件权限

九种文件权限分别对应着：(1) user (2) group (3) others，所以我们可以借由 u,g,o 来代表三种身份的权限。除此之外，a 代表 all 即全部的身份。

比如我们想要给 *-rwxr-xr-x* 设置权限，那么我们所使用的命令应该是

```sh
 chmod u=rwx,go=rx .filename
```

如果我们想要给所有人增加写入权限，就可以这么操作

```sh
 chmod a+w .filename
```

如果我们想给所有人去掉写入权限，就可以这么写入指令

```sh
 chmod a-w .filename
```

我们上面列出了三种指令，分别是 =、+、- 号，= 号表示赋值指定权限，+ 号表示增加权限，- 号表示去掉某些权限，在 + 和 - 的状态下，只要没找到指令的项目，那么该权限不会发生变动。



### **Linux 目录和文件权限**

我们上面聊的都是文件权限，文件是容纳数据的地方，这些文件包括一般文本文件、数据库文件、二进制文件等，权限对于文件的意义在于

- r（read）：可以读取文件的实际内容，比如读取文本文件的文字内容
- w（write）：可以新增、编辑或者修改文件中的内容（不包括删除文件）
- x（execute）：使文件具有被文件系统执行的权限。

Windows 下面判断文件是否能够执行的因素是看文件扩展名， 比如 .exe, .bat, .com 等等，但是在 Linux 中，判断文件是否具有可执行权限是直接判断文件有没有 x 这个权限，和文件名无关。

但是在 Linux 中，不只有文件具有权限，目录也有权限，文件是存放实际数据的地方，而目录是**记录文件所在位置的清单**，我们只有通过目录才能找到文件放在哪里！权限对于不同的目录，也代表着不同的概念。

- r （read contents in directory）：表示具有读取目录结构清单的权限，所以如果你具有读取一个目录的权限时，就代表你可以查询目录下的文件，所以你就可以使用 ls 将目录的内容显示出来。

- w（modify contents of directory）：写入权限表示你具有对文件目录和目录中的文件进行修改的操作，主要包括

- - 删除已经存在的文件和目录。
  - 创建新的文件和目录。
  - 将已存在的文件或目录进行改名。
  - 移动目录内文件、目录位置。

- x（access directory）：这执行权限有啥用？总不能目录也能够被执行吧？其实并不是这样，执行权限表示着你有没有权限进入到指定目录下，也就是 `cd(change directory)` 。

### Linux 文件种类和扩展名

想必大家都听说过这样一句话：任何设备在 Linux 下都是文件，但是文件也分为多种，除了上面介绍过的**一般文件(-)** 和**目录文件(d)** 之外，还包括下面这些文件类型

- 常规文件(regular file)：常规文件就是我们使用 ls -al 所显示出来的属性，也就是上面我们列出的第一个字符，

![图片](https://mmbiz.qpic.cn/mmbiz_jpg/A3ibcic1Xe0iaTF3zEqPfX4f0aTXsNrwyBFnV8jwpiatFVl58nPwXnQLYV6UBxBfh3gWSL5ibBeVYA6GPUicljhCwb6A/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1&wx_co=1)

#### 读取不同文件类型方式

文件类型又可以分为

1. 纯文本文件（ASCII），这是 Linux 系统中最多的一种文件类型，纯文本文件是我们能够直接看到的数据，你可以使用 `cat` 来直接看到这部分内容。比如我们最常用的设置 Linux 静态 ip 的文件 ens33 ，就可以使用 cat 命令来输出



```sh
 cat ifcfg-ens33
```

![图片](https://mmbiz.qpic.cn/mmbiz_jpg/A3ibcic1Xe0iaTF3zEqPfX4f0aTXsNrwyBFesqJxT6qO9e8XluUQk6vzqicItXzSf7hm1pvduK21WcHXZFsronh29w/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1&wx_co=1)

1. 二进制文件，在 Linux 中，查看二进制文件可以使用 `xxd` 或者 `od` 进行格式化输出
2. 数据格式文件，数据文件直接使用 cat 读取会显示乱码，但是它能够通过 `last` 指令进行输出

- 目录，目录没什么好说的，就是表示一个文件清单，目录的表示就是 *[d]* ，也就是 directory。
- 链接文件（link），链接文件就是某些程序执行时需要和这些链接文件进行链接才能执行的一种文件类型。
- 设备与设备文件（device），Linux 下的设备分为两种，块设备和字符设备：

块设备是一个能存储`固定大小块`信息的设备，它支持**以固定大小的块，扇区或群集读取和（可选）写入数据**。每个块都有自己的`物理地址`。通常块的大小在 512 - 65536 之间。所有传输的信息都会以`连续`的块为单位。块设备的基本特征是每个块都较为对立，能够独立的进行读写。常见的块设备有 **硬盘、蓝光光盘、USB 盘**。

块设备一般位于 /dev/sda 下，它的第一个属性为 *[b]* 。

![图片](https://mmbiz.qpic.cn/mmbiz_jpg/A3ibcic1Xe0iaTF3zEqPfX4f0aTXsNrwyBFpw6saFmlImfLk26GZXCJG9Vf9In8ib6m38tlM1OPV9F292ZE2vV97Dg/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1&wx_co=1)

另一类 I/O 设备是`字符设备`。字符设备以`字符`为单位发送或接收一个字符流，而不考虑任何块结构。常见的字符设备有 **打印机、网络设备、鼠标、以及大多数与磁盘不同的设备**。

![图片](https://mmbiz.qpic.cn/mmbiz_jpg/A3ibcic1Xe0iaTF3zEqPfX4f0aTXsNrwyBFwXpcBvX1x2Xjib7P5gPnF7jbdHqpEr66x31uRUic8sBhjIvpTmeeAx5w/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1&wx_co=1)

字符设备最大的特点就是一次性读取，不能够截断输出，举例来说，你不可能将鼠标一下跳到另外一个地方，而是采用平滑移动的方式才可以，字符设备的第一个属性是 *[c]* 。

- 数据接口文件（sockets）：数据接口文件顾名思义就是利用 socket 承接网络数据的，它的属性是 *[s]* ，一般在 /run 或者 /tmp 这些目录中看到。
- 数据输送文件（FIFO，pipe）：FIFO 也是一种特殊类型的文件，它的主要目的在于解决多个程序同时存取一个文件所造成的的错误问题，它的第一个属性为 *[p]* 。

#### Linux 扩展名

说到这个扩展名其实就很头疼，Linux 中是没有扩展名这个概念的，但是又有一些扩展名的命名方式，这就很尴尬，所以暂且成为扩展类型吧。一般有下面几种

- *.sh ，这是一个执行脚本或者批处理脚本，一般也被称为 shell 脚本，里面是一些 shell 语法写的指令。
- .tar，.tar.gz，.zip，*.tgz，这种扩展类型是打包的压缩文件，根据不同的打包方式有不同的扩展类型
- .html ，.php ：网页相关文件，分别代表 HTML 和 PHP 语法的网页文件。





















