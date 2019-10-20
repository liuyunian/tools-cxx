#include <string.h>
#include <unistd.h>     // read write
#include <netinet/in.h>
#include <netinet/tcp.h>

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

ssize_t Socket::read(void * buf, ssize_t count){
    memset(buf, 0, count);
    return ::read(m_sockfd, buf, count);
}

ssize_t Socket::write(const void * buf, ssize_t count){
    return ::write(m_sockfd, buf, count);
}

void Socket::set_reuse_address(bool on){
    int optVal = on ? 1 : 0;
    int ret = ::setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &optVal, static_cast<socklen_t>(sizeof(optVal)));
    if(ret < 0 && on){
        LOG_SYSFATAL("Failed to set reuse addr in Socket::set_reuseAddr(bool)");
    }
}

void Socket::set_reuse_port(bool on){
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

void Socket::set_keep_alive(bool on){
    int optVal = on ? 1 : 0;
    int ret = ::setsockopt(m_sockfd, SOL_SOCKET, SO_KEEPALIVE, &optVal, static_cast<socklen_t>(sizeof(optVal)));
    if(ret < 0 && on){
        LOG_SYSFATAL("Failed to set keep alive in Socket::set_reuseAddr(bool)");
    }
}

void Socket::set_no_delay(bool on){
    int optVal = on ? 1 : 0;
    int ret = ::setsockopt(m_sockfd, IPPROTO_TCP, TCP_NODELAY, &optVal, static_cast<socklen_t>(sizeof(optVal)));
    if(ret < 0 && on){
        LOG_SYSFATAL("Failed to set TCP no delay in Socket::set_reuseAddr(bool)");
    }
}