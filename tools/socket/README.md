# Socket
<font color = red size = 4>代码参考自muduo网络库，对字节序转化函数、网络地址、Socket和Socket操作函数进行了封装</font>   

## Endian -- 字节序转化
对于字节序转化函数，Unix和Linux共同拥有的是htonl(), htons(), ntohl(), ntohs()  
另外对于Linux，在glibc 2.19版本中添加了htobe16(), htole16(), be16toh(), le16toh()等函数  
为了统一，对字节序转化函数进行了如下封装：  

| 函数 | 功能 | 参数 | 返回值 |
| :---: | :---: | :---: | :---:|
| host_to_network64 | 将64位无符号整数由本机字节序转化成网络字节序 | uint64_t | uint64_t |
| host_to_network32 | 32位无符号整数 | uint32_t | uint32_t |
| host_to_network16 | 16位无符号整数 | uint16_t | uint16_t |
| network_to_host64 | 将64位无符号整数由网络字节序转化成本机字节序 | uint64_t | uint64_t |
| network_to_host32 | 32位无符号整数 | uint32_t | uint32_t |
| network_to_host16 | 16位无符号整数 | uint16_t | uint16_t |

具体如何使用参考[Endian_test.cpp](/examples/socket/Endian_test.cpp)  

## InetAddress -- 网络地址
对于Unix和Linux，网络地址有多种，常用的有：
* 通用套接字地址结构struct sockaddr
* IPv4套接字地址结构struct sockaddr_in
* IPv6套接字地址结构struct sockaddr_in6  

这里对IPv4和IPv6地址结构进行了封装，使得构建地址、地址之间的转化更简单  
具体如何使用参考[InetAddress_test.cpp](/examples/socket/InetAddress_test.cpp)

## SocketsOps -- Socket操作函数
对诸如socket(), bind(), listen()等函数进行了包装，包装的主要目的是简化错误处理  
  
| 函数 | 功能 | 返回值 |
| :---: | :---: | :---: | :---: |
| create_socket| 创建阻塞式Socket描述符 | sockfd -- int |
| create_nonblocking_socket | 创建非阻塞式Socket描述符 | sockfd -- int | 
| close | -- | void | 
| bind | -- | void |
| listen | -- | void | 
| accept(int) | 接收连接，返回阻塞式Socket描述符 | connfd -- int |
| accept(int, struct sockaddr_in6*) | 接收连接并记录客户端的地址，返回阻塞式Socket描述符 | connfd -- int |
| accept_nonblocking(int) | 返回非阻塞式的Socket描述符 | connfd -- int | 
| accept_nonblocking(int, struct sockaddr_in6*) | 记录客户端地址，返回非阻塞式的Socket描述符 | connfd -- int |
| connect | -- | void |  

## Socket & ServerSocket
Socket类采用RAII技术对socket文件描述符进行封装，使得文件描述符的生命周期与类对象的生存期关联，更方便高效的关联socket文件描述符资源  
ServerSocket类继承自Socket，是对服务器端监听Socket的封装  

这里利用封装的Socket和poll/epoll复用技术实现了三个echo回射服务器和一个客户端
* [echoClient_test.cpp](/examples/socket/echoClient_test.cpp)
* [echoServer_test.cpp](/examples/socket/echoServer_test.cpp)
* [echoServer_poll_test.cpp](/examples/socket/echoServer_poll_test.cpp)
* [echoServer_epoll_test.cpp](/examples/socket/echoServer_epoll_test.cpp)

