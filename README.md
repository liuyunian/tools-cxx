# tools-cxx
C++工具库 -- 造轮子使人快乐

## 编译
### 环境 & 依赖  
1. 测试OS：ubuntu18.04
2. 测试编译器：gcc version 7.3.0
3. 测试构建工具：cmake 3.10.2

### 构建  
1. 构建：make  
2. 安装：make install   
3. 卸载：make uninstall  
4. 清理：make clean

## 示例  
示例程序位于examples目录下  
1. 构建：make  
2. 执行：构建生成的可执行程序位于examples/build/xxx目录下  
3. 清理：make clean

## 使用介绍
* [基础库](/tools/base/README.md)
* [解析配置文件](/tools/config/README.md)  
* [协程](/tools/coroutine/README.md)
* [多线程日志](/tools/log/README.md)
* [IO复用](/tools/poller/README.md)
* [Socket](/tools/socket/README.md)
* [线程池](/tools/threadpool/README.md)
* [定时器](/tools/timer/README.md)