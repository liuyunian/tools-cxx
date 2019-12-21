#ifndef SOCKET_H_
#define SOCKET_H_

#include <sys/types.h>

#include "tools/base/copyable.h"
#include "tools/base/noncopyable.h"

class InetAddress;
class Socket;

class SocketGuard : noncopyable {
private:
  friend class Socket;
  SocketGuard(int sockfd);
  ~SocketGuard();

  const int m_sockfd;
  int m_count;
};

class Socket : copyable{
public:
  Socket();

  Socket(int sockfd);

  Socket(const Socket &sock);

  virtual ~Socket();

  Socket& operator=(const Socket &sock);

  inline int get_sockfd() const {
    return m_guard->m_sockfd;
  }

  void bind(const InetAddress& localAddr);

  ssize_t read(void *buf, ssize_t count);

  ssize_t write(const void *buf, ssize_t count);

private:
  SocketGuard *m_guard;
};

#endif // SOCKET_H_