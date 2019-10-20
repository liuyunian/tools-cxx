#include <fcntl.h>      // fcntl
#include <errno.h>      // errno
#include <unistd.h>     // close
#include <string.h>     // strlen memset
#include <stdio.h>      // snprintf
#include <assert.h>     // assert
#include <sys/socket.h>

#include "tools/log/log.h"

#include "SocketsOps.h"
#include "Endian.h"

static void
set_nonblock_and_cloexec(int sockfd){
    // non-block
    int flags = ::fcntl(sockfd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    int ret = ::fcntl(sockfd, F_SETFL, flags);
    if(ret < 0){
        LOG_SYSFATAL("Failed to set nonblocking in set_nonblock_and_cloexec(int)");
    }

    // cloexec
    flags = ::fcntl(sockfd, F_GETFL, 0);
    flags |= FD_CLOEXEC;
    ret = ::fcntl(sockfd, F_SETFL, flags);
    if(ret < 0){
        LOG_SYSFATAL("Failed to set cloexec in set_nonblock_and_cloexec(int)");
    }
}

int sockets::create_socket(int family){
    int sockfd = ::socket(family, SOCK_STREAM, IPPROTO_TCP);
    if(sockfd < 0){
        LOG_SYSFATAL("Failed to create socket in create_socket(int)");
    }

    return sockfd;
}

int sockets::create_nonblocking_socket(int family){
#ifdef __linux__
    int sockfd = ::socket(family, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if(sockfd < 0){
        LOG_SYSFATAL("Failed to create socket in create_nonblocking_socket(int)");
    }
#else
    int sockfd = create_socket(family);
    ::set_nonblock_and_cloexec(sockfd);
#endif

    return sockfd;
}

void sockets::close(int sockfd){
    int ret = ::close(sockfd);
    if(ret < 0){
        LOG_SYSFATAL("Failed to close sockfd in sockets::close(int)");
    }
}

void sockets::bind(int sockfd, const struct sockaddr* addr){
    int ret = ::bind(sockfd, addr, static_cast<socklen_t>(sizeof(*addr)));                  // 在MacOS上第三个参数填sizof(struct sockaddr_in6)运行时会出错 -- Invalid argument
    if(ret < 0){
        LOG_SYSFATAL("Failed to bind addr in sockets::bind(int, const struct sockaddr*)");
    }
}

void sockets::listen(int sockfd){
    int ret = ::listen(sockfd, SOMAXCONN);
    if(ret < 0){
        LOG_SYSFATAL("Failed to listen in sockets::listen(int)");
    }
}

int sockets::accept(int sockfd){
    int connfd = ::accept(sockfd, nullptr, nullptr);
    if(connfd < 0){
        int savedErrno = errno;
        switch(errno){
            case EAGAIN:
            case ECONNABORTED:
            case EINTR:
            case EPROTO:
            case EPERM:
            case EMFILE:
                LOG_SYSERR("expected error of ::accept in sockets::accept(int)");
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
                LOG_SYSFATAL("unexpected error of ::accept in sockets::accept(int)");
                break;
            default:
                LOG_SYSFATAL("unknown error of ::accept in sockets::accept(int)");
                break;
        }
    }

    return connfd;
}

int sockets::accept(int sockfd, struct sockaddr_in6* addr){
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

int sockets::accept_nonblocking(int sockfd){
#ifdef __linux__
    int connfd = ::accept4(sockfd, nullptr, nullptr, SOCK_NONBLOCK | SOCK_CLOEXEC);
#else
    int connfd = ::accept(sockfd, nullptr, nullptr);
    set_nonblock_and_cloexec(connfd);
#endif
    if(connfd < 0){
        int savedErrno = errno;
        switch(errno){
            case EAGAIN:
            case ECONNABORTED:
            case EINTR:
            case EPROTO:
            case EPERM:
            case EMFILE:
                LOG_SYSERR("expected error of ::accept in accept_nonblocking(int)");
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
                LOG_SYSFATAL("unexpected error of ::accept in accept_nonblocking(int)");
                break;
            default:
                LOG_SYSFATAL("unknown error of ::accept in accept_nonblocking(int)");
                break;
        }
    }

    return connfd;
}

int sockets::accept_nonblocking(int sockfd, struct sockaddr_in6* addr){
    socklen_t addrlen = static_cast<socklen_t>(sizeof(struct sockaddr_in6));
#ifdef __linux__
    int connfd = ::accept4(sockfd, reinterpret_cast<struct sockaddr*>(addr), &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
#else
    int connfd = ::accept(sockfd, reinterpret_cast<struct sockaddr*>(addr), &addrlen);
    set_nonblock_and_cloexec(connfd);
#endif
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

void sockets::connect(int sockfd, const struct sockaddr* addr){
    int ret = ::connect(sockfd, addr, static_cast<socklen_t>(sizeof(*addr)));                       // 在MacOS上第三个参数填sizof(struct sockaddr_in6)运行时会出错 -- Invalid argument
    if(ret < 0){
        LOG_SYSFATAL("Failed to connect server in sockets::connect(int, const struct sockaddr*)");
    }
}

void sockets::to_ip(const struct sockaddr* addr, char* buf, size_t size){
    if(addr->sa_family == AF_INET){
        assert(size >= INET_ADDRSTRLEN);
        const struct sockaddr_in* addr4 = reinterpret_cast<const struct sockaddr_in*>(addr);
        ::inet_ntop(AF_INET, &addr4->sin_addr, buf, size);
    }
    else{
        assert(size >= INET6_ADDRSTRLEN);
        const struct sockaddr_in6* addr6 = reinterpret_cast<const struct sockaddr_in6*>(addr);
        ::inet_ntop(AF_INET6, &addr6->sin6_addr, buf, size);
    }
}

void sockets::to_ip_and_port(const struct sockaddr* addr, char* buf, size_t size){
    to_ip(addr, buf, size);
    size_t end = ::strlen(buf);
    const struct sockaddr_in* addr4 = reinterpret_cast<const struct sockaddr_in*>(addr);
    uint16_t port = sockets::network_to_host16(addr4->sin_port);
    assert(size > end);
    snprintf(buf + end, size - end, ":%u", port);
}

void sockets::from_ip_and_port(const char * ip, uint16_t port, struct sockaddr_in* addr){
    addr->sin_family = AF_INET;
    addr->sin_port = host_to_network16(port);
    int ret = ::inet_pton(AF_INET, ip, &addr->sin_addr);
    if(ret < 0){
        LOG_SYSFATAL("Failed to call inet_pton in from_ip_and_port");
    }
}

void sockets::from_ip_and_port(const char * ip, uint16_t port, struct sockaddr_in6* addr){
    addr->sin6_family = AF_INET6;
    addr->sin6_port = host_to_network16(port);
    int ret = ::inet_pton(AF_INET6, ip, &addr->sin6_addr);
    if(ret < 0){
        LOG_SYSFATAL("Failed to call inet_pton in from_ip_and_port");
    }
}

struct sockaddr_in6 sockets::get_local_addr(int sockfd){
    struct sockaddr_in6 localAddr;
    memset(&localAddr, 0, sizeof(localAddr));
    socklen_t addrLen = static_cast<socklen_t>(sizeof(localAddr));
    int ret = ::getsockname(sockfd, reinterpret_cast<struct sockaddr*>(&localAddr), &addrLen);
    if(ret < 0){
        LOG_SYSFATAL("Failed to call getsockname in get_local_addr");
    }

    return localAddr;
}

struct sockaddr_in6 sockets::get_peer_addr(int sockfd){
    struct sockaddr_in6 peerAddr;
    memset(&peerAddr, 0, sizeof(peerAddr));
    socklen_t addrLen = static_cast<socklen_t>(sizeof(peerAddr));
    int ret = ::getpeername(sockfd, reinterpret_cast<struct sockaddr*>(&peerAddr), &addrLen);
    if(ret < 0){
        LOG_SYSFATAL("Failed to call getpeername in get_peer_addr");
    }

    return peerAddr;
}