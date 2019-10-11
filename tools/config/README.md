# 解析配置文件
<font color = red size = 4>自行封装的解析配置文件的库，功能有限</font>   

## 关于程序交互
程序运行总是需要一些输入数据，执行输入数据的方式有多种：  
* 在程序中以常量方式指定 -- 数据改变时需要重新编译程序
* 程序执行时以命令行参数方式指定 -- 输入数据有限，数据改变时需要重新运行程序
* 以配置文件方式指定输入数据 -- 数据改变时，重载配置文件，可以不要停止程序的运行

## 使用
核心接口有两个：
* load(const char *) -- 读取参数（配置文件的绝对路径或者相对路径）指定的配置文件并解析保存
* get<Type>(const std::string &) -- 模板函数，获取指定配置项的内容

具体如何调用参考[config_test.cpp](/examples/config/config_test.cpp)  
其中ConfigFile类一般在整个程序中只存在一个实例，所以可以借助[基础库](../base/README.md)中的Singleton类将ConfigFile转换成一个单例类使用  
具体如何使用参考[config_Singleton_test.cpp](/examples/config/config_Singleton_test.cpp) 

