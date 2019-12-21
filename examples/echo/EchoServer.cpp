#include <string.h>     // strlen
#include <netinet/in.h>

#include <tools/base/Exception.h>
#include <tools/socket/Socket.h>
#include <tools/socket/SocketsOps.h>
#include <tools/socket/InetAddress.h>
#include <tools/socket/ServerSocket.h>

#define LISTEN_PORT 9000
#define BUFFER_SZ 1024

int main(){
  ServerSocket ss(sockets::create_socket(AF_INET));

  InetAddress addr(LISTEN_PORT);
  ss.bind(addr);

  ss.listen();

  char buf[BUFFER_SZ];
  ssize_t len = 0;
  for(;;){
    try{
      Socket connSocket = ss.accept_nonblocking(nullptr);

      while(connSocket.read(buf, BUFFER_SZ) > 0){
        connSocket.write(buf, strlen(buf));
      }
    }
    catch(const Exception &e){
      continue;
    }
  }

  return 0;
}
