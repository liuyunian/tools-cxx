#include <vector>
#include <map>

#include <poll.h>       // poll
#include <errno.h>      // errno
#include <string.h>     // memset
#include <signal.h>     // signal
#include <fcntl.h>      // open
#include <netinet/in.h> // AF_INET

#include <tools/log/log.h>
#include <tools/socket/Socket.h>
#include <tools/socket/SocketsOps.h>
#include <tools/socket/InetAddress.h>
#include <tools/socket/ServerSocket.h>

#define LISTEN_PORT 9000
#define BUFFER_SZ 1024

int main(){
    /**
     * 忽略SIGPIPE信号
     * SIGPIPE信号发生的实际：如果客户端没有按照四次挥手关闭TCP连接，那么服务器如果调用write或者send发送数据时，会收到一个RST报文段，相应的两个函数错误返回
     * 如果再次调用write或者send发送数据，那么就会收到一个内核发来的SIGPIPE信号，该信号的默认处理动作是终止进程
     * 为了服务器能够长时间稳定原型，这里忽略该信号
    */
    signal(SIGPIPE, SIG_IGN);

    int idlefd = open("/dev/null", O_RDONLY | O_CLOEXEC);               // 预留文件描述符

    ServerSocket ss(sockets::create_nonblocking_socket(AF_INET));

    InetAddress addr(LISTEN_PORT);
    ss.set_reuse_address(true);
    ss.bind(addr);

    struct pollfd pd;
    pd.fd = ss.get_sockfd();
    pd.events = POLL_IN;                                                // 监听读事件 -- 监听套接字的读事件发生时，表示有连接接入
    std::vector<struct pollfd> pollfds;
    pollfds.push_back(pd);

    ss.listen();
    LOG_INFO("服务器监听中...");

    Socket* connSocket;
    std::map<int, Socket*> connectionPool;
    int nready;                                                         // 记录poll函数返回的事件数
    char buf[BUFFER_SZ];
    ssize_t len = 0;                                                    // 记录recv()函数的返回值
    for(;;){
        nready = poll(pollfds.data(), pollfds.size(), -1);
        if(nready <= 0){
            if(errno == EINTR){
                continue;
            }

            LOG_SYSERR("poll函数执行错误");
            continue;
        }
        if(pollfds[0].revents & POLLIN){                                // poll不像epoll那样会将发生的事件都存放在一个数组中
            connSocket = ss.accept_nonblocking(nullptr);
            if(connSocket == nullptr){
                if(errno == EMFILE){                                    // 进程描述符已达到上限
                    sockets::close(idlefd);                             // 关闭预留描述符，进程有了一个空闲描述符
                    connSocket = ss.accept_nonblocking(nullptr);        // 此时可以正确的接受连接
                    delete connSocket;                                  // 服务器端优雅的关闭连接
                    
                    idlefd = open("/dev/null", O_RDONLY | O_CLOEXEC);   // 再次预留文件描述符
                    
                    continue;
                }

                LOG_SYSERR("accept()函数执行出错");
                continue;
            }

            pd.fd = connSocket->get_sockfd();
            pd.events = POLLIN;
            pd.revents = 0;
            pollfds.push_back(pd);
            connectionPool.insert({pd.fd, connSocket});

            -- nready;
            if(nready == 0){
                continue;
            }
        }
        for(auto iter = pollfds.begin() + 1; iter != pollfds.end(); ++ iter){
            if(iter->revents & POLLIN){
                -- nready;

                auto connSocketIter = connectionPool.find(iter->fd);
                len = connSocketIter->second->read(buf, BUFFER_SZ);
                if(len < 0){
                    LOG_SYSERR("调用recv()接收数据失败");
                }
                else if(len == 0){
                    LOG_INFO("客户端断开连接");

                    iter = pollfds.erase(iter);
                    -- iter;
                    delete connSocketIter->second;
                    connectionPool.erase(connSocketIter);

                    continue;
                }

                connSocketIter->second->write(buf, strlen(buf));
                memset(buf, 0, BUFFER_SZ);
            }
        }
    }

    return 0;
}
