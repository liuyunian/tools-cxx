# 线程池
<font color = red size = 4>自行实现的线程池代码</font>   

## 特点
* 采用POSIX线程，因此不具备跨平台性
* 静态线程池，线程池中线程数据在创建时指定，不可动态扩容
* 任务队列不具备优先级，采用FIFO方式处理

## 使用
| 函数接口 | 功能 | 参数 |
| --- | --- | :---: |
| create | 创建线程池中线程 | size_t -- 指定线程数 |
| call | 向任务队列中添加任务 | const std::function<void()>& -- 要执行的任务 |
| stop | 线程池停止工作 | void |  

具体如何调用参考[threadpool_test.cpp](/examples/threadpool/threadpool_test.cpp)  
注意：使用时需要链接***pthread***库

