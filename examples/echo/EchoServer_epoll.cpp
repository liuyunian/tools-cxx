#include <vector>
#include <map>

#include <sys/epoll.h>  // epoll
#include <string.h>     // memset

#include <tools/log/log.h>
#include <tools/base/Exception.h>
#include <tools/socket/ConnSocket.h>
#include <tools/socket/SocketsOps.h>
#include <tools/socket/InetAddress.h>
#include <tools/socket/ServerSocket.h>

#define LISTEN_PORT 9000
#define BUFFER_SZ 1024

int main(){
  ServerSocket ss(LISTEN_PORT);
  ss.listen();
  LOG_INFO("server is listening...");

  int epfd = ::epoll_create1(EPOLL_CLOEXEC);

  struct epoll_event event;
  event.events = EPOLLIN;                                                           // 关注监听套接字的读事件，LT工作模式（默认）
  event.data.fd = ss.get_sockfd();
  ::epoll_ctl(epfd, EPOLL_CTL_ADD, event.data.fd, &event);

  std::map<int, ConnSocket> connPool;
  int nready;
  char buf[BUFFER_SZ];
  ssize_t len = 0;
  std::vector<struct epoll_event> revents(16);                                      // 保存epoll_wait()函数返回的事件，初始大小设置为16，之后可变长

  for(;;){
    nready = epoll_wait(epfd, revents.data(), revents.size(), -1);
    if(nready <= 0){
      LOG_WARN("epoll_wait error");
      continue;
    }

    if(nready == revents.size()){
      revents.resize(revents.size()*2);
    }

    for(auto &revent : revents){
      if(!(revent.events & EPOLLIN)){                                               // 不处理非可读事件
        continue;
      }

      if(revent.data.fd == ss.get_sockfd()){                                        // 监听套接字的可读事件
        try{
          ConnSocket connSocket = ss.accept_nonblocking();
          int connfd = connSocket.get_sockfd();
          connPool.insert({connfd, connSocket});          
          event.data.fd = connfd;
          event.events = EPOLLIN;                                                   // 关注连接套接字的读事件，LT工作模式
          epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &event);
        }
        catch(const Exception &e){
          LOG_WARN("accept error");
        }
      }
      else{
        auto connSocketIter = connPool.find(revent.data.fd);
        len = connSocketIter->second.read(buf, BUFFER_SZ);
        if(len < 0){
          LOG_WARN("read error");
        }
        else if(len == 0){
          LOG_INFO("client disconnects");
          connPool.erase(connSocketIter);
        }
        else{
          connSocketIter->second.write(buf, len);
        }
      }
    }
  }

  return 0;
}
