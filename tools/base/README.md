# 基础库
基础库中封装了一些常用工具类：标记类、时间戳...  
<font color = red size = 4> 代码学习自muduo基础库，目前只选取对该项目有用的部分 </font>  

## copyable & noncopyable
标记类，作为基类存在，用于表征子类的实例是否可以拷贝

## CountDownLatch
并发编程高级构件，用于线程间的同步

## CurrentThread
CurrentThread名称空间，定义了与当前线程相关信息的函数，比如：获取当前线程的真实ID ...

## ProcessInfo
ProcessInfo名称空间，定义了获取当前进程相关信息的函数，比如：获取当前进程的PID ...

## Singleton
单例模板类，用于将其他类转换成单例类使用

## Timestamp
时间戳类，获取当前时间、格式化时间戳 ...