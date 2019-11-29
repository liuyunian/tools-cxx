#include <string.h> // strlen
#include <netinet/in.h>

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
  Socket* connSocket;
  for(;;){
      connSocket = ss.accept(nullptr);
      if(connSocket == nullptr){ // expected error
          // TODO
          continue;
      }

      while((len = connSocket->read(buf, BUFFER_SZ)) > 0){
          connSocket->write(buf, strlen(buf));
      }

      delete connSocket;
  }

  return 0;
}
