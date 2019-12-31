#ifndef SERVERSOCKET_H_
#define SERVERSOCKET_H_

#include <arpa/inet.h>

class ConnSocket;
class InetAddress;

#include "tools/base/noncopyable.h"

class ServerSocket : noncopyable{
public:
  ServerSocket(int port, sa_family_t family = AF_INET, bool isBlocking = true);

  ServerSocket(const InetAddress &localAddr, bool isBlocking = true);

  ~ServerSocket();

  inline int get_sockfd() const {
    return m_sockfd;
  }

  // void bind(const InetAddress &localAddr);

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