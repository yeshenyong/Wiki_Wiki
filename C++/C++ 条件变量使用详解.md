# C++ 条件变量使用详解

转自：https://blog.csdn.net/c_base_jin/article/details/89741247

在C++11中，我们可以使用条件变量（condition_variable）实现多个线程间的同步操作；当条件不满足时，相关线程被一直阻塞，直到某种条件出现，这些线程才会被唤醒。

其主要成员函数如下:

![图片](https://mmbiz.qpic.cn/mmbiz_png/pldYwMfYJpjcN55ueUzLnTOrTiaoZEj3eJwVRTsYaQq3sMkIKLmPcmZfOUdRUSTh4mQMAJoHnC7gGtXDTD9W3FQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

例子：（一例胜千言）

```cpp
std::mutex g_cvMutex;
std::condition_variable g_cv;

//缓存区
std::deque<int> g_data_deque;
//缓存区最大数目
const int  MAX_NUM = 30;
//数据
int g_next_index = 0;

//生产者，消费者线程个数
const int PRODUCER_THREAD_NUM  = 3;
const int CONSUMER_THREAD_NUM = 3;

void  producer_thread(int thread_id)
{
  while (true)
  {
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      //加锁
      std::unique_lock <std::mutex> lk(g_cvMutex);
      //当队列未满时，继续添加数据
      g_cv.wait(lk, [](){ return g_data_deque.size() <= MAX_NUM; });
      g_next_index++;
      g_data_deque.push_back(g_next_index);
      std::cout << "producer_thread: " << thread_id << " producer data: " << g_next_index;
      std::cout << " queue size: " << g_data_deque.size() << std::endl;
      //唤醒其他线程 
      g_cv.notify_all();
      //自动释放锁
  }
}

void  consumer_thread(int thread_id)
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(550));
        //加锁
        std::unique_lock <std::mutex> lk(g_cvMutex);
        //检测条件是否达成
        g_cv.wait( lk,   []{ return !g_data_deque.empty(); });
        //互斥操作，消息数据
        int data = g_data_deque.front();
        g_data_deque.pop_front();
        std::cout << "\tconsumer_thread: " << thread_id << " consumer data: ";
        std::cout << data << " deque size: " << g_data_deque.size() << std::endl;
        //唤醒其他线程
        g_cv.notify_all();
        //自动释放锁
    }
}


int main()
{
    std::thread arrRroducerThread[PRODUCER_THREAD_NUM];
    std::thread arrConsumerThread[CONSUMER_THREAD_NUM];

    for (int i = 0; i < PRODUCER_THREAD_NUM; i++)
    {
        arrRroducerThread[i] = std::thread(producer_thread, i);
    }

    for (int i = 0; i < CONSUMER_THREAD_NUM; i++)
    {
        arrConsumerThread[i] = std::thread(consumer_thread, i);
    }

    for (int i = 0; i < PRODUCER_THREAD_NUM; i++)
    {
        arrRroducerThread[i].join();
    }

    for (int i = 0; i < CONSUMER_THREAD_NUM; i++)
    {
        arrConsumerThread[i].join();
    }
    
 return 0;
}
```





