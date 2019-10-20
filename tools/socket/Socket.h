#ifndef SOCKET_H_
#define SOCKET_H_

#include <sys/types.h>

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

    ssize_t read(void * buf, ssize_t count);

    ssize_t write(const void * buf, ssize_t count);

    void set_reuse_address(bool on);

    void set_reuse_port(bool on);

    void set_keep_alive(bool on);

    void set_no_delay(bool on);

protected:
    const int m_sockfd;
};

#endif // SOCKET_H_