#ifndef SERVERSOCKET_H_
#define SERVERSOCKET_H_

class ConnSocket;
class InetAddress;

#include "tools/base/noncopyable.h"

class ServerSocket : noncopyable{
public:
  ServerSocket(int sockfd);

  ~ServerSocket();

  inline int get_sockfd() const {
    return m_sockfd;
  }

  void bind(const InetAddress &localAddr);

  void listen();

  ConnSocket accept();

  ConnSocket accept_nonblocking();

  void set_reuse_address(bool on);

  void set_reuse_port(bool on);

  void set_keep_alive(bool on);

  void set_no_delay(bool on);

private:
  const int m_sockfd;
};

#endif // SERVERSOCKET_H_