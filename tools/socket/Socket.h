#ifndef SOCKET_H_
#define SOCKET_H_

#include "tools/base/noncopyable.h"

class InetAddress;  // 前向声明

class Socket : public noncopyable {
public:
    Socket(int sockfd);

    virtual ~Socket();

    inline int get_sockfd() const {
        return m_sockfd;
    }

    void bind(const InetAddress& localAddr);

    void set_reuseAddr(bool on);

    void set_reusePort(bool on);

protected:
    const int m_sockfd;
};

#endif // SOCKET_H_