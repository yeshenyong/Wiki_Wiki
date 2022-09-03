# Linux 信号及其用途

摘自：https://en.wikipedia.org/wiki/Signal_(IPC)#cite_note-0

|  Signal   | Portable number |    Default action     |                         Description                          |
| :-------: | :-------------: | :-------------------: | :----------------------------------------------------------: |
|  SIGABRT  |        6        | Terminate (core dump) |               Process abort signal（进程退出）               |
|  SIGALRM  |       14        |       Terminate       |                  Alarm clock（计时器到时）                   |
|  SIGBUS   |        —        | Terminate (core dump) |      Access to an undefined portion of a memory object       |
|  SIGCHLD  |        —        |        Ignore         |       Child process terminated, stopped, or continued        |
|  SIGCONT  |        —        |       Continue        |                Continue executing, if stopped                |
|  SIGFPE   |        8        | Terminate (core dump) | Erroneous arithmetic operation（浮点指针异常，错误的算数操作） |
|  SIGHUP   |        1        |       Terminate       |                        Hangup（挂起）                        |
|  SIGILL   |        4        | Terminate (core dump) |               Illegal instruction（非法指令）                |
|  SIGINT   |        2        |       Terminate       |                  Terminal interrupt signal                   |
|  SIGKILL  |        9        |       Terminate       |              Kill (cannot be caught or ignored)              |
|  SIGPIPE  |       13        |       Terminate       |            Write on a pipe with no one to read it            |
|  SIGPOLL  |        —        |       Terminate       |                        Pollable event                        |
|  SIGPROF  |        —        |       Terminate       |                   Profiling timer expired                    |
|  SIGQUIT  |        3        | Terminate (core dump) |                     Terminal quit signal                     |
|  SIGSEGV  |       11        | Terminate (core dump) |           Invalid memory reference（非法内存访问）           |
|  SIGSTOP  |        —        |         Stop          |         Stop executing (cannot be caught or ignored)         |
|  SIGSYS   |        —        | Terminate (core dump) |                       Bad system call                        |
|  SIGTERM  |       15        |       Terminate       |            Termination signal（终止（要求终止））            |
|  SIGTRAP  |        5        | Terminate (core dump) |                    Trace/breakpoint trap                     |
|  SIGTSTP  |        —        |         Stop          |                     Terminal stop signal                     |
|  SIGTTIN  |        —        |         Stop          |              Background process attempting read              |
|  SIGTTOU  |        —        |         Stop          |             Background process attempting write              |
|  SIGUSR1  |        —        |       Terminate       |                    User-defined signal 1                     |
|  SIGUSR2  |        —        |       Terminate       |                    User-defined signal 2                     |
|  SIGURG   |        —        |        Ignore         | [Out-of-band data](https://en.wikipedia.org/wiki/Out-of-band_data) is available at a socket |
| SIGVTALRM |        —        |       Terminate       |                    Virtual timer expired                     |
|  SIGXCPU  |        —        | Terminate (core dump) |                   CPU time limit exceeded                    |
|  SIGXFSZ  |        —        | Terminate (core dump) |                   File size limit exceeded                   |
| SIGWINCH  |        —        |        Ignore         |                 Terminal window size changed                 |

