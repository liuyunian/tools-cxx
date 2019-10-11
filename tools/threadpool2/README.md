# 线程二
<font color = red size = 4>代码取自[progschj/ThreadPool](https://github.com/progschj/ThreadPool)</font>   

## 特点
* 采用C++11中thread类，具备跨平台性
* 可以获取任务执行之后的结果 -- std::future

## 使用
| 函数接口 | 功能 | 返回值 |
| --- | --- | :---: |
| enqueue | 向任务队列中添加任务 | std::future |

具体如何调用参考[threadpool2_test.cpp](/examples/threadpool/threadpool2_test.cpp)  
注意：使用时需要链接***pthread***库

