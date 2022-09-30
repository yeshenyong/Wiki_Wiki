# Linux CPU 信息



参考

https://www.jianshu.com/p/aea52895de5e



### lscpu

```shell
lscpu
Architecture:        x86_64
CPU op-mode(s):      32-bit, 64-bit
Byte Order:          Little Endian
CPU(s):              1
On-line CPU(s) list: 0
Thread(s) per core:  1
Core(s) per socket:  1
Socket(s):           1
NUMA node(s):        1
Vendor ID:           GenuineIntel
CPU family:          6
Model:               79
Model name:          Intel(R) Xeon(R) CPU E5-2682 v4 @ 2.50GHz
Stepping:            1
CPU MHz:             2500.044
BogoMIPS:            5000.08
Hypervisor vendor:   KVM
Virtualization type: full
L1d cache:           32K
L1i cache:           32K
L2 cache:            256K
L3 cache:            40960K
NUMA node0 CPU(s):   0
Flags:               fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 ss ht syscall nx pdpe1gb rdtscp lm constant_tsc rep_good nopl nonstop_tsc cpuid tsc_known_freq pni pclmulqdq ssse3 fma cx16 pcid sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand hypervisor lahf_lm abm 3dnowprefetch invpcid_single pti ibrs ibpb stibp fsgsbase tsc_adjust bmi1 hle avx2 smep bmi2 erms invpcid rtm rdseed adx smap xsaveopt arat
```



### /proc/stat

在Linux系统中，可以用/proc/stat文件来计算cpu的利用率。这个文件包含了所有CPU活动的信息，该文件中的所有值都是从系统启动开始累计到当前时刻

系统启动以来的很多系统和内核的统计信息，平时大家比较关心的比如包括CPU运行情况、中断情况、启动时间、上线文切换次数、运行中的进程等信息都在其中



```shell
cat /proc/stat

cpu  33430252 90 43037932 1709022339 631580 0 21634 0 0 0
cpu0 33430252 90 43037932 1709022339 631580 0 21634 0 0 0
intr 3190384096 ...
ctxt 123867500194
btime 1645756440
processes 673923
procs_running 6
procs_blocked 0
softirq 2253290314 0 1265062578 48 14528945 0 0 24 0 0 973698719
```

```sh
name   user  nice   system      idle      iowait  irrq  softirq  steal guest guest_nice 
cpu    60382   1     80032     198934063   2349     0     109      0     0       0
cpu0   2405    0     2084      4140924     682      0     6        0     0       0
...  # 此处较多冗余信息，简化之
cpu47  200     0     134       4147222     10       0     0        0     0       0
```

```sh
intr 33622492 64 ... 0 0 0 0 # 此处较多冗余信息，简化之
# 这行给出中断的信息，第一个为自系统启动以来，发生的所有的中断的次数
# 然后每个数对应一个特定的中断自系统启动以来所发生的次数。

ctxt 68533835       #自系统启动以来CPU发生的上下文交换的次数

btime 1528905555    #系统启动到现在的时间，单位为秒(s)

processes 318904    #自系统启动以来所创建的任务的个数目

procs_running 1     #当前运行队列的任务的数目

procs_blocked 0     #当前被阻塞的任务的数目

softirq 16567860 0 3850777 8555 5448802 116727 0 1 3577293 1290 3564415
# 此行显示所有CPU的softirq总数
# 第一列是所有软件和每个软件的总数
# 后面的列是特定softirq的总数
```

字段含义

|  cpu指标   | 含义           | 时间单位 | 备注                                                         |
| :--------: | -------------- | -------- | ------------------------------------------------------------ |
|    user    | 用户态时间     | jiffies  | 一般/高优先级，仅统计nice<=0                                 |
|    nice    | nice用户态时间 | jiffies  | 低优先级，仅统计nice>0                                       |
|   system   | 内核态时间     | jiffies  |                                                              |
|    idle    | 空闲时间       | jiffies  | 不包含IO等待时间                                             |
|   iowait   | I/O等待时间    | jiffies  | 硬盘IO等待时间                                               |
|    irq     | 硬中断时间     | jiffies  |                                                              |
|  softirq   | 软中断时间     | jiffies  |                                                              |
|   steal    | 被盗时间       | jiffies  | 虚拟化环境中运行其他操作系统上花费的时间（since Linux 2.6.11） |
|   guest    | 来宾时间       | jiffies  | 操作系统运行虚拟CPU花费的时间(since Linux 2.6.24)            |
| guest_nice | nice来宾时间   | jiffies  | 运行一个带nice值的guest花费的时间(since Linux 2.6.33)        |

1、1 jiffies = 0.01s = 10ms
 2、常用计算等式：CPU时间 = user + system + nice + idle + iowait + irq + softirq
 3、man手册中iowait有单独说明，iowait时间是不可靠值，具体原因如下：
 1）CPU不会等待I/O执行完成，而iowait是等待I/O完成的时间。
 当CPU进入idle状态，很可能会调度另一个task执行，所以iowait计算时间偏小；
 2）多核CPU中，iowait的计算并非某一个核，因此计算每一个cpu的iowait非常困难
 3）这个值在某些情况下会减少



CPU时间=user+system+nice+idle+iowait+irq+softirq

那么CPU利用率可以使用以下两个方法。先取两个采样点，然后计算其差值：
cpu usage=(idle2-idle1)/(cpu2-cpu1)*100

cpu usage=[(user_2 +sys_2+nice_2) - (user_1 + sys_1+nice_1)]/(total_2 - total_1)*100

