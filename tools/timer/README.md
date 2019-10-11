# 定时器
<font color = red size = 4>代码学习自[W2Y博客 -- C++定时器CTimer的实现](https://blog.csdn.net/lizhijian21/article/details/83417747)</font>   

## 关于定时器的思考
### 需求
根据目前的需求，我对定时器有如下要求：  
* 一次性定时。定时器计时过程不影响程序的继续执行，到时间之后，调用一个回调函数
* 循环定时。循环定时执行某项任务，比如每个2秒执行一个函数，并且可以取消执行
* 多个定时任务可以共存

### 实现方案
定时器的实现方案有很多，更加我自己的阅历，总结如下：
* sleep(3)、usleep(3)、alarm(2) + SIGALRM信号。参考[alarm_test.cpp](/examples/alarm_test.cpp)
* * 这种实现方式的问题在于：定时任务不能共存，并且在多线程程序中处理信号是很麻烦的事，应当避免
* 多线程 + 休眠。这也是本定时器的实现方式，每个定时器类对象持有一个线程对象，线程休眠指定的定时时间，之后调用传入的回调函数
* * 这种实现方式的问题在于：开销大，定时不精确
* Reactor + timerfd

## 使用
| 函数接口 | 功能 | 返回值 |
| --- | --- | :---: |
| async_once | 一次性定时任务 | bool |
| async_loop | 循环定时任务 | bool |
| cancel_timer | 取消定时任务 | void |  

具体如何调用参考[timer_test.cpp](/examples/timer/timer_test.cpp)  
注意：使用时需要链接***pthread***库

