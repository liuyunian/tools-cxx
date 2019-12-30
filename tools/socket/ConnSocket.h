#ifndef CONNSOCKET_H_
#define CONNSOCKET_H_

#include <sys/types.h>

#include "tools/base/copyable.h"
#include "tools/base/noncopyable.h"
#include "tools/socket/InetAddress.h"

class ConnSocket : noncopyable{
public:
  ConnSocket();

  ConnSocket(int sockfd, InetAddress addr);

  ConnSocket(ConnSocket &&sock);

  ~ConnSocket();

  ConnSocket& operator=(ConnSocket &&sock);

  inline int get_sockfd() const {
    return m_sockfd;
  }

  inline InetAddress get_remote_address() const {
    return m_remoteAddr;
  }

  ssize_t read(void *buf, ssize_t count);

  ssize_t write(const void *buf, ssize_t count);

private:
  int m_sockfd;
  InetAddress m_remoteAddr;
};

#endif // CONNSOCKET_H_