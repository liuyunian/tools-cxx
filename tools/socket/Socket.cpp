#include <string.h>
#include <netinet/in.h>

#include "Socket.h"
#include "SocketsOps.h"
#include "InetAddress.h"

#include "tools/log/log.h"

Socket::Socket(int sockfd) :
    m_sockfd(sockfd){}

Socket::~Socket(){
    sockets::close(m_sockfd);
}

void Socket::bind(const InetAddress& localAddr){
    sockets::bind(m_sockfd, localAddr.get_sockaddr());
}

void Socket::set_reuseAddr(bool on){
    int optVal = on ? 1 : 0;
    int ret = ::setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &optVal, static_cast<socklen_t>(sizeof(optVal)));
    if(ret < 0 && on){
        LOG_SYSFATAL("Failed to set reuse addr in Socket::set_reuseAddr(bool)");
    }
}

void Socket::set_reusePort(bool on){
#ifdef SO_REUSEPORT
    int optVal = on ? 1:0;
    int ret = ::setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEPORT, &optVal, static_cast<socklen_t>(sizeof(optVal)));
        if(ret < 0 && on){
        LOG_SYSFATAL("Failed to set reuse port in Socket::set_reusePort(bool)");
    }
#else
    if(on){
        LOG_ERR("SO_REUSEPORT is not supported");
    }
#endif
}