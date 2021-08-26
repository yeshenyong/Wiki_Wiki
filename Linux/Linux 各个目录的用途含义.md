# Linux 各个目录的用途含义



学习至https://blog.csdn.net/weixin_40331034/article/details/79522923



#### ./bin  重要的二进制(binary)应用程序



> ​	包含二进制文件，系统的所有用户使用的命令都在这个目录下

![img](https://img-blog.csdn.net/20180312093131132?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQwMzMxMDM0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



#### ./boot  启动（boot）配置文件

> ​	包含引导加载程序相关的文件

![img](https://img-blog.csdn.net/20180312093248752?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQwMzMxMDM0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



#### ./dev 设备（device）文件

> ​	包含设备文件，包含终端设备，USB或连接到系统的任何设备



#### ./etc 配置文件、启动脚本等（etc）

> ​	包含所有程序所需的配置文件等，也包含了用于启动/停止单个程序的启动和关闭shell脚本

![img](https://img-blog.csdn.net/2018031209331295?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQwMzMxMDM0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



#### ./home 本地用户主（home）目录

> ​	所有用户用home目录来存储他们的个人档案

![img](https://img-blog.csdn.net/20180312093318392?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQwMzMxMDM0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



#### ./lib 系统（libraries）文件

> ​	包含支持位于/bin和/sbin下的二进制文件的库文件

![img](https://img-blog.csdn.net/20180312093329261?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQwMzMxMDM0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



#### ./lost+found 在根(/)目录下提供一个遗失+查找（lost+found）系统

> ​	必须在root用户下才可以查看当前目录下的内容



#### ./media 挂载可移动介质(media)，诸如CD、数码相机

> ​	用于挂载可移动设备的临时目录



#### ./mnt 挂载(mounted)文件系统

> ​	临时安装目录，系统管理员可以挂载文件系统



#### ./opt 提供一个供可选的(optional)应用程序安装目录

> ​	包含从各个厂商的附加应用程序，附加的应用程序应该安装在/opt或者/opt的子目录



#### ./proc 特殊的动态目录，用以维护系统信息和状态，包括当前运行中进程(processes)信息

> ​	包含系统进程的相关信息，是一个虚拟的文件系统，包含有关正在运行的进程的信息，系统资源以文本信息形式存在



#### ./root root用户主文件夹，读作'slash-root'



#### ./sbin 重要的系统二进制（system binaries）文件

> ​	也是包含的二进制可执行文件。在这个目录下的Linux命令通常都是由系统管理员使用的，对系统进行维护。



#### ./sys 系统（system）文件



#### ./tmp 临时（temporary）文件

> ​	包含系统和用户创建的临时文件。当系统重启时，这个目录下的文件都被删除



#### ./usr 包含绝大部分所有用户（users）都能访问的应用程序和文件

> ​	包含二进制文件，库文件。文档和二级程序的源代码



#### ./var 经常变化的（variables）文件，诸如日志或数据库等

> ​	代表变量文件。在这个目录下可以找到内容可能增长的文件







