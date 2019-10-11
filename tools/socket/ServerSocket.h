#ifndef SERVERSOCKET_H_
#define SERVERSOCKET_H_

#include "Socket.h"

class ServerSocket : public Socket {
public:
    ServerSocket(int sockfd) : Socket(sockfd) {}

    ~ServerSocket() = default;

    void listen();

    Socket* accept(InetAddress* peerAddr);

    Socket* accept_nonblocking(InetAddress* peerAddr);
};

#endif // SERVERSOCKET_H_