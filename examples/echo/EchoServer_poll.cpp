#include <vector>
#include <map>

#include <poll.h>       // poll
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
  ServerSocket ss(sockets::create_nonblocking_socket(sockets::IPv4));
  InetAddress addr(LISTEN_PORT);
  ss.set_reuse_address(true);
  ss.bind(addr);
  ss.listen();
  LOG_INFO("server is listening...");

  std::vector<struct pollfd> pollfds;
  struct pollfd pd;
  pd.fd = ss.get_sockfd();
  pd.events = POLLIN;                                                // 监听读事件 -- 监听套接字的读事件发生时，表示有连接接入
  pollfds.push_back(pd);

  std::map<int, ConnSocket> connPool;
  int nready;
  char buf[BUFFER_SZ];
  ssize_t len = 0;

  for(;;){
    nready = poll(pollfds.data(), pollfds.size(), -1);
    if(nready <= 0){
      LOG_WARN("select error");
      continue;
    }

    if(pollfds[0].revents & POLLIN){
      try{
        ConnSocket connSocket = ss.accept_nonblocking();
        pd.fd = connSocket.get_sockfd();
        pd.events = POLLIN;                                         // 监听读事件 -- 连接套接字的读事件发生时，表示接收到数据
        pollfds.push_back(pd);
        connPool.insert({pd.fd, connSocket});
      }
      catch(const Exception &e){
        LOG_WARN("accept error");
      }

      -- nready;
      if(nready == 0){
        continue;
      }
    }
    
    for(auto iter = pollfds.begin() + 1; iter != pollfds.end();){
      if(iter->revents & POLLIN){
        auto connSocketIter = connPool.find(iter->fd);
        len = connSocketIter->second.read(buf, BUFFER_SZ);
        if(len < 0){
          LOG_WARN("read error");
        }
        else if(len == 0){
          LOG_INFO("client disconnects");
          iter = pollfds.erase(iter);
          connPool.erase(connSocketIter);
          continue;
        }
        else{
          connSocketIter->second.write(buf, strlen(buf));
        }

        -- nready;
        if(nready == 0){
          break;
        }
      }

      ++ iter;
    }
  }

  return 0;
}