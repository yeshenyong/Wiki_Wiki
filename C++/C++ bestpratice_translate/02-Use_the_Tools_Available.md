# 使用有效的工具

应该在开发过程的早期建立一个用于执行这些工具的自动化框架。检出源代码、构建和执行测试所使用的命令不应超过2-3个。一旦测试完成执行，您应该对代码的状态和质量有一个几乎完整的了解。

## 源代码控制

源代码控制对于任何软件开发项目都是绝对必要的。如果你还没有使用，那就开始使用

- [GitHub](https://github.com/) - 允许无限的公共存储库，和无限的私人存储库，最多3个协作者
- [Bitbucket](https://bitbucket.org/) - 允许无限制的私人存储库，最多5个合作者，免费
- [SourceForge](http://sourceforge.net/) - 只开放源码托管
- [GitLab](https://gitlab.com/) - 允许无限的公共和私人存储库，包括无限的CI runner，免费
- [Visual Studio Online](https://visualstudio.com/) (http://www.visualstudio.com/what-is-visual-studio-online-vs) - 允许无限的公共存储库，必须为私有存储库付费。仓库可以是git或TFVC。另外:问题跟踪、项目计划(多个敏捷模板，如SCRUM)、集成托管构建、将所有这些集成到Microsoft Visual Studio中。仅Windows



## 构建工具

使用被广泛接受的行业标准构建工具。这可以防止您在**发现/链接**一个新的**库/包**进你的产品/等时做重复开发轮子的工作。例子包括:

- [Autotools](https://autotools.io/) - 传统的GNU构建系统
- CMake
  - Consider（认为）: https://github.com/sakra/cotire/ 构建性能
  - Consider: https://github.com/toeb/cmakepp 为提高可用性
  - Utilize（使用）: https://cmake.org/cmake/help/v3.6/command/target_compile_features.html 对于C++ standard flags
  - Consider: https://github.com/cheshirekow/cmake_format 用于自动格式化CMakeLists.txt文件
  - See the [Further Reading](https://github.com/cpp-best-practices/cppbestpractices/blob/master/11-Further_Reading.md) section for CMake specific best practices
  - `cmake --build` 提供用于编译项目的通用接口，而不考虑平台
- [Waf](https://waf.io/)
- [FASTBuild](http://www.fastbuild.org/)
- [Ninja](https://ninja-build.org/) - 可以大大提高较大项目的增量构建时间 可以作为CMake的目标
- [Bazel](http://bazel.io/) - 使用网络工件缓存和远程执行的快速增量构建。
- [Buck](http://buckbuild.com/) - 类似于Bazel，对iOS和Android都有很好的支持
- [gyp](https://chromium.googlesource.com/external/gyp/) - 谷歌的构建工具 for chromium.
- [maiken](https://github.com/Dekken/maiken) - 具有maven式配置风格的跨平台构建工具
- [Qt Build Suite](http://doc.qt.io/qbs/) - 来自Qt的跨平台构建工具
- [meson](http://mesonbuild.com/index.html) - 开源构建系统意味着速度非常快，更重要的是，尽可能使用户友好
- [premake](https://premake.github.io/)
- [xmake](https://xmake.io/) - 一个基于Lua的跨平台构建工具。现代C/ C++编译工具，支持多语言混合编译
- [build2](https://build2.org/) - 一个cargo-like的完整工具链(构建系统、包管理器、项目管理器)

**记住，它不仅仅是一个构建工具，它也是一种编程语言。尽量维护良好干净的构建脚本，并遵循您正在使用的工具的推荐实践**

## 包管理器

包管理是c++中的一个重要主题，目前还没有明显的赢家。考虑使用包管理器来帮助您跟踪项目的依赖关系，并使新人更容易开始项目

- [Conan](https://www.conan.io/) - C++的跨平台依赖管理器
- [hunter](https://github.com/ruslo/hunter) - 用于C/ C++的CMake驱动跨平台包管理器
- [C++ Archive Network (CPPAN)](https://cppan.org/) - C++的跨平台依赖管理器
- [qpm](https://www.qpm.io/) - Qt的包管理器
- [build2](https://build2.org/) - 一个cargo-like的完整工具链(构建系统、包管理器、项目管理器)
- [Buckaroo](https://buckaroo.pm/) - 真正去中心化的跨平台依赖管理器，适用于C/C++等等
- [Vcpkg](https://github.com/microsoft/vcpkg) - Microsoft C++ Library 管理器 for Windows, Linux, and MacOS - [description](https://docs.microsoft.com/en-us/cpp/build/vcpkg)
- [CPM](https://github.com/cpm-cmake/CPM.cmake) - 用于现代CMake的CMake包管理器

## 持续集成

一旦选择了构建工具，就建立一个持续集成环境

持续集成(CI)工具在将更改推送到存储库时自动构建源代码。它们可以私下托管，也可以使用CI主机。

- [Travis CI](http://travis-ci.org/)
  - 可以很好地使用 C++
  - 设计用于与GitHub一起使用
  - 免费提供给GitHub上的公共存储库
- [AppVeyor](http://www.appveyor.com/)
  - 支持Windows, MSVC和MinGW
  - 免费提供给GitHub上的公共存储库
- [Hudson CI](http://hudson-ci.org/) / [Jenkins CI](https://jenkins-ci.org/)
  - Java Application Server是必需的
  - supports Windows, OS X, and Linux
  - 可扩展的插件
- TeamCity
  - 有开源项目的免费选项
- Decent CI
  - 简单的特别的持续集成，将结果发布到GitHub
  - supports Windows, OS X, and Linux
  - used by [ChaiScript](http://chaiscript.com/ChaiScript-BuildResults/full_dashboard.html)
- Visual Studio Online（http://www.visualstudio.com/what-is-visual-studio-online-vs）
  - 与Visual Studio Online的源代码库紧密集成
  - 使用MSBuild (Visual Studio的构建引擎)，可在Windows、OS X和Linux上使用
  - 提供托管的生成代理，还允许用户提供生成代理
  - 可以在Microsoft Visual Studio中控制和监控
  - 通过Microsoft Team Foundation Server进行内部安装
- GitLab
  - 使用自定义Docker映像，因此可以用于C++
  - has free shared runners
  - has trivial processing of result of coverage analyze（有简单的处理结果的覆盖率分析）

### 编译器

使用所有可用和合理的警告选项。有些警告选项只在启用了优化时有效，或者选择的优化级别越高工作得越好，例如GCC的`-Wnull-dereference`

您应该为您的平台使用尽可能多的编译器。每个编译器对标准的实现略有不同，支持多个将有助于确保最可移植、最可靠的代码

#### GCC / Clang

`-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic` -使用这些并考虑以下(参见下面的描述)

- `-pedantic` - 对语言扩展的警告
- `-Wall -Wextra` 合理的和标准的
- `-Wshadow` 如果变量声明从上下文中隐藏了一个变量，则警告用户
- `-Wnon-virtual-dtor` 如果带有虚函数的类有非虚析构函数，则警告用户。这有助于捕获难以跟踪的内存错误
- `-Wold-style-cast` 对C类型转换发出警告
- `-Wcast-align` 警告潜在的性能问题强制转换
- `-Wunused` 警告任何未使用的东西
- `-Woverloaded-virtual`如果重载(而不是重写)虚函数，则发出警告
- `-Wpedantic` (all versions of GCC, Clang >= 3.2) 如果使用非标准C++，则发出警告
- `-Wconversion` 对可能丢失数据的类型转换发出警告
- `-Wsign-conversion` (Clang all versions, GCC >= 4.3) 警告符号转换
- `-Wmisleading-indentation` (only in GCC >= 6.0) 如果缩进块与块之间不存在对应，则发出警告
- `-Wduplicated-cond` (only in GCC >= 6.0) 如果`if `/ `else`链有重复的条件，则发出警告
- `-Wduplicated-branches` (only in GCC >= 7.0) 如果`if` / `else`分支有重复的代码，则发出警告
- `-Wlogical-op` (only in GCC) 警告可能需要按位使用的逻辑操作
- `-Wnull-dereference` (only in GCC >= 6.0) 如果检测到空引用（null dereference）则发出警告
- `-Wuseless-cast` (only in GCC >= 4.8) 如果对同一类型执行强制转换，则发出警告
- `-Wdouble-promotion` (GCC >= 4.6, Clang >= 3.8) 如果`float`隐式提升为`double`则发出警告
- `-Wformat=2` 对格式化输出函数(即`printf`)的安全问题发出警告
- `-Wlifetime`(目前只有`Clang`的特殊分支)显示对象生命周期问题

考虑使用`-Weverything`和禁用少数警告你需要在Clang

 `-Weffc++`警告模式可能太吵了，但如果它适用于你的项目，也可以使用它。

#### General（通用）

从一开始就设置非常严格的警告。在项目进行后试图提高预警级别可能是痛苦的。

考虑使用将警告视为错误设置。`/WX`与MSVC，` -Werror`与GCC / Clang

## LLVM-based工具

基于LLVM的工具与可以输出编译命令数据库的构建系统(如cmake)最适合，例如:

```sh
$ cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .
```

如果您不使用这样的构建系统，您可以考虑构建EAR，它将与您的构建系统挂钩，并为您生成一个编译命令数据库

CMake现在还提供了在正常编译期间调用`clang-tidy`的内置支持

- [include-what-you-use](https://github.com/include-what-you-use), [example results](https://github.com/ChaiScript/ChaiScript/commit/c0bf6ee99dac14a19530179874f6c95255fde173)
- [clang-modernize](http://clang.llvm.org/extra/clang-modernize.html), [example results](https://github.com/ChaiScript/ChaiScript/commit/6eab8ddfe154a4ebbe956a5165b390ee700fae1b)
- [clang-check](http://clang.llvm.org/docs/ClangCheck.html)
- [clang-tidy](http://clang.llvm.org/extra/clang-tidy.html)

## Static Analyzers（静态分析程序）

最好的选择是静态分析器，您可以将其作为自动化构建系统的一部分运行。Cppcheck和clang满足免费选项的要求

### Coverity Scan

Coverity有一个免费的(开源的)静态分析工具包，它可以在与Travis CI和AppVeyor集成的每个提交上工作

### PVS-Studio

PVS-Studio是一个用于检测程序源代码中的bug的工具，用C, C++和c#编写。它对个人学术项目、开源非商业项目和个人开发者的独立项目都是免费的。它可以在Windows和Linux环境下工作

### Cppcheck

[Cppcheck](http://cppcheck.sourceforge.net/)是免费的开源的。它努力实现0误报，并且做得很好。因此，所有警告都应启用: `--enable=all`

Notes:

- 为了正确的工作，它需要头文件格式良好的路径，所以在使用之前不要忘记传递: `--check-config`.
- 如果`-j`大于1，则不能查找未使用的头文件。
- 如果需要检查所有带有`#ifdef`的代码，请记住添加`--force`

### cppclean

[cppclean](https://github.com/myint/cppclean) - 开源静态分析器专注于发现C++源代码中导致大型代码库开发缓慢的问题。

### CppDepend

[CppDepend](https://www.cppdepend.com/) 通过分析和可视化代码依赖关系，通过定义设计规则，通过进行影响分析，并比较不同版本的代码，简化了对复杂C/ C++代码库的管理。它对OSS贡献者是免费的。

### Clang's Static Analyzer

Clang的分析器的默认选项适用于各自的平台。可以直接使用 [from CMake](http://garykramlich.blogspot.com/2011/10/using-scan-build-from-clang-with-cmake.html). 它们也可以通过[LLVM-based Tools](https://github.com/cpp-best-practices/cppbestpractices/blob/master/02-Use_the_Tools_Available.md#llvm-based-tools)中的clang-check和clang-tidy调用。

Also, [CodeChecker](https://github.com/Ericsson/CodeChecker) 可用作clang的静态分析的前端

`clang-tidy` 可以很容易地使用Visual Studio通过 [Clang Power Tools](https://clangpowertools.com/) 扩展.

### MSVC's Static Analyzer

可以使用 `/analyze` [command line option](http://msdn.microsoft.com/en-us/library/ms173498.aspx). 现在我们将继续使用默认选项

### Flint / Flint++

[Flint](https://github.com/facebook/flint) and [Flint++](https://github.com/L2Program/FlintPlusPlus) 是针对Facebook的编码标准分析C++代码的筛选器

### OCLint

[OCLint](http://oclint.org/) 是一个免费的、自由的、开源的静态代码分析工具，它可以通过许多不同的方式来提高C++代码的质量

### ReSharper C++ / CLion

这两个来自JetBrains的工具都提供了一定程度的静态分析和可以做得更好的常见事情的自动修复。他们为开源项目负责人提供了免费许可证选项.

### Cevelop

基于Eclipse的[Cevelop](https://www.cevelop.com/) IDE有各种静态分析和重构/代码修复工具可用。例如，您可以用C++的构造表达式替换宏，重构名称空间(提取/内联使用，限定名称)，并将代码重构为C++ 11的统一初始化语法。Cevelop 是免费使用的.

### Qt Creator

Qt Creator可以插入clang静态分析器

### clazy

[clazy](https://github.com/KDE/clazy) 是一个基于clang的工具来分析Qt的使用

### IKOS

[IKOS](https://ti.arc.nasa.gov/opensource/ikos/) 是一个开源静态分析仪，由NASA开发。它以抽象解释为基础。它是用C++编写的，使用LLVM为C和c++提供了一个分析器. 源代码目录 [available on Github](https://github.com/NASA-SW-VnV/ikos).

## Runtime Checkers（运行时检查）

### Code Coverage Analysis（代码覆盖率分析）

当测试被执行时，一个覆盖分析工具应该被运行，以确保整个应用程序被测试。不幸的是，覆盖率分析需要禁用编译器优化。这将导致测试执行时间大大延长

- [Codecov](https://codecov.io/)
  - 与Travis CI和AppVeyor集成
  - 免费用于开源项目
- [Coveralls](https://coveralls.io/)
  - 与Travis CI和AppVeyor集成
  - 免费用于开源项目
- [LCOV](http://ltp.sourceforge.net/coverage/lcov.php)
  - 非常可配置
- [Gcovr](http://gcovr.com/)
- [kcov](http://simonkagstrom.github.io/kcov/index.html)
  - 集成codecov和coveralls
  - 执行代码覆盖报告，不需要特殊的编译器标志，只需要调试符号
- [OpenCppCoverage](https://github.com/OpenCppCoverage/OpenCppCoverage) - 面向Windows的开源覆盖率报告工具

### Heap profiling（堆分析）

- Valgrind
  - 是一个运行时代码分析器，它可以检测内存泄漏、竞争条件和其他相关问题。各种Unix平台都支持它
- Heaptrack
  - A profiler created by a Valgrind's Massif developer. 和Massif很像，有优点也有缺点，不过更直观。
- [Dr Memory](http://www.drmemory.org/)
- [Memoro](https://epfl-vlsc.github.io/memoro/) - 一个详细的堆分析器

### CPU profiling（CPU性能分析）

- [Hotspot](https://github.com/KDAB/hotspot) - 一个直观的前端来可视化[perf](https://perf.wiki.kernel.org/) CPU分析器产生的数据.
- [uftrace](https://github.com/namhyung/uftrace) - 可以用来生成函数调用图的程序执行。

### Reverse engineering tools（逆向工程工具）

- [Cutter](https://cutter.re/) - [Radare2](https://www.radare.org/n/radare2.html)的前端。提供了反编译器、反汇编器、图形可视化器、十六进制编辑器等工具
- [Ghidra](https://ghidra-sre.org/) - Ghidra是美国国家安全局(NSA)开发的免费开源逆向工程工具

### GCC / Clang Sanitizers

这些工具提供了许多与Valgrind相同的特性，但是内置在编译器中。它们易于使用，并提供出错的报告

- AddressSanitizer
- MemorySanitizer
- ThreadSanitizer
- UndefinedBehaviorSanitizer

注意可用的 sanitizer 可用选项，包括运行时选项  https://kristerw.blogspot.com/2018/06/useful-gcc-address-sanitizer-checks-not.html

### Fuzzy Analyzers（模糊分析）

如果您的项目接受用户定义的输入，考虑运行一个模糊输入测试器

这两种工具都使用覆盖率报告来寻找新的代码执行路径，并尝试为您的代码产生新的输入。它们可以发现崩溃、挂起和你不知道是有效的输入

- [american fuzzy lop](http://lcamtuf.coredump.cx/afl/)
- [LibFuzzer](http://llvm.org/docs/LibFuzzer.html)
- [KLEE](http://klee.github.io/) - 可以用来模糊个别功能

#### Continuous Fuzzing（连续模糊化处理）

Continuous fuzzing tools exist to run fuzz tests for you with each commit.

存在连续模糊测试工具，可以在每次提交时为您运行模糊测试

- [Fuzzit](https://fuzzit.dev/)

### Mutation Testers

这些工具获取在单元测试运行期间执行的代码，并修改执行的代码。如果测试继续通过，但存在一个突变，那么套件中很可能存在一个有缺陷的测试

- [Dextool Mutate](https://github.com/joakim-brannstrom/dextool/tree/master/plugin/mutate)
- [MuCPP](https://neptuno.uca.es/redmine/projects/mucpp-mutation-tool/wiki)
- [mull](https://github.com/mull-project/mull)
- [CCMutator](https://github.com/markus-kusano/CCMutator)

### Control Flow Guard

MSVC's [Control Flow Guard](https://msdn.microsoft.com/en-us/library/windows/desktop/mt637065(v=vs.85).aspx?f=255&MSPPError=-2147217396) 增加高性能运行时安全检查

### Checked STL Implementations

- `_GLIBCXX_DEBUG` with GCC's implementation libstdc++ implementation. See [Krister's blog article](https://kristerw.blogspot.se/2018/03/detecting-incorrect-c-stl-usage.html).

libstdc++的实现

## Ignoring Warnings（忽略警告）

如果团队一致认为编译器或分析器对不正确或不可避免的事情发出了警告，那么团队将把特定的错误禁用到尽可能本地化的代码部分

在对一段代码禁用该警告后，请确保重新启用该警告。您不希望禁用的警告泄露到其他代码中 You do not want your disabled warnings to [leak into other code](http://www.forwardscattering.org/post/48).

## Testing（测试）

上面提到的CMake有一个用于执行测试的内置框架。确保您所使用的构建系统能够执行内置的测试

[Google Test](https://github.com/google/googletest), [Catch](https://github.com/philsquared/Catch), [CppUTest](https://github.com/cpputest/cpputest), [Boost.Test](http://www.boost.org/doc/libs/release/libs/test/) 

为了进一步帮助执行测试，请考虑像GTest、Catch、CppUTest或Boost这样的库。测试，以帮助您组织测试

### Unit Tests（单元测试）

单元测试是针对小的代码块，可以独立测试的独立功能

### Integration Tests（一体化测试）

应该对提交的每个特性或bug修复都启用测试。另请参见代码覆盖率（[Code Coverage Analysis](https://github.com/cpp-best-practices/cppbestpractices/blob/master/02-Use_the_Tools_Available.md#code-coverage-analysis)）分析。这些测试比单元测试级别更高。它们仍然应该被限制在单个特性的范围内

### Negative Testing（逆向测试、错误测试）

不要忘记确保您的错误处理正在进行测试，并且能够正常工作。如果您的目标是100%的代码覆盖率，这将变得非常明显

## Debugging（调试）

### GDB

[GDB](https://www.gnu.org/software/gdb/) - GNU调试器，功能强大且被广泛使用。大多数IDE实现一个接口来使用它

### rr

[rr](http://rr-project.org/) 是一个免费的(开源的)反向调试器，支持C++



## Other Tools（其他工具）

### Lizard

[Lizard](http://www.lizard.ws/) 为针对C++代码库运行复杂性分析提供了一个非常简单的接口。

### Metrix++

[Metrix++](http://metrixplusplus.sourceforge.net/)  可以识别并报告代码中最复杂的部分。减少复杂的代码可以帮助您和编译器更好地理解和优化它

### ABI Compliance Checker

[ABI Compliance Checker](http://ispras.linuxbase.org/index.php/ABI_compliance_checker) 可以分析两个库版本，并生成关于API和C++ ABI变化的详细兼容性报告。这可以帮助库开发人员发现无意的破坏性更改，以确保向后兼容性。

### CNCC

[Customizable Naming Convention Checker](https://github.com/mapbox/cncc) 可以报告代码中不遵循特定命名约定的标识符。

### ClangFormat

[ClangFormat](http://clang.llvm.org/docs/ClangFormat.html) 是否可以自动检查和纠正代码格式以匹配组织约定 [Multipart series](https://engineering.mongodb.com/post/succeeding-with-clangformat-part-1-pitfalls-and-planning/) 利用clang-format。

### SourceMeter

[SourceMeter](https://www.sourcemeter.com/) 提供了一个免费的版本，为您的代码提供了许多不同的指标，也可以调用cppcheck。

### Bloaty McBloatface

[Bloaty McBloatface](https://github.com/google/bloaty) 二进制大小分析器/分析器是否适用于unix类平台

### pahole

[pahole](https://linux.die.net/man/1/pahole) 生成关于已编译代码中数据结构和类打包中的漏洞的数据。它还可以确定结构的大小以及它们如何适应系统的缓存线

### BinSkim

[BinSkim](https://github.com/Microsoft/binskim) 是一个二进制静态分析工具，为Windows可执行文件和*nix ELF二进制格式提供安全和正确的结果