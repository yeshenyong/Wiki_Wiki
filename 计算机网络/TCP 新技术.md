# TCP 新技术



目前全球互联网数据流量 90% 以上通过 TCP 传输，通过 UDP 传输的不足10%，而且 TCP 所占份额还在继续扩大。但这一设计于二十多年前的传输协议已经越来越不适应飞速发展的高速网络环境和新型应用的要求。网络路径上存在一定的丢包和延时的情况下，TCP 连接的吞吐显著下滑，导致应用响应缓慢甚至无法使用。

　　以拥塞判断及处理方式来区分，到目前为止，TCP 加速技术已经历了两代技术演进：

　　• Loss-based (基于丢包的拥塞判断及处理)– 以丢包来判断拥塞并调整传输速率

　　• Delay-based(基于延迟的拥塞判断及处理)– 以往返延迟(Round Trip Time , RTT)变化来判断拥塞并调整传输速率

　　不论是 Loss-based 还是 Delay-based TCP 加速技术都采用静态算法,即基于对互联网流量模型的假设前提采用固定的拥塞判断及恢复机制。但网络的发展[趋势](http://product.yesky.com/feedsystem/trendnet/)是流量特征越来越复杂并难以预测。因此，Loss-based 和 Delay-based TCP 加速技术常常只在其前提假设成立的特定网络场景下有效，并且随着传输的进行，网络路径特征发生变化，效果也会起伏不定，有时甚至出现反效果。



##  **Learning-based TCP** 

Learning-based(基于学习的拥塞判断及处理)– 通过对路径上传输历史的学习动态判断拥塞并调整传输速率。

　　华夏创新研究设计的新一代 Learning-based TCP加速技术采用网络路径特征自学习的动态算法，基于每一个TCP 连接实时观察、分析网络特征，根据学习到的网络特征随时调整算法来更准确地判断拥塞程度、更及时地判断丢包，从而更恰当的进行拥塞处理并更快速地进行丢包恢复。这一设计从原理上克服了静态算法无法适应网络路径特征变化的问题，保证了在各种不同网络环境及频繁变化的网络延迟、丢包特征下加速效果的持续有效性。



## **华夏创新 ZetaTCP® 单边加速技术**



ZetaTCP® 作为最新一代 Learning-based TCP 加速技术，从设计原理上解决了 Lossbased和 Delay-based 的核心问题，通过针对每个 TCP 连接传输动态学习来掌握其网络路径的各种特征，能够适应各种网络情况及网络特征的变化，达到更稳定及快速的传输效果，最终提升用户的体验。

　　下图比较了 Loss-based、Delay-based 和Learning-based TCP 加速的效果：

![ ZetaTCP单边加速 新一代TCP加速技术诞生](http://image.tianjimedia.com/uploadImages/2014/343/56/9O02VV07S931_500.jpg)

​	从[测试](http://product.yesky.com/ceshi/)中可以明显看到，Learning-based ZetaTCP® 加速在三者中能提供最高的传输速率，特别是在延迟和丢包较高的时候，Learning-based ZetaTCP® 相较于前两代加速技术显示出了非常明显的优势。

　　学术界和产业界从部署方式上将TCP加速分为**双边 TCP 加速和单边 TCP 加速**两类。**双边 TCP 加速将 TCP 转化为私有协议，需要在传输的两端部署同样的技术**，应用范围仅限于**公司跨分支机构的数据传输**。而**单边 TCP 加速方式和标准 TCP 协议的完全兼容**，只需要在 **TCP 连接的一端部署**，具有非常广泛的适用性。ZetaTCP® 即是这样一种单边 TCP 加速技术。





　**华夏创新ZetaTCP® 单边加速技术应用范围更加广泛**

　　在架构设计上，ZetaTCP® 不仅和标准 TCP 全兼容，而且并不替代标准 TCP，只是作为一个传输增强模块。当 ZetaTCP® 以硬件产品形式部署时，内嵌 ZetaTCP®的设备以透明[网桥](http://product.yesky.com/wirelessbridge/)的形式接入需要加速的网络路径上即可，不需要对现有网络设备做任何配置改动。当ZetaTCP® 以软件形式直接安装在应用[服务器](http://product.yesky.com/service/)、[智能手机](http://product.yesky.com/mobilephone/)或[笔记本电脑](http://product.yesky.com/ultrabook/)等用户终端上时，ZetaTCP® 加速引擎作为一个内核模块工作在[操作系统](http://product.yesky.com/os/) TCP 协议栈和网络接口驱动模块之间，实现了对应用及网络拓扑的全透明部署。华夏创新的 ZetaTCP® 单边加速技术是最佳选择。