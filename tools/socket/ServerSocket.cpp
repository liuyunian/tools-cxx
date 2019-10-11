#include <string.h>     // memset

#include "ServerSocket.h"
#include "tools/socket/SocketsOps.h"
#include "tools/socket/InetAddress.h"

void ServerSocket::listen(){
    sockets::listen(get_sockfd());
}

Socket* ServerSocket::accept(InetAddress* peerAddr){
    int connfd = -1;
    if(peerAddr == nullptr){
        connfd = sockets::accept(m_sockfd);
        if(connfd < 0){
            return nullptr;
        }

        return new Socket(connfd);
    }

    struct sockaddr_in6 addr;
    memset(&addr, 0, sizeof(addr));
    connfd = sockets::accept(m_sockfd, &addr);
    if(connfd < 0){
        return nullptr;
    }

    peerAddr->set_sockaddr_in6(addr);
    return new Socket(connfd);
}

Socket* ServerSocket::accept_nonblocking(InetAddress* peerAddr){
    int connfd = -1;
    if(peerAddr == nullptr){
        connfd = sockets::accept_nonblocking(m_sockfd);
        if(connfd < 0){
            return nullptr;
        }

        return new Socket(connfd);
    }

    struct sockaddr_in6 addr;
    memset(&addr, 0, sizeof(addr));
    connfd = sockets::accept_nonblocking(m_sockfd, &addr);
    if(connfd < 0){
        return nullptr;
    }

    peerAddr->set_sockaddr_in6(addr);
    return new Socket(connfd);
}