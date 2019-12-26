#include <string.h> // memset

#include "tools/socket/ConnSocket.h"
#include "tools/socket/SocketsOps.h"

SocketGuard::SocketGuard(int sockfd) : 
  m_sockfd(sockfd),
  m_count(1)
{}

SocketGuard::~SocketGuard(){
  sockets::close(m_sockfd);
}

ConnSocket::ConnSocket() : 
  m_guard(nullptr),
  m_remoteAddr(){}

ConnSocket::ConnSocket(int sockfd, InetAddress addr) :
  m_guard(new SocketGuard(sockfd)),
  m_remoteAddr(addr){}

ConnSocket::ConnSocket(const ConnSocket &sock) : 
  m_guard(sock.m_guard),
  m_remoteAddr(sock.m_remoteAddr)
{
  ++ m_guard->m_count;
}

ConnSocket::~ConnSocket(){
  -- m_guard->m_count;
  if(m_guard->m_count == 0){
    delete m_guard;
  }
}

ConnSocket& ConnSocket::operator=(const ConnSocket &sock){
  if(m_guard != nullptr){
    this->~ConnSocket();
  }

  m_guard = sock.m_guard;
  ++ m_guard->m_count;
  m_remoteAddr = sock.m_remoteAddr;
}

ssize_t ConnSocket::read(void *buf, ssize_t count){
  memset(buf, 0, count);
  return sockets::read(m_guard->m_sockfd, buf, count);
}

ssize_t ConnSocket::write(const void *buf, ssize_t count){
  return sockets::write(m_guard->m_sockfd, buf, count);
}