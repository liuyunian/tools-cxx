#include <string.h>     // strlen memset

#include <tools/base/Exception.h>
#include <tools/socket/ConnSocket.h>
#include <tools/socket/SocketsOps.h>
#include <tools/socket/InetAddress.h>
#include <tools/socket/ServerSocket.h>

#define LISTEN_PORT 9000
#define BUFFER_SZ 1024

int main(){
  ServerSocket ss(LISTEN_PORT, true);
  ss.set_reuse_address(true);
  ss.listen();

  char buf[BUFFER_SZ];
  ssize_t len = 0;
  for(;;){
    try{
      ConnSocket connSocket = ss.accept();

      while(connSocket.read(buf, BUFFER_SZ) > 0){
        connSocket.write(buf, strlen(buf));
        ::memset(buf, 0, BUFFER_SZ);
      }
    }
    catch(const Exception &e){
      continue;
    }
  }

  return 0;
}
