# deque的数据结构

deque除了维护一个先前说过的指向map的指针外，也维护start，finish两个迭代器，分别指向第一缓冲区的第一个元素和最后缓冲区的最后一个元素（的下一个位置）。此外，它当然也必须记住目前的map大小，因为一旦map所提供的节点不足，就必须重新配置更大的一块map。

```c++
  template<class T, class Alloc = alloc, size_t BufSiz = 0>  
  class deque{  
  public :  
      typedef T   value_type ;  
      typedef value_type* pointer ;  
      typedef size_t  size_type ;  
  public :  
      typedef __deque_iterator<T,T&,T*,BufSiz>  iterator ;  
  protected :  
      //元素的指针的指针(pointer of pointer of T)  
      typedef pointer*    map_pointer ;  
  protected:  
      iterator    start ; //表现第一节点  
      iterator    finish ; //表现最后一个节点  
      map_pointer map ; //指向map,map是块连续空间，其每个元素都是个指针，指向一个节点(缓冲区)  
      size_type   map_size ; //map内有多少指针  
      ...  
  } ; 
```

