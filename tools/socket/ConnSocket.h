#ifndef CONNSOCKET_H_
#define CONNSOCKET_H_

#include <sys/types.h>

#include "tools/base/copyable.h"
#include "tools/base/noncopyable.h"
#include "tools/socket/InetAddress.h"

class ConnSocket;

class SocketGuard : noncopyable {
private:
  friend class ConnSocket;
  SocketGuard(int sockfd);
  ~SocketGuard();

  const int m_sockfd;
  int m_count;
};

class ConnSocket : copyable{
public:
  ConnSocket();

  ConnSocket(int sockfd, InetAddress addr);

  ConnSocket(const ConnSocket &sock);

  ~ConnSocket();

  ConnSocket& operator=(const ConnSocket &sock);

  inline int get_sockfd() const {
    return m_guard->m_sockfd;
  }

  inline InetAddress get_remote_address() const {
    return m_remoteAddr;
  }

  ssize_t read(void *buf, ssize_t count);

  ssize_t readv(const struct iovec *iov, int iovcnt);

  ssize_t write(const void *buf, ssize_t count);

  void shutdown_write();

  void set_keep_alive(bool on);

  void set_no_delay(bool on);

private:
  SocketGuard *m_guard;
  InetAddress m_remoteAddr;
};

#endif // CONNSOCKET_H_