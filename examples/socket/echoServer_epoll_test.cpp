#include <vector>

#include <sys/epoll.h>  // epoll相关
#include <errno.h>      // errno
#include <string.h>     // memset
#include <signal.h>     // signal
#include <fcntl.h>      // open

#include <tools/log/log.h>
#include <tools/socket/Socket.h>
#include <tools/socket/SocketsOps.h>
#include <tools/socket/InetAddress.h>
#include <tools/socket/ServerSocket.h>

#define LISTEN_PORT 9000
#define BUFFER_SZ 1024

int main(){
    signal(SIGPIPE, SIG_IGN);

    int idlefd = open("/dev/null", O_RDONLY | O_CLOEXEC);                               // 预留文件描述符

    ServerSocket ss(sockets::create_nonblocking_socket(AF_INET));

    InetAddress addr(LISTEN_PORT);
    ss.set_reuseAddr(true);
    ss.bind(addr);

    int epfd = epoll_create1(EPOLL_CLOEXEC);
    if(epfd < 0){
        LOG_SYSFATAL("创建epoll对象失败");
    }

    struct epoll_event event;
    event.events = EPOLLIN;                                                             // 关注监听套接字的读事件，LT工作模式（默认）
    event.data.fd = ss.get_sockfd();
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, event.data.fd, &event);
    if(ret < 0){
        LOG_SYSFATAL("调用epoll_ctl()函数向epoll对象添加监听套接字的读事件失败");
    }

    ss.listen();
    LOG_INFO("服务器监听中...");

    Socket* connSocket;
    int nready;                                                                         // 记录epoll函数返回的事件数
    char buf[BUFFER_SZ] = {0};                                                          // 应用层的接收和发送缓存
    ssize_t len = 0;                                                                    // 记录recv()函数的返回值
    std::vector<struct epoll_event> reventVec(16);                                      // 保存epoll_wait()函数返回的事件，初始大小设置为16，之后可变长
    for(;;){
        nready = epoll_wait(epfd, reventVec.data(), reventVec.size(), -1);
        if(nready <= 0){
            if(errno == EINTR){
                continue;
            }

            LOG_SYSERR("epoll_wait()函数执行错误");
            continue;
        }

        if(nready == reventVec.size()){
            reventVec.resize(reventVec.size() * 2);
        }

        for(auto & revent : reventVec){
            if(revent.events & EPOLLIN){                                                // 发生了可读事件
                if(revent.data.fd == ss.get_sockfd()){
                    connSocket = ss.accept_nonblocking(nullptr);
                    if(connSocket == nullptr){
                        if(errno == EMFILE){                                            // 进程描述符已达到上限
                            sockets::close(idlefd);                                     // 关闭预留描述符，进程有了一个空闲描述符
                            connSocket = ss.accept_nonblocking(nullptr);                // 此时可以正确的接受连接
                            delete connSocket;                                          // 服务器端优雅的关闭连接
                            
                            idlefd = open("/dev/null", O_RDONLY | O_CLOEXEC);           // 再次预留文件描述符
                            continue;
                        }

                        LOG_SYSERR("accept()函数执行出错");
                        continue;
                    }

                    event.data.ptr = static_cast<void*>(connSocket);
                    event.events = EPOLLIN;                                             // 关注连接套接字的读事件，LT工作模式
                    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, connSocket->get_sockfd(), &event);
                    if(ret < 0){
                        LOG_SYSERR("调用epoll_ctl()函数想epoll对象添加连接套接字的读事件失败");
                        continue;
                    }
                }
                else{
                    Socket* connSocket = static_cast<Socket*>(revent.data.ptr);
                    len = sockets::read(connSocket->get_sockfd(), buf, BUFFER_SZ);
                    if(len < 0){
                        LOG_SYSERR("调用recv()接收数据失败");
                    }
                    else if(len == 0){
                        LOG_INFO("客户端断开连接");
                        
                        delete connSocket;
                        continue;
                    }

                    sockets::write(connSocket->get_sockfd(), buf, strlen(buf));
                    memset(buf, 0, BUFFER_SZ);
                }
            }
        }
    }
}
