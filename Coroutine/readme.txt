一、概念
协程是一个无优先级的子程序调度组件，允许子程序在特点的地方挂起恢复。
线程包含于进程，协程包含于线程。只要内存足够，一个线程中可以有任意多个协程，但某一时刻只能有一个协程在运行，多个协程分享该线程分配到的计算机资源。
二、协程 vs 事件驱动
以nginx为代表的事件驱动的异步server正在横扫天下，那么事件驱动模型会是server端模型的终点吗？
事件驱动编程的架构是预先设计一个事件循环，这个事件循环程序不断地检查目前要处理的信息，根据要处理的信息运行一个触发函数。其中这个外部信息可能来自一个目录夹中的文件，可能来自键盘或鼠标的动作，或者是一个时间事件。这个触发函数，可以是系统默认的也可以是用户注册的回调函数。
事件驱动程序设计着重于弹性以及异步化上面。许多GUI框架（如windows的MFC，Android的GUI框架），Zookeeper的Watcher等都使用了事件驱动机制。
基于事件驱动的编程是单线程思维，其特点是异步+回调。
协程也是单线程，但是它能让原来要使用异步+回调方式写的非人类代码,可以用看似同步的方式写出来。它是实现推拉互动的所谓非抢占式协作的关键。
三、协程的优缺点
1、优点：
跨平台
跨体系架构
无需线程上下文切换的开销
无需原子操作锁定及同步的开销
方便切换控制流，简化编程模型
高并发+高扩展性+低成本：一个CPU支持上万的协程都不是问题。所以很适合用于高并发处理。
2、缺点：
无法利用多核资源：协程的本质是个单线程,它不能同时将单个CPU 的多个核用上,协程需要和进程配合才能运行在多CPU上.当然我们日常所编写的绝大部分应用都没有这个必要，除非是cpu密集型应用。
进行阻塞（Blocking）操作（如IO时）会阻塞掉整个程序：这一点和事件驱动一样，可以使用异步IO操作来解决
四、协程库的实现
1、ucontext-人人都可以实现的简单协程库
https://blog.csdn.net/qq910894904/article/details/41911175
 项目地址：https://github.com/Winnerhust/uthread
2、 一种协程的 C/C++ 实现
http://www.cnblogs.com/Pony279/p/3903048.html
​项目地址：https://github.com/roxma/cpp_learn/tree/master/cpp/linux_programming/coroutine
五、开源的协程库
1、libco
libco 是腾讯开源的一个C++协程库，作为微信后台的基础库，经受住了实际的检验。
项目地址：https://github.com/Tencent/libco
2、state threads library(以下简称st)
协程库state threads library(以下简称st)是一个基于setjmp/longjmp实现的C语言版用户线程库或协程库（user level thread）。
项目地址：https://github.com/tangyibo/StateThreads
协程例子 http://www.csl.mtu.edu/cs4411.ck/www/NOTES/non-local-goto/coroutine.html，
3、Protothreads
Protothreads 是一种针对 C 语言封装后的轻量级的协程（宏）函数库，为 C 语言模拟了一种无堆栈的轻量线程环境，能够实现模拟线 程的条件阻塞、信号量操作等操作系统中特有的机制，从而使程序实现多线程操作。
项目地址：https://github.com/tangyibo/Protothreads
4、基于ucontext实现的协程
ucontext相关接口，主要有如下四个：
getcontext：获取当前context
setcontext：切换到指定context
makecontext: 用于将一个新函数和堆栈，绑定到指定context中
swapcontext：保存当前context，并且切换到指定context
文档地址：https://linux.die.net/man/3/swapcontext
（1）libtask库，基于项目地址：https://github.com/tangyibo/libtask
（2）云风的coroutine，项目地址：https://github.com/tangyibo/coroutine（源地址：https://github.com/cloudwu/coroutine）
5、基于setjmp/longjmp接口实现的协程
（1）上述的state threads library(以下简称st)协程库
（2）利用pthread_create函数实现的协程：http://www.cnblogs.com/Pony279/p/3903048.html
6、WindowsAPI的协程
  windows 纤程(fiber) 实现的协程 ，项目地址：https://blog.csdn.net/woshiyuanlei/article/details/54426789
7、Boost.Context协程库
Boost库中的协程支持两种方式：一种是封装了Boost.Coroutine的spawn，是一个stackful类型的协程；一种是asio作者写出的stackless协程。
介绍：https://www.cnblogs.com/my_life/articles/5942395.html
8、此外还有tbox的协程实现（https://github.com/tboox/tbox）等。
