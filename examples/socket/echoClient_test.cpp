#include <string.h>     // strlen
#include <netinet/in.h> // AF_INET

#include <tools/socket/Endian.h>
#include <tools/socket/Socket.h>
#include <tools/socket/SocketsOps.h>
#include <tools/socket/InetAddress.h>

#define PORT 9000
#define IP "127.0.0.1"
#define BUFFER_SZ 1024

int main(){
    Socket socket(sockets::create_socket(AF_INET));

    InetAddress addr(IP, PORT);
    sockets::connect(socket.get_sockfd(), addr.get_sockaddr());

    char recvBuf[BUFFER_SZ] = {0};
    char sendBuf[BUFFER_SZ] = {0};
    while(fgets(sendBuf, BUFFER_SZ, stdin) != nullptr){
        socket.write(sendBuf, strlen(sendBuf));
        socket.read(recvBuf, sizeof(recvBuf));

        fputs(recvBuf, stdout);
        memset(sendBuf, 0, BUFFER_SZ);
    }

    return 0;
}
