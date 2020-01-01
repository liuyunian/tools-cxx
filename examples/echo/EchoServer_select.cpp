#include <vector>

#include <string.h>     // memset
#include <unistd.h>     // select

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
  ss.set_reuse_address(true);
  ss.listen();
  LOG_INFO("server is listening...");

  int maxfd = ss.get_sockfd();                                    // maxfd用来记录当前最大的文件描述符
  fd_set allfds, rfds;                                            // allfds用来记录所关心的文件描述符，rfds则作为select的第二个参数，是一个“值-结果”参数
  FD_ZERO(&allfds);
  FD_SET(maxfd, &allfds);

  int nready = 0;
  std::vector<ConnSocket> connPool;
  char buf[BUFFER_SZ];
  ssize_t len = 0;

  for(;;){
    rfds = allfds;
    nready = ::select(maxfd+1, &rfds, nullptr, nullptr, nullptr); // 直到一个事件发生时才返回
    if(nready <= 0){
      LOG_WARN("select error");
      continue;
    }

    if(FD_ISSET(ss.get_sockfd(), &rfds)){
      try{
        ConnSocket connSocket = ss.accept_nonblocking();
        maxfd = connSocket.get_sockfd();
        FD_SET(maxfd, &allfds);
        connPool.push_back(connSocket);
      }
      catch(const Exception &e){
        LOG_WARN("accept error");
      }

      -- nready;
      if(nready == 0){
        continue;
      }
    }

    for(auto iter = connPool.begin(); iter != connPool.end();){
      if(FD_ISSET(iter->get_sockfd(), &rfds)){
        len = iter->read(buf, BUFFER_SZ);
        if(len < 0){
          LOG_WARN("read error");
        }
        else if(len == 0){
          LOG_INFO("client disconnects");
          FD_CLR(iter->get_sockfd(), &allfds);
          iter = connPool.erase(iter);
          continue;
        }
        else{
          iter->write(buf, len);
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