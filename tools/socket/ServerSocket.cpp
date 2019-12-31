#include <string.h>     // memset

#include "tools/base/Exception.h"
#include "tools/socket/ServerSocket.h"
#include "tools/socket/ConnSocket.h"
#include "tools/socket/SocketsOps.h"
#include "tools/socket/InetAddress.h"

ServerSocket::ServerSocket(int port, sa_family_t family, bool isBlocking) : 
  m_sockfd(isBlocking ? sockets::create_socket(family)
                      : sockets::create_nonblocking_socket(family))
{
  InetAddress localAddr(port, family);
  sockets::bind(m_sockfd, localAddr.get_sockaddr());
}

ServerSocket::ServerSocket(const InetAddress &localAddr, bool isBlocking) : 
  m_sockfd(isBlocking ? sockets::create_socket(localAddr.get_family())
                      : sockets::create_nonblocking_socket(localAddr.get_family()))
{
  sockets::bind(m_sockfd, localAddr.get_sockaddr());
}

ServerSocket::~ServerSocket(){
  sockets::close(m_sockfd);
}

// void ServerSocket::bind(const InetAddress &localAddr){
//   sockets::bind(m_sockfd, localAddr.get_sockaddr());
// }

void ServerSocket::listen(){
  sockets::listen(m_sockfd);
}

ConnSocket ServerSocket::accept(){
  struct sockaddr_in6 addr;
  memset(&addr, 0, sizeof(addr));
  int connfd = sockets::accept(m_sockfd, &addr);
  if(connfd < 0){
    throw Exception("accept error");
  }

  InetAddress remoteAddr(addr);
  return ConnSocket(connfd, remoteAddr);
}

ConnSocket ServerSocket::accept_nonblocking(){
  struct sockaddr_in6 addr;
  memset(&addr, 0, sizeof(addr));
  int connfd = sockets::accept_nonblocking(m_sockfd, &addr);
  if(connfd < 0){
    throw Exception("accept_nonblocking error");
  }

  InetAddress remoteAddr(addr);
  return ConnSocket(connfd, remoteAddr);
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