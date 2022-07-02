# git 基本命令





#### 初始化仓库

```
git init
```

然后

```
git add readme.md
```

```
git commit -m ""
```

```
git remote add origin https://github.com/yeshenyong/LeetCode.git //进行远程挂载
```

```
git push -u origin master
```

```
fatal: protocol 'https' is not supported 
错误
解决方法1：重启git bash
解决方法2：换ssh
```

```sh
法2的话：
报错fatal: remote origin already exists.
解决取消关联origin 远程库
git remote rm origin
git remote add origin git@github.com:yeshenyong/LeetCode.git
还是报错的话
error: Could not remove config section 'remote.origin'. 我们需要修改gitconfig文件的内容
找到你的github的安装路径
找到一个名为gitconfig的文件，打开它把里面的[remote "origin"]那一行删掉就好了！
```

```
git push -u origin master 还是报错
```

```
git@github.com: Permission denied (publickey).
fatal: Could not read from remote repository.

Please make sure you have the correct access rights
and the repository exists.
```

```
经过翻译可以得知我是在上传过程中没有权限所以被拒绝了，经过了对git的一番了解以后得知我好像做少了其中一个比较重要的一步，就是设置一个SSH KEY，即是密钥，（出于安全考虑，Github 服务器和我们本地的通讯要求使用 SSH Key 来验证）。这个时候只需设置一个密钥就行了。
```

解决方法

从github主页点击设置进入SSH and GPG keys

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190611014727899.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDM5NDc1Mw==,size_16,color_FFFFFF,t_70)

随后点击创建一个新的密钥"New SSH key"，

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190611014900820.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDM5NDc1Mw==,size_16,color_FFFFFF,t_70)

但是没有Key呀，要从哪里找呢？找到需要上传的项目文件夹的根目录，右键Git Base Here，如图所示。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190611015323937.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDM5NDc1Mw==,size_16,color_FFFFFF,t_70)

输入命令：ssh-keygen -t rsa -C ["876818551@qq.com](mailto:"876818551@qq.com)"
然后便一直猛击回车，有y/n的时候尽可能去翻译一下句子，而我下面这个y/n就是已经创有文件项目是否覆盖的意思。
接下来经过回车敲击后可以看到一个代码小图片，那就说明成功了。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190611015442313.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDM5NDc1Mw==,size_16,color_FFFFFF,t_70)

Github上面的key创建完成后就可以执行git push --set-upstream origin master这条指令了。





#### 删除远程文件夹

1.预览将要删除的文件（切记切记，要先预览）

```
git rm -r -n --cached 文件/文件夹名称

加上 -n 参数，执行命令时，是不会删除任何文件，而是展示此命令要删除的文件列表预览
```

2.确认无误后删除文件

```
git rm -r --cached 文件/文件夹名称
```

3.提交到本地并推送到远程服务器

```
git commit -m ""
git push origin master
```

4.修改本地.gitignore 文件 并提交





#### 提交文件

```
git add.		他会监控工作区的状态树，使用它会把工作时的所有变化提交到暂存区，包括文件内容修改(modified)以及新文件(new)，但不包括被删除的文件。
git add -u		他仅监控已经被add的文件（即tracked file），他会将被修改的文件提交到暂存区。add -u 不会提交新文件（untracked file）。（git add --update的缩写）
git add -A  	是上面两个功能的合集（git add --all的缩写）
```



总结：

·  git add -A  提交所有变化

·  git add -u  提交被修改(modified)和被删除(deleted)文件，不包括新文件(new)

·  git add .  提交新文件(new)和被修改(modified)文件，不包括被删除(deleted)文件









# git 命令大全

#### git 拉远程代码并切换分支

git clone

git checkout -b dev origin/dev



git push --set-upstream origin master



#### git 合并两个分支代码

分支切换

git checkout 分支名



本地分支和远程分支有区别



git merge ysy_dev

将ysy_dev分支合并到当前分支中





git merge 冲突修改





先取消merge合并

```
git merge --abort
```



https://www.liaoxuefeng.com/wiki/896043488029600/900004111093344



**git cherry-pick**可以选择某一个分支中的一个或几个commit(s)来进行操作

```
git cherry-pick
```

**例如：**
 $ git checkout v2.0分支
 $ git cherry-pick 38361a55    # 这个 38361a55 号码，位于v3.0分支中：

> $ git log
> commit 38361a55138140827b31b72f8bbfd88b3705d77a
> Author: Justin [Justin@xxx.com](https://link.jianshu.com?t=mailto:Justin@xxx.com)
> Date:   Sat Dec 10 00:11:44 2016 +0800

**1. 如果顺利，就会正常提交**。结果：

> Finished one cherry-pick.
> On branch v2.0分支
> Your branch is ahead of 'origin/old_cc' by 3 commits.

**2. 如果在cherry-pick 的过程中出现了冲突**

> Automatic cherry-pick failed.
> After resolving the conflicts,mark the corrected paths with 'git add <paths>' or 'git rm <paths>'and commit the result with:
> git commit -c 15a2b6c61927e5aed6111de89ad9dafba939a90b
> **或者:**
> error: could not apply 0549563... dev
> hint: after resolving the conflicts, mark the corrected paths
> hint: with 'git add <paths>' or 'git rm <paths>'
> hint: and commit the result with 'git commit'





#### git 查看远程仓库

```
git remote -v
```



#### git 创建分支并提交分支



创建分支并切换到分支下

```
git checkout -b ysy_branch
```

将分支信息推送到远程上

```
git push origin HEAD -u
```

然后确保已经切换到分支上了，就可以进行提交了

```sh
git add ...
git commit ...
git push ...
git push origin master:my_remote_new_branch
git push origin origin/origin/ysy_estimator:ysy_estimator
git push origin origin/ysy_estimator:ysy_estimator
git push origin 本地分支:远程分支
```



#### git commit 撤销

1、仅仅撤销commit ,保留add

```sh
git reset --soft HEAD^

//当我们执行 --soft 命令后，可以看到控制台无任何输出。它只是把HEAD指向了上一个版本。
```

2、仅仅撤销commit 并且也撤销add。(也就是删除工作空间的改动代码)

```sh
git reset --hard HEAD^
```



#### git commit 再补交add

有时候，我们提交完成后发现漏掉几个文件没有添加，或者信息写错了，这时候就需要对操作进行撤销。

```
git commit --amend
```

```
git commit -m 'xxxxx'
git add file
git commit --amend
```

最终只会有一个提交，第二次提交将代替第一次提交结果。

 

#### git  commit 多行注释

如果想要用多行注释，可以使用如下写法：

```bash
git commit -m "测试多行注释" -m "工具类注释添加"
```

![img](https://img-blog.csdnimg.cn/20210225113634699.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x3eDM1NjQ4MQ==,size_16,color_FFFFFF,t_70)



#### git 分支查看和切换

查看所有分支

```
git branch -a
```

查看当前使用分支

```
git branch
```

切换分支

```
git checkout 分支名
```





#### git 查看日志

```
git log会按时间先后顺序列出所有的提交git log --stat 显示基本信息之外，还附带了每次提交的变化git log --pretty=format:"%h - %an, %ar : %s"git log --since=2.weeksgit show commitID
```





#### git 查看最新分支

git fetch	将远程主机的最新内容拉到本地



```sh
git branch -a		//显示所有分支
```



```sh
git checkout -b vbranch origin/vbranch	//切换分支
```





#### git 更新当前分支版本

```
git fetch
```

 重新

```
git pull origin 对应分支
```



#### git 版本穿梭机



```sh
git loggit log --pretty=oneline	//缩小git log打印信息
```

现在有19、20、21世纪，现在是21世纪

版本回退到20世纪，HEAD^意味着上一版本、HEAD^^上上个版本、HEAD~100前100个版本

```shell
git reset --hard HEAD^
```



完了穿越到20世纪，怎么回来呢

法1：未关闭之前git log窗口，知道之前commit 版本的commit ID

```
git reset --hard COMMITID或者COMMITID前几位
```

法2：关闭了git log窗口，不知道21世纪的commit ID

在Git中，总有后悔药可以吃。恢复到21世纪的世界，必须要有21世纪的commit ID。Git提供一个命令`git reflog`用来记录你的每一次命令

```sh
git reflog
```

从git reflog得知21世纪的commit ID是xxx，然后执行法1的对应命令，回归时光机。



Git的版本回退速度非常快，因为Git在内部有个指向当前版本的`HEAD`指针，当你回退版本的时候，Git仅仅是把HEAD从指向`append GPL`：

```ascii
┌────┐│HEAD│└────┘   │   └──> ○ append GPL        │        ○ add distributed        │        ○ wrote a readme file
```

改为指向`add distributed`：

```ascii
┌────┐│HEAD│└────┘   │   │    ○ append GPL   │    │   └──> ○ add distributed        │        ○ wrote a readme file
```



- `HEAD`指向的版本就是当前版本，因此，Git允许我们在版本的历史之间穿梭，使用命令`git reset --hard commit_id`。
- 穿梭前，用`git log`可以查看提交历史，以便确定要回退到哪个版本。
- 要重返未来，用`git reflog`查看命令历史，以便确定要回到未来的哪个版本。



**一、问题描述**
在利用github实现多人合作程序开发的过程中，我们有时会出现错误提交的情况，此时我们希望能撤销提交操作，让程序回到提交前的样子，本文总结了两种解决方法：回退（reset）、反做（revert）。

**二、背景知识**

> git的版本管理，及HEAD的理解
> 使用git的每次提交，Git都会自动把它们串成一条时间线，这条时间线就是一个分支。如果没有新建分支，那么只有一条时间线，即只有一个分支，在Git里，这个分支叫主分支，即master分支。有一个HEAD指针指向当前分支（只有一个分支的情况下会指向master，而master是指向最新提交）。每个版本都会有自己的版本信息，如特有的版本号、版本名等。如下图，假设只有一个分支：![这里写图片描述](https://imgconvert.csdnimg.cn/aHR0cDovL2ltZy5ibG9nLmNzZG4ubmV0LzIwMTgwNDE0MjAwNDQyNjAz)



**三、解决方法**
**方法一：git reset**
原理： git reset的作用是修改HEAD的位置，即将HEAD指向的位置改变为之前存在的某个版本，如下图所示，假设我们要回退到版本一：

![这里写图片描述](https://imgconvert.csdnimg.cn/aHR0cDovL2ltZy5ibG9nLmNzZG4ubmV0LzIwMTgwNDE0MjEyMjIxMDMz)

适用场景： 如果想恢复到之前某个提交的版本，且那个版本之后提交的版本我们都不要了，就可以用这种方法。

具体操作：

1. 查看版本号：
   可以使用命令“git log”查看：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190731102022900.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3l4bHNoaw==,size_16,color_FFFFFF,t_70)

也可以在github网站上查看：

![这里写图片描述](https://imgconvert.csdnimg.cn/aHR0cDovL2ltZy5ibG9nLmNzZG4ubmV0LzIwMTgwNDE0MjAzMzQ0MTM4)

2. 使用“git reset --hard 目标版本号”命令将版本回退：

![这里写图片描述](https://imgconvert.csdnimg.cn/aHR0cDovL2ltZy5ibG9nLmNzZG4ubmV0LzIwMTgwNDE0MjAyNDEzNjY2)

再用“git log”查看版本信息，此时本地的HEAD已经指向之前的版本：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190731102055301.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3l4bHNoaw==,size_16,color_FFFFFF,t_70)

3. 使用“git push -f”提交更改：
   此时如果用“git push”会报错，因为我们本地库HEAD指向的版本比远程库的要旧：

![这里写图片描述](https://imgconvert.csdnimg.cn/aHR0cDovL2ltZy5ibG9nLmNzZG4ubmV0LzIwMTgwNDE0MjAzNjA1Nzk3)

所以我们要用“git push -f”强制推上去，就可以了：

![这里写图片描述](https://imgconvert.csdnimg.cn/aHR0cDovL2ltZy5ibG9nLmNzZG4ubmV0LzIwMTgwNDE0MjAzOTU3MjE3)

在github图形化界面上看，远程库的HEAD也已经指向目标版本：

![这里写图片描述](https://imgconvert.csdnimg.cn/aHR0cDovL2ltZy5ibG9nLmNzZG4ubmV0LzIwMTgwNDE0MjA0MjQ1ODkx)

回退成功！
**方法二：git revert**
原理： git revert是用于“反做”某一个版本，以达到撤销该版本的修改的目的。比如，我们commit了三个版本（版本一、版本二、 版本三），突然发现版本二不行（如：有bug），想要撤销版本二，但又不想影响撤销版本三的提交，就可以用 git revert 命令来反做版本二，生成新的版本四，这个版本四里会保留版本三的东西，但撤销了版本二的东西。如下图所示：

![这里写图片描述](https://imgconvert.csdnimg.cn/aHR0cDovL2ltZy5ibG9nLmNzZG4ubmV0LzIwMTgwNDE0MjA1ODE2MTg4)

适用场景： **如果我们想撤销之前的某一版本，但是又想保留该目标版本后面的版本，记录下这整个版本变动流程，就可以用这种方法**。

具体操作：
举个例子，现在库里面有三个文件：READ.md、text.txt、text2.txt。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190726111023401.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3l4bHNoaw==,size_16,color_FFFFFF,t_70)

1. 查看版本号：
   可以通过命令行查看（输入git log）：
   如图，最近的两个版本分别叫：“add text.txt”（即新增了文件text.txt）、“add text2.txt”（新增了文件text2.txt）。这个时候我们不需要text.txt这个文件了，那就是说不想要“add text.txt”那个版本的操作，那可以通过反做“add text.txt”这个版本来实现。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190726105234748.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3l4bHNoaw==,size_16,color_FFFFFF,t_70)

也可以通过github网站图形化界面查看版本号：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190726105637358.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3l4bHNoaw==,size_16,color_FFFFFF,t_70)

2.使用“git revert -n 版本号”反做，并使用“git commit -m 版本名”提交：
（1）反做，使用“git revert -n 版本号”命令。如下命令，我们反做版本号为8b89621的版本：

```
git revert -n 8b89621019c9adc6fc4d242cd41daeb13aeb9861
```


注意： 这里可能会出现冲突，那么需要手动修改冲突的文件。而且要git add 文件名。
（2）提交，使用“git commit -m 版本名”，如：

```
git commit -m "revert add text.txt" 
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190726110223433.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3l4bHNoaw==,size_16,color_FFFFFF,t_70)
此时可以用“git log”查看本地的版本信息，可见多生成了一个新的版本，该版本反做了“add text.txt”版本，但是保留了“add text2.txt”版本：

3.使用“git push”推上远程库：

```
git push
```


查看github上显示的远程库版本信息：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190726110646667.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3l4bHNoaw==,size_16,color_FFFFFF,t_70)

此时查看仓库的文件，剩下两个：READ.md、text2.txt

![在这里插入图片描述](https://img-blog.csdnimg.cn/201907261108187.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3l4bHNoaw==,size_16,color_FFFFFF,t_70)


反做成功！



#### git pull 冲突解决

```erlang
error: Your local changes to 'c/environ.c' would be overwritten by merge.  Aborting.Please, commit your changes or stash them before you can merge.
```

> 		这个提示意思就是说更新下来的内容和本地修改的内容有冲突，先提交你改变的内容或者先将你本地修改的内容暂时存起来。
>				
> 	下面我们就分几步解决处理这个pull冲突问题.



1.存储本地修改的内容

```sh
git stash
```

这句命令就是将本地修改的代码做一份备份存储起来，可以用git stash list 查看刚刚备份保存的内容：

![img](https://upload-images.jianshu.io/upload_images/4970496-84f5c1a7a77608c6.png?imageMogr2/auto-orient/strip|imageView2/2/w/1130/format/webp)

其中stash@{0}就是刚刚备份存储的标记

2.pull内容

将本地代码做了备份保存后，就可以pull远端代码

```shell
git pull
```

3.还原备份暂存的代码

```sh
git stash pop stash@{0}
```

stash@{0}是你刚刚备份存储的标记
这时候系统会提示类似以下的信息：

```sh
Auto-merging c/environ.cCONFLICT (content): Merge conflict in c/environ.c
```

这个提示内容意思就是系统自动合并修改的内容，但是当中会有冲突，需要解决其中的冲突。

4.解决文件中的冲突内容

打开上面提示的冲突文件，会看到类似的内容：

![img](https://upload-images.jianshu.io/upload_images/4970496-92f347853efe0765.png?imageMogr2/auto-orient/strip|imageView2/2/w/698/format/webp)

​	其中Updated upstream和=======之间的内容是从远端pull下来的代码，=======和Stashed changes之间的内容则是你本地修改的内容。这时候，需要你修改决定留下哪些需要的内容。

​	最后，解决完冲突，就可以正常git提交了。



#### git 删除文件

法1：从版本库中删除该文件

```sh
git rm test.txtgit commit -m "message"
```

法2：完了，又手贱删错了，但是版本库里还有，还是很轻松把误删恢复的

```sh
git checkout -- test.txt
```

​	git rm用于删除一个文件，如果一个文件已经被提交版本库，那么永远不用担心误删，但是要小心，你只能恢复文件到最新版本，你会丢失**最近一次提交后你修改的内容**。



#### git 文件回退

```sh
git checkout -- file.txt
```

两种情况

一种是`readme.txt`自修改后还没有被放到暂存区，现在，撤销修改就回到和版本库一模一样的状态；

一种是`readme.txt`已经添加到暂存区后，又作了修改，现在，撤销修改就回到添加到暂存区后的状态。

总之，就是让这个文件回到最近一次`git commit`或`git add`时的状态。



#### git 出现root账号提交



```
git config --global user.name "yeshenyong.ysy"git config --global user.email或者，如果出现$HOME not setgit config --system user.name "yeshenyong.ysy"git config --system user.email
```



重置提交信息

```
git commit --amend --reset-author
```



#### **git 创建与合并分支**





#### git 工作区和暂存区





版本库



#### .gitignore 

​	我注意到很多开发者没有使用 `.gitignore` 文件，尽管使用 `.gitignore` 文件来指定你不希望 Git 在版本控制中跟踪的文件是[最佳实践](https://link.zhihu.com/?target=https%3A//opensource.com/article/20/7/git-repos-best-practices)之一。`.gitignore` 可以提高代码质量，所以你不应该忽略版本库中的 [.gitignore](https://link.zhihu.com/?target=https%3A//git-scm.com/docs/gitignore)。

**什么是 .gitignore？**

Git 仓库中的文件可以是：

1. **未跟踪的**：未被暂存或提交的变更。
2. **跟踪的**：已暂存或提交的变更。
3. **忽略的**：你让 Git 忽略的文件。



​	有些文件你希望 Git 忽略，不要在你的版本库中跟踪它，这些文件包括许多自动生成的或特定于平台的文件，以及其他本地配置文件，如：

1. 含有敏感信息的文件
2. 编译出的代码，如 `.dll` 或 `.class`。
3. 系统文件，如 `.DS_Store` 或 `Thumbs.db`。
4. 含有临时信息的文件，如日志、缓存等。
5. 生成的文件，如 `dist` 文件夹。



​	如果你不想让 Git 跟踪版本库中的某些文件，不过这不能通过 [Git 命令](https://link.zhihu.com/?target=https%3A//acompiler.com/git-commands/)做到。（虽然你可以用 `git rm` 命令停止跟踪一个文件，比如 `git rm --cached`。）相反，你需要使用 `.gitignore` 文件，这是一个告诉 Git 不要跟踪哪些文件的文本文件。

​	创建 `.gitignore` 文件很简单，只需创建一个文本文件并命名为 `.gitignore`。记得在文件名的开头有一个点（`.`）。就这样就完成了。

**编写 .gitignore 文件的规则**

根据[文档](https://link.zhihu.com/?target=https%3A//git-scm.com/docs/gitignore)，“`.gitignore` 文件中的每一行都指定了一个模式。”

​	在此上下文中，“模式”可以指一个特定的文件名，或者指文件名的某些部分结合上通配符。换句话说，`example.txt` 是匹配名为 `example.txt` 的文件的有效模式，而 `ex*txt` 是匹配名为 `example.txt` 以及 `export.txt` 的文件的有效模式。

以下是一些帮助你正确设置 `.gitignore` 文件的基本规则：

1. 任何以哈希（`#`）开头的行都是注释。
2. `\` 字符可以转义特殊字符。
3. `/` 字符表示该规则只适用于位于同一文件夹中的文件和文件夹。
4. 星号（`*`）表示任意数量的字符（零个或更多）。
5. 两个星号（`**`）表示任意数量的子目录。
6. 一个问号（`?`）代替零个或一个字符。
7. 一个感叹号（`!`）会反转特定的规则（即包括了任何被前一个模式排除的文件）。
8. 空行会被忽略，所以你可以用它们来增加空间，使你的文件更容易阅读。
9. 在末尾添加 `/` 会忽略整个目录路径。



**本地与全局 .gitignore 文件的比较**

有两种类型的 `.gitignore` 文件：

- **本地**：放在 Git 仓库的根目录下，只在该仓库中工作，并且必须提交到该仓库中。
- **全局**：放在你的主目录根目录下，影响你在你的机器上使用的每个仓库，不需要提交。



​	很多开发者在项目仓库中使用本地的 `.gitignore` 文件，但很少有人使用全局的 `.gitignore` 文件。使用全局文件最显著的优势是，你不需要提交就可以使用它，而且做一个改动会影响你所有的版本库

```gas
built/*build/graphlearn/proto/service.pb*graphlearn/proto/service.grpc*graphlearn.egg-info/graphlearn/python/lib
```

**Git 忽略的优势**

除了确保特定的文件不被 Git 追踪之外，使用 `.gitignore` 文件还有其他好处。

1. 通过忽略不需要的文件，它可以帮助你保持代码库的干净。
2. 它可以控制代码库的大小，这在你正在做一个大项目的时候特别有用。
3. 你的每一次提交、推送和拉取请求都将是干净的。



#### git 与 vim的结合

```
git diff | vim -
```





​	Git 很强大，但归根结底，它只是一个计算机程序而已。使用最佳实践并保持你的代码仓库稳定是一个团队的努力，其中要做到一件事就是使用 `.gitignore` 文件。

























































