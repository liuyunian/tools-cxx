#ifndef SERVERSOCKET_H_
#define SERVERSOCKET_H_

#include <arpa/inet.h>

#include "tools/socket/InetAddress.h"

class ConnSocket;

#include "tools/base/noncopyable.h"

class ServerSocket : noncopyable{
public:
  ServerSocket(int port, bool isBlocking = false, sa_family_t family = AF_INET);

  ServerSocket(const InetAddress &localAddr, bool isBlocking = false);

  ~ServerSocket();

  inline int get_sockfd() const {
    return m_sockfd;
  }

  void listen();

  ConnSocket accept();

  ConnSocket accept_nonblocking();

  void set_reuse_address(bool on);

  void set_reuse_port(bool on);

private:
  InetAddress m_localAddr;
  const int m_sockfd;
};

#endif // SERVERSOCKET_H_