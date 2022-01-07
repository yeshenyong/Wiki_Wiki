# vim功法——成神法



## vim 快捷键

多行注释：（后面有插件，可以玩的更high）

1. 进入命令行模式，按ctrl + v进入 visual block模式，然后按j, 或者k选中多行，把需要注释的行标记起来

2. 按大写字母I，再插入注释符，例如//

3. 按esc键就会全部注释了

 

取消多行注释：

1. 进入命令行模式，按ctrl + v进入 visual block模式，按字母l横向选中列的个数，例如 // 需要选中2列
2. 按字母j，或者k选中注释符号
3. 按d键就可全部取消注释



多行注释、取消多行注释法二

```shell
:27, 30#^#//#g
:27, 30#^//##g

:27, 30/^/#/g
:27. 30/^#//g
```





迅速选择一个单词，并在此文档上搜索复制

v(visual)  aw

`y` +`q` +`/`+`p`

copy + 键入 + paste





```shell
call plug#begin('~/.vim/plugged')
Plug 'preservim/tagbar'
Plug 'theniceboy/vim-deus'
Plug 'preservim/nerdtree'
Plug 'vim-scripts/AutoComplPop'
Plug 'frazrepo/vim-rainbow'
Plug 'preservim/nerdcommenter'
Plug 'airblade/vim-gitgutter'
call plug#end()

map <F7> :TagbarToggle<CR>
map <F8> :NERDTreeMirror<CR>
map <F8> :NERDTreeToggle<CR>
```



### 三级高手

下面，让我们看一下vim是怎么重复自己的：

1. `.` → (小数点) 可以重复上一次的命令
2. N<command> → 重复某个命令N次

```sh
2dd → 删除2行
3p → 粘贴文本3次
100idesu [ESC] → 会写下 “desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu desu “
. → 重复上一个命令—— 100 “desu “.
3. → 重复 3 次 “desu” (注意：不是 300，你看，VIM多聪明啊).
```

按单词移动：

> 1. `w` → 到下一个单词的开头。
> 2. `e` → 到下一个单词的结尾。
>
> \> 如果你认为单词是由默认方式，那么就用小写的e和w。默认上来说，一个单词由字母，数字和下划线组成
>
> \> 如果你认为单词是由blank字符分隔符，那么你需要使用大写的E和W。

下面，让我来说说最强的光标移动：（*）

> - `%` : 匹配括号移动，包括 `(`, `{`, `[`. （陈皓注：你需要把光标先移到括号上）
> - `*` 和 `#`:  匹配光标当前所在的单词，移动光标到下一个（或上一个）匹配单词（*是下一个，#是上一个）



你一定要记住光标的移动，因为很多命令都可以和这些移动光标的命令连动。很多命令都可以如下来干：

```
<start position><command><end position>
```

例如 `0y$` 命令意味着：

- `0` → 先到行头
- `y` → 从这里开始拷贝
- `$` → 拷贝到本行最后一个字符

你可可以输入 `ye`，从当前位置拷贝到本单词的最后一个字符。

你也可以输入 `y2/foo` 来拷贝2个 “foo” 之间的字符串。

还有很多时间并不一定你就一定要按y才会拷贝，下面的命令也会被拷贝：

- `d` (删除 )
- `v` (可视化的选择)
- `gU` (变大写)
- `gu` (变小写)
- 等等



你只需要掌握前面的命令，你就可以很舒服的使用VIM了。但是，现在，我们向你介绍的是VIM杀手级的功能。下面这些功能是我只用vim的原因。

##### 在当前行上移动光标: `0` `^` `$` `f` `F` `t` `T` `,` `;`

> - `0` → 到行头
> - `^` → 到本行的第一个非blank字符
> - `$` → 到行尾
> - `g_` → 到本行最后一个不是blank字符的位置。
> - `fa` → 到下一个为a的字符处，你也可以fs到下一个为s的字符。
> - `t,` → 到逗号前的第一个字符。逗号可以变成其它字符。
> - `3fa` → 在当前行查找第三个出现的a。
> - `F` 和 `T` → 和 `f` 和 `t` 一样，只不过是相反方向。
>   ![Line moves](http://yannesposito.com/Scratch/img/blog/Learn-Vim-Progressively/line_moves.jpg)

还有一个很有用的命令是 `dt"` → 删除所有的内容，直到遇到双引号—— `"。`



##### 区域选择 `<action>a<object>` 或 `<action>i<object>`

在visual 模式下，这些命令很强大，其命令格式为

```
<action>a<object>` 和 `<action>i<object>
```

- action可以是任何的命令，如 `d` (删除), `y` (拷贝), `v` (可以视模式选择)。
- object 可能是： `w` 一个单词， `W` 一个以空格为分隔的单词， `s` 一个句字， `p` 一个段落。也可以是一个特别的字符：`"、` `'、` `)、` `}、` `]。`





## vim 插件

> 没有插件的vim 是不完美的 —— 我

### vim plug（必须）

`vim-plug` 安装

```sh
wget https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
```

并把它放到`~/.vim/autoload/` 里（Vim用户）

把plug.vim放到正确的位置之后，你就可以开始安装插件了。在你的vim配置文件中，写下这两行：

Vim用户：

```sh
call plug#begin('~/.vim/plugged')

call plug#end()
```



![img](https://i0.hdslb.com/bfs/article/8c21fde01b33e4c2d8d26df99a685d98ca046060.png@942w_558h_progressive.webp)



插件管理工具

```visual basic
call plug#begin('~/.vim/plugged')
Plug 'theniceboy/vim-deus'
Plug 'scrooloose/nerdtree'
call plug#end()
```

```visual basic
修改完vimrc之后重启vim 
:PlugInstall        '安装新添加的插件'
:PlugStatus			"检查插件安装状态"
:PlugUpdate			"更新已安装的插件"
:PlugClean			"卸载在call plug#begin与call plug#end() 之间删去的插件"
:PlugUpgrade		"升级vim-plug本身"
```

远程管理：中间放GitHub 涉及插件url

本地vim 文件插件管理：中间放GitHub 涉及的vim 文件绝对地址

```visual basic
" start和end之间填入需要安装的插件
" 从github上安装 'username/projectname'
Plug 'junegunn/vim-easy-align'
Plug 'scrooloose/nerdtree'
" 从github url安装
" Plug 'githuburl'
" 从本地
" Plug '/tmp/plugin'
```



### 文件目录

https://github.com/preservim/nerdtree

```visual basic
:NERDTree
?（打开帮助，现实所有映射和命令，最完整的来源是文档:help NERDTree）
可快捷键做映射
map <F3> :NERDTreeMirror<CR>
map <F3> :NERDTreeToggle<CR>
```

快捷键

```visual basic
ctrl + w + r    移动当前窗口的布局位置
o       在已有窗口中打开文件、目录或书签，并跳到该窗口
go      在已有窗口 中打开文件、目录或书签，但不跳到该窗口
t       在新 Tab 中打开选中文件/书签，并跳到新 Tab
i       split 一个新窗口打开选中文件，并跳到该窗口
gi      split 一个新窗口打开选中文件，但不跳到该窗口
s       vsplit 一个新窗口打开选中文件，并跳到该窗口
gs      vsplit 一个新 窗口打开选中文件，但不跳到该窗口
!       执行当前文件
O       递归打开选中 结点下的所有目录
m    文件操作：复制、删除、移动等
```





### 获取成员在文件信息

https://github.com/preservim/tagbar

```visual basic
nmap  <F8> :TagbarToggle <CR>
```



```visual basic
<CR>：跳转到标签定义（CR是会车）
p: 展示方法位置，但不跳转光标
P: 展示方法位置，并跳转光标到该方法中
<ctrl + n> 向下移动一个方法位
<ctrl + p> 向上移动一个方法位
<space> 展示标签（方法）的类型信息
v: 隐藏不是public 的标签
```



### 代码提示

#### 工具一

https://github.com/vim-scripts/AutoComplPop

AutoComplPop

根据输入过的单词提示，全局单词检索展示插件

```visual basic
" AutoComplPop
" 输入一个字母即可补全
let g:acp_behaviorKeywordLength = 1
" 自动弹出是由键映射，对于通过移动来避免自动弹出很有用
let g:AutoComplPop_MappingDriven = 1
" 修改GUI 高粱参数 该设置全局有效
hi Pmenu guibg=#444444
hi PmenuSel ctermfg=7 ctermbg=4 guibg=#555555 guifg=#ffffff
```



#### 工具二





### 括号颜色

https://github.com/frazrepo/vim-rainbow

标点括号等标识符添加颜色，`.vimrc`

```shell
let g:rainbow_active = 1

let g:rainbow_load_separately = [
    \ [ '*' , [['(', ')'], ['\[', '\]'], ['{', '}']] ],
    \ [ '*.tex' , [['(', ')'], ['\[', '\]']] ],
    \ [ '*.cpp' , [['(', ')'], ['\[', '\]'], ['{', '}']] ],
    \ [ '*.{html,htm}' , [['(', ')'], ['\[', '\]'], ['{', '}'], ['<\a[^>]*>', '</[^>]*>']] ],
    \ ]

let g:rainbow_guifgs = ['RoyalBlue3', 'DarkOrange3', 'DarkOrchid3', 'FireBrick']
let g:rainbow_ctermfgs = ['lightblue', 'lightgreen', 'yellow', 'red', 'magenta']
```



### 多行代码注释

这是一款用于快速高效注释代码的插件

使用这款插件，你可以对多种文件类型的文件进行不同方式地、快速地注释。这对使用Vim来写代码或者修改配置文件的同学来说，无疑是提升效率和快感的一件利器

https://github.com/preservim/nerdcommenter

```sh
:help nerdcommenter
```



`.vimrc`优化

```sh
" Create default mappings
let g:NERDCreateDefaultMappings = 1

" Add spaces after comment delimiters by default
let g:NERDSpaceDelims = 1

" Use compact syntax for prettified multi-line comments
let g:NERDCompactSexyComs = 1

" Align line-wise comment delimiters flush left instead of following code indentation
let g:NERDDefaultAlign = 'left'

" Set a language to use its alternate delimiters by default
let g:NERDAltDelims_java = 1

" Add your own custom formats or override the defaults
let g:NERDCustomDelimiters = { 'c': { 'left': '/**','right': '*/' } }

" Allow commenting and inverting empty lines (useful when commenting a region)
let g:NERDCommentEmptyLines = 1

" Enable trimming of trailing whitespace when uncommenting
let g:NERDTrimTrailingWhitespace = 1

" Enable NERDCommenterToggle to check all selected lines is commented or not 
let g:NERDToggleCheckAllLines = 1
```



快捷键使用

```sh
1、 \cc 注释当前行和选中行
2、 \cn 没有发现和\cc有区别
3、 \c<空格> 如果被选区域有部分被注释，则对被选区域执行取消注释操作，其它情况执行反转注释操作
4、 \cm 对被选区域用一对注释符进行注释，前面的注释对每一行都会添加注释
5、 \ci 执行反转注释操作，选中区域注释部分取消注释，非注释部分添加注释（max important）
6、 \cs 添加性感的注释，代码开头介绍部分通常使用该注释
7、 \cy 添加注释，并复制被添加注释的部分
8、 \c$ 注释当前光标到改行结尾的内容
9、 \cA 跳转到该行结尾添加注释，并进入编辑模式
10、\ca 转换注释的方式，比如： /**/和//
11、\cl \cb 左对齐和左右对其，左右对其主要针对/**/
12、\cu 取消注释
```

如果想把默认的leader从“\”改为“,”， 则在在配置文件.vimrc中，加入下面一行

```sh
let mapleader=","
```





### Git 修改提示

https://github.com/airblade/vim-gitgutter

#### 激活

您可以明确地关闭和打开 vim-gitgutter（默认为打开）：

- turn off with `:GitGutterDisable`
- turn on with `:GitGutterEnable`
- toggle with `:GitGutterToggle`.

要为每个缓冲区切换 vim-gitgutter：

- turn off with `:GitGutterBufferDisable`
- turn on with `:GitGutterBufferEnable`
- toggle with `:GitGutterBufferToggle`

您可以打开和关闭标志（默认为打开）：

- turn on with `:GitGutterSignsEnable`
- turn off with `:GitGutterSignsDisable`
- toggle with `:GitGutterSignsToggle`.

您可以打开和关闭线条突出显示（默认为关闭）：

- turn on with `:GitGutterLineHighlightsEnable`
- turn off with `:GitGutterLineHighlightsDisable`
- toggle with `:GitGutterLineHighlightsToggle`.



> #### vim的终级插件
>
> CentOS下：yum erase emacs
>
> Ubuntu下：apt-get remove emacs







### 借鉴

https://www.bilibili.com/read/cv8264341?spm_id_from=333.999.0.0

https://coolshell.cn/articles/11312.html

https://coolshell.cn/articles/5426.html





### 扩展
