#include <fcntl.h>        // fcntl
#include <errno.h>        // errno
#include <unistd.h>       // close
#include <string.h>       // strlen memset
#include <stdio.h>        // snprintf
#include <assert.h>       // assert
#include <sys/socket.h>   // setsockopt
#include <netinet/tcp.h>  // TCP_NODELAY
#include <netinet/in.h>   // AF_INET

#include "tools/log/log.h"
#include "tools/socket/SocketsOps.h"
#include "tools/socket/Endian.h"

namespace sockets {

const sa_family_t IPv4 = AF_INET;
const sa_family_t IPv6 = AF_INET6;

int create_socket(sa_family_t family){
  int sockfd = ::socket(family, SOCK_STREAM, IPPROTO_TCP);
  if(sockfd < 0){
    LOG_SYSFATAL("Failed to create socket in sockets::create_socket(int)");
  }

  return sockfd;
}

int create_nonblocking_socket(sa_family_t family){
  int sockfd = ::socket(family, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
  if(sockfd < 0){
    LOG_SYSFATAL("Failed to create socket in sockets::create_nonblocking_socket(int)");
  }

  return sockfd;
}

void close(int sockfd){
  int ret = ::close(sockfd);
  if(ret < 0){
    LOG_SYSFATAL("Failed to close sockfd in sockets::close(int)");
  }
}

ssize_t read(int sockfd, void *buf, ssize_t count){
  return ::read(sockfd, buf, count);
}

ssize_t write(int sockfd, const void *buf, ssize_t count){
  return ::write(sockfd, buf, count);
}

void bind(int sockfd, const struct sockaddr *addr){
  int ret = ::bind(sockfd, addr, static_cast<socklen_t>(sizeof(*addr)));                  // 在MacOS上第三个参数赋值sizof(struct sockaddr_in6)运行时会出错 -- Invalid argument
  if(ret < 0){
    LOG_SYSFATAL("Failed to bind addr in sockets::bind(int, const struct sockaddr*)");
  }
}

void listen(int sockfd){
  int ret = ::listen(sockfd, SOMAXCONN);
  if(ret < 0){
    LOG_SYSFATAL("Failed to listen in sockets::listen(int)");
  }
}

int accept(int sockfd, struct sockaddr_in6 *addr){
  socklen_t addrlen = static_cast<socklen_t>(sizeof(struct sockaddr_in6));
  int connfd = ::accept(sockfd, reinterpret_cast<struct sockaddr*>(addr), &addrlen);
  if(connfd < 0){
    int savedErrno = errno;
    switch(errno){
      case EAGAIN:
      case ECONNABORTED:
      case EINTR:
      case EPROTO:
      case EPERM:
      case EMFILE:
        LOG_SYSERR("expected error of ::accept in sockets::accept(int, struct sockaddr_in6*)");
        errno = savedErrno;
        break;
      case EBADF:
      case EFAULT:
      case EINVAL:
      case ENFILE:
      case ENOBUFS:
      case ENOMEM:
      case ENOTSOCK:
      case EOPNOTSUPP:
        LOG_SYSFATAL("unexpected error of ::accept in sockets::accept(int, struct sockaddr_in6*)");
        break;
      default:
        LOG_SYSFATAL("unknown error of ::accept in sockets::accept(int, struct sockaddr_in6*)");
        break;
    }
  }

  return connfd;
}

int accept_nonblocking(int sockfd, struct sockaddr_in6 *addr){
  socklen_t addrlen = static_cast<socklen_t>(sizeof(struct sockaddr_in6));
  int connfd = ::accept4(sockfd, reinterpret_cast<struct sockaddr*>(addr), &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
  if(connfd < 0){
    int savedErrno = errno;
    switch(errno){
      case EAGAIN:
      case ECONNABORTED:
      case EINTR:
      case EPROTO:
      case EPERM:
      case EMFILE:
        LOG_SYSERR("expected error of ::accept in accept_nonblocking(int, struct sockaddr_in6*)");
        errno = savedErrno;
        break;
      case EBADF:
      case EFAULT:
      case EINVAL:
      case ENFILE:
      case ENOBUFS:
      case ENOMEM:
      case ENOTSOCK:
      case EOPNOTSUPP:
        LOG_SYSFATAL("unexpected error of ::accept in accept_nonblocking(int, struct sockaddr_in6*)");
        break;
      default:
        LOG_SYSFATAL("unknown error of ::accept in accept_nonblocking(int, struct sockaddr_in6*)");
        break;
    }
}

  return connfd;
}

void connect(int sockfd, const struct sockaddr *addr){
  int ret = ::connect(sockfd, addr, static_cast<socklen_t>(sizeof(*addr)));                       // 在MacOS上第三个参数填sizof(struct sockaddr_in6)运行时会出错 -- Invalid argument
  if(ret < 0){
    LOG_SYSFATAL("Failed to connect server in sockets::connect(int, const struct sockaddr*)");
  }
}

void set_reuse_address(int sockfd, bool on){
  int optVal = on ? 1 : 0;
  int ret = ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optVal, static_cast<socklen_t>(sizeof(optVal)));
  if(ret < 0 && on){
    LOG_SYSFATAL("Failed to set reuse addr in sockets::set_reuseAddr(bool)");
  }
}

void set_reuse_port(int sockfd, bool on){
#ifdef SO_REUSEPORT
  int optVal = on ? 1:0;
  int ret = ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &optVal, static_cast<socklen_t>(sizeof(optVal)));
    if(ret < 0 && on){
    LOG_SYSFATAL("Failed to set reuse port in sockets::set_reusePort(bool)");
  }
#else
  if(on){
    LOG_ERR("SO_REUSEPORT is not supported");
  }
#endif
}

void set_keep_alive(int sockfd, bool on){
  int optVal = on ? 1 : 0;
  int ret = ::setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &optVal, static_cast<socklen_t>(sizeof(optVal)));
  if(ret < 0 && on){
    LOG_SYSFATAL("Failed to set keep alive in sockets::set_reuseAddr(bool)");
  }
}

void set_no_delay(int sockfd, bool on){
  int optVal = on ? 1 : 0;
  int ret = ::setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &optVal, static_cast<socklen_t>(sizeof(optVal)));
  if(ret < 0 && on){
    LOG_SYSFATAL("Failed to set TCP no delay in sockets::set_reuseAddr(bool)");
  }
}

void to_ip(const struct sockaddr *addr, char *buf, size_t size){
  if(addr->sa_family == AF_INET){
    assert(size >= INET_ADDRSTRLEN);
    const struct sockaddr_in *addr4 = reinterpret_cast<const struct sockaddr_in*>(addr);
    ::inet_ntop(AF_INET, &addr4->sin_addr, buf, size);
  }
  else{
    assert(size >= INET6_ADDRSTRLEN);
    const struct sockaddr_in6 *addr6 = reinterpret_cast<const struct sockaddr_in6*>(addr);
    ::inet_ntop(AF_INET6, &addr6->sin6_addr, buf, size);
  }
}

void to_ip_and_port(const struct sockaddr *addr, char *buf, size_t size){
  to_ip(addr, buf, size);
  size_t end = ::strlen(buf);
  const struct sockaddr_in *addr4 = reinterpret_cast<const struct sockaddr_in*>(addr);
  uint16_t port = network_to_host16(addr4->sin_port);
  assert(size > end);
  snprintf(buf + end, size - end, ":%u", port);
}

void from_ip_and_port(const char *ip, uint16_t port, struct sockaddr_in *addr){
  addr->sin_family = AF_INET;
  addr->sin_port = host_to_network16(port);
  int ret = ::inet_pton(AF_INET, ip, &addr->sin_addr);
  if(ret < 0){
    LOG_SYSFATAL("Failed to call inet_pton in from_ip_and_port");
  }
}

void from_ip_and_port(const char *ip, uint16_t port, struct sockaddr_in6 *addr){
  addr->sin6_family = AF_INET6;
  addr->sin6_port = host_to_network16(port);
  int ret = ::inet_pton(AF_INET6, ip, &addr->sin6_addr);
  if(ret < 0){
    LOG_SYSFATAL("Failed to call inet_pton in from_ip_and_port");
  }
}

struct sockaddr_in6 get_local_addr(int sockfd){
  struct sockaddr_in6 localAddr;
  memset(&localAddr, 0, sizeof(localAddr));
  socklen_t addrLen = static_cast<socklen_t>(sizeof(localAddr));
  int ret = ::getsockname(sockfd, reinterpret_cast<struct sockaddr*>(&localAddr), &addrLen);
  if(ret < 0){
    LOG_SYSFATAL("Failed to call getsockname in get_local_addr");
  }

  return localAddr;
}

struct sockaddr_in6 get_peer_addr(int sockfd){
  struct sockaddr_in6 peerAddr;
  memset(&peerAddr, 0, sizeof(peerAddr));
  socklen_t addrLen = static_cast<socklen_t>(sizeof(peerAddr));
  int ret = ::getpeername(sockfd, reinterpret_cast<struct sockaddr*>(&peerAddr), &addrLen);
  if(ret < 0){
    LOG_SYSFATAL("Failed to call getpeername in get_peer_addr");
  }

  return peerAddr;
}

} // namespace sockets