# screen 命令修改默认快捷键

```bash
screen -ls              # 查看所有screen
screen -S <screen-name> # 创建screen，并命名
ctr + A, D              # 快捷键，退出当前screen
screen -r <screen-name> # 进入screen
screen -X quit          # 删除screen，但没有指定会话
screen -X -S [session you want to kill] quit #删除screen，指定会话
screen -wipe            # 清除dead screens
```

​		所有人都知道screen默认快捷键前提（即escape配置）为ctrl+a，但是全网找遍了也只有万里挑一的文章写了escape的配置修改。

​		难道ctrl + a配置在Linux当中没有和终端回到行首快捷键冲突吗？（在开启screen的时候）

​		吐槽完了之后，直接出正文



> ​	进入screen 配置文件/etc/screenrc （不同系统文件路径不同，大家可以上午找一下）



```
sudo vim /etc/screenrc
```



加入以下命令

根据个人喜好，我喜欢ctrl + j 替换 ctrl + a

```bash
escape ^Jj
```



```
ESC
:wq!
```



即可



切记！！修改完记得把screen退出再进！！或者关掉再修改，不然现有的screen还是会保持以前的配置

