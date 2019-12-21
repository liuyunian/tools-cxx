#include <string.h>
#include <unistd.h>     // read write

#include "Socket.h"
#include "SocketsOps.h"
#include "InetAddress.h"

#include "tools/log/log.h"

SocketGuard::SocketGuard(int sockfd) : 
  m_sockfd(sockfd),
  m_count(1)
{}

SocketGuard::~SocketGuard(){
  sockets::close(m_sockfd);
}

Socket::Socket() : 
  m_guard(nullptr){}

Socket::Socket(int sockfd) :
  m_guard(new SocketGuard(sockfd)){}

Socket::Socket(const Socket &sock) : 
  m_guard(sock.m_guard)
{
  ++ m_guard->m_count;
}

Socket::~Socket(){
  -- m_guard->m_count;
  if(m_guard->m_count == 0){
    delete m_guard;
  }
}

Socket& Socket::operator=(const Socket &sock){
  if(m_guard != nullptr){
    this->~Socket();
  }

  m_guard = sock.m_guard;
  ++ m_guard->m_count;
}

void Socket::bind(const InetAddress &localAddr){
  sockets::bind(m_guard->m_sockfd, localAddr.get_sockaddr());
}

ssize_t Socket::read(void *buf, ssize_t count){
  memset(buf, 0, count);
  return ::read(m_guard->m_sockfd, buf, count);
}

ssize_t Socket::write(const void *buf, ssize_t count){
  return ::write(m_guard->m_sockfd, buf, count);
}