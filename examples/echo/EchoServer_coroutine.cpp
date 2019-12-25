#include <iostream>

#include <string.h> // memset
#include <netinet/in.h>

#include <tools/log/log.h>
#include <tools/base/Exception.h>
#include <tools/socket/Socket.h>
#include <tools/socket/SocketsOps.h>
#include <tools/socket/InetAddress.h>
#include <tools/socket/ServerSocket.h>
#include <tools/coroutine/CoSchedulerThread.h>

#define LISTEN_PORT 9000
#define BUFFER_SZ 1024

void echo(Socket connSocket, CoScheduler *csd){
  int len;
  char buf[BUFFER_SZ];
  for(;;){
    len = connSocket.read(buf, BUFFER_SZ);
    if(len < 0){
      if(errno == EAGAIN){
        csd->yield();
      }
      else{
        LOG_WARN("read error");
      }
    }
    else if(len == 0){
      LOG_INFO("client disconnects");
      break;
    }

    connSocket.write(buf, len);
    memset(buf, 0, BUFFER_SZ);
  }
}

int main(){
  ServerSocket ss(sockets::create_socket(AF_INET));
  InetAddress addr(LISTEN_PORT);
  ss.bind(addr);
  ss.listen();
  LOG_INFO("server is listening...");

  CoSchedulerThread cst;
  CoScheduler *csd = cst.start();
  
  for(;;){
    try{
      Socket connSocket = ss.accept_nonblocking(nullptr);
      csd->create_coroutine(std::bind(echo, connSocket, std::placeholders::_1));
    }
    catch(const Exception &e){
      LOG_WARN("accept error");
    }
  }

  return 0;
}
