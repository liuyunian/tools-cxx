#include <string.h> // memset

#include "tools/socket/ConnSocket.h"
#include "tools/socket/SocketsOps.h"

ConnSocket::ConnSocket() : 
  m_sockfd(-1),
  m_remoteAddr(){}

ConnSocket::ConnSocket(int sockfd, InetAddress addr) :
  m_sockfd(sockfd),
  m_remoteAddr(addr){}

ConnSocket::ConnSocket(ConnSocket &&sock) : 
  m_sockfd(sock.m_sockfd),
  m_remoteAddr(sock.m_remoteAddr)
{
  sock.m_sockfd = -1;
}

ConnSocket::~ConnSocket(){
  if(m_sockfd != -1){
    sockets::close(m_sockfd);
  }
}

ConnSocket& ConnSocket::operator=(ConnSocket &&sock){
  this->~ConnSocket();

  m_sockfd = sock.m_sockfd;
  m_remoteAddr = sock.m_remoteAddr;

  sock.m_sockfd = -1;
}

ssize_t ConnSocket::read(void *buf, ssize_t count){
  memset(buf, 0, count);
  return sockets::read(m_sockfd, buf, count);
}

ssize_t ConnSocket::write(const void *buf, ssize_t count){
  return sockets::write(m_sockfd, buf, count);
}