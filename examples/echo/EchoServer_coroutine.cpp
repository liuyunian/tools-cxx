#include <iostream>

#include <string.h> // strlen
#include <netinet/in.h>

#include <tools/socket/Socket.h>
#include <tools/socket/SocketsOps.h>
#include <tools/socket/InetAddress.h>
#include <tools/socket/ServerSocket.h>
#include <tools/coroutine/SchedulerThread.h>

#define LISTEN_PORT 9000
#define BUFFER_SZ 1024

void echo(Socket *connSocket, co::Scheduler *sd){
  int len;
  char buf[BUFFER_SZ];
  for(;;){
    len = connSocket->read(buf, BUFFER_SZ);
    if(len < 0){
      if(errno == EAGAIN){
        sd->yield();
      }
    }
    else if(len == 0){
      std::cout << "客户端断开连接" << std::endl;
      delete connSocket;
      break;
    }

    connSocket->write(buf, strlen(buf));
    memset(buf, 0, BUFFER_SZ);
  }
}

int main(){
  ServerSocket ss(sockets::create_socket(AF_INET));
  InetAddress addr(LISTEN_PORT);
  ss.bind(addr);
  ss.listen();

  co::SchedulerThread st;
  co::Scheduler *s = st.start();
  
  Socket* connSocket;
  for(;;){
    connSocket = ss.accept_nonblocking(nullptr);
    if(connSocket == nullptr){ // expected error
        // TODO
        continue;
    }

    std::cout << "accept one" << std::endl;

    s->create_coroutine(std::bind(echo, connSocket, std::placeholders::_1));
  }

  return 0;
}
