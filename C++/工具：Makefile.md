# Makefile







#### makefile中的$< $@ $^解释

```makefile
OBJS = ifconfig.o tftp_util.o ping.o
SRCS = $(OBJS:.o=.c)

PHONY += all
all : $(BUILT_IN_OBJ)

$(BUILT_IN_OBJ) : $(OBJS)
      $(LD) $(LDFLAGS) -r $^ -o $@

$(OBJS) : $(SRCS)
      $(CC) $(CFLAGS) -c $^

PHONY += clean
clean :
      @rm -vf *.o

.PHONY : $(PHONY)
\----------------------------------------------------------------------------------------------------------------------------------------
说明：$(BUILT_IN_OBJ) 和 $(PHONY)来自上层目录g-bios/app/Makefile，可以不用理解。
```

$@ 代表目标 上例为$(BUILT_IN_OBJ)
$^ 代表所有的依赖对象 上例第一个$^为$(OBJS)，即 ifconfig.o tftp_util.o ping.o，上例第二个$^为$(SRCS)，即$(OBJS:.o=.c) (Makefile解释后为: ifconfig.c tftp_util.c ping.c)
$< 代表第一个依赖对象 前面例子修改如下:
$(BUILT_IN_OBJ) : $(OBJS)
      $(LD) $(LDFLAGS) -r $< -o $@
则$<表示为ifconfig.o