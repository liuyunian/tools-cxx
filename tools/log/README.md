# 日志库一
<font color = red size = 4>参考[错误处理](../error/README.md)代码自行整理日志库</font>   

## 特点
* C printf(fmt, ...)风格 -- LOG_INFO("hello world, %d", 10);
* 日志函数是分散的 -- LOG_WARN()、LOG_INFO()、LOG_DEBUG()...
* 5种日志级别 -- FATAL、ERROR、WARN、INFO、DEBUG
* 两种设置默认日志级别的方式 -- 环境变量和调用log_setlevel()  

## 使用

| 宏定义 | 本质函数 | 日志级别 | 是否输出error | 结束语句 |
| --- | --- | --- | :---: | :---: |
| LOG_FATAL | log_fatal | FATAL | 否 | abort() |
| LOG_SYSFATAL | log_sysfatal | FATAL | 是 | abort() |
| LOG_ERR | log_err | ERROR | 否 | return |
| LOG_SYSERR | log_syserr | ERROR | 是 | return |
| LOG_WARN | log_warn | WARN | 否 | 无 |
| LOG_INFO | log_info | INFO | 否 | 无 |
| LOG_DEBUG | log_debug | DEBUG | 否 | 无 |  

函数如何调用参考：[log_test.cpp](/examples/log_test.cpp)  
编译时链接***libtools_log.a***静态库

## 日志输出
日志格式：时间 线程 日志级别: 正文 (errno: strerror(errno)) - 源文件名:行号
```
20190915 14:10:37.189308 5043 [ERROR]: syserr, ef (2: No such file or directory) - test.cpp:13
```
日志输出位置  
日志信息默认是输出到标准错误stderr  
如果需要将日志信息输出到文件，需要使用**重定向**
```
命令 &>> 文件
```