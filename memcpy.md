# memcpy

## 按字节(Byte)拷贝实现的memcpy

    void *my_memcpy_byte(void *dst, const void *src, int n)
    {
        if (dst == NULL || src == NULL || n <= 0)
            return NULL;
    	char * pdst = (char *)dst;
    	char * psrc = (char *)src;
    
        if (pdst > psrc && pdst < psrc + n)
        {
            pdst = pdst + n - 1;
            psrc = psrc + n - 1;
            while (n--)
                *pdst-- = *psrc--;
        }
        else
        {
            while (n--)
                *pdst++ = *psrc++;
        }
        return dst;
    } 
## **按4字节拷贝实现的memcpy**

高性能的**memcpy**与很多因数相关，与平台，处理器，编译器，具体拷贝情形等相关。

VS2017中对C库的memcpy进行优化，glibc对memcpy也有优化



