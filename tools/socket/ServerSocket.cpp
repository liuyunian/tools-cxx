#include <string.h>     // memset

#include "tools/socket/ServerSocket.h"
#include "tools/socket/Socket.h"
#include "tools/base/Exception.h"
#include "tools/socket/SocketsOps.h"
#include "tools/socket/InetAddress.h"

ServerSocket::ServerSocket(int sockfd) : 
  m_sockfd(sockfd)
{}

ServerSocket::~ServerSocket(){
  sockets::close(m_sockfd);
}

void ServerSocket::bind(const InetAddress &localAddr){
  sockets::bind(m_sockfd, localAddr.get_sockaddr());
}

void ServerSocket::listen(){
  sockets::listen(m_sockfd);
}

Socket ServerSocket::accept(InetAddress *peerAddr){
  int connfd = -1;
  if(peerAddr == nullptr){
    connfd = sockets::accept(m_sockfd);
    if(connfd < 0){
      throw Exception("accept error");
    }

    return Socket(connfd);
  }

  struct sockaddr_in6 addr;
  memset(&addr, 0, sizeof(addr));
  connfd = sockets::accept(m_sockfd, &addr);
  if(connfd < 0){
    throw Exception("accept error");
  }

  peerAddr->set_sockaddr_in6(addr);
  return Socket(connfd);
}

Socket ServerSocket::accept_nonblocking(InetAddress *peerAddr){
  int connfd = -1;
  if(peerAddr == nullptr){
    connfd = sockets::accept_nonblocking(m_sockfd);
    if(connfd < 0){
      throw Exception("accept_nonblocking error");
    }

    return Socket(connfd);
  }

  struct sockaddr_in6 addr;
  memset(&addr, 0, sizeof(addr));
  connfd = sockets::accept_nonblocking(m_sockfd, &addr);
  if(connfd < 0){
    throw Exception("accept_nonblocking error");
  }

  peerAddr->set_sockaddr_in6(addr);
  return Socket(connfd);
}

void ServerSocket::set_reuse_address(bool on){
  sockets::set_reuse_address(m_sockfd, on);
}

void ServerSocket::set_reuse_port(bool on){
  sockets::set_reuse_port(m_sockfd, on);
}

void ServerSocket::set_keep_alive(bool on){
  sockets::set_keep_alive(m_sockfd, on);
}

void ServerSocket::set_no_delay(bool on){
  sockets::set_keep_alive(m_sockfd, on);
}