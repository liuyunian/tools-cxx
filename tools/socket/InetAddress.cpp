#include <string.h>
#include <netinet/in.h>

#include "InetAddress.h"
#include "SocketsOps.h"
#include "Endian.h"

static_assert(sizeof(InetAddress) == sizeof(struct sockaddr_in6), "InetAddress is same size as sockaddr_in6");

static_assert(offsetof(sockaddr_in, sin_family) == 0, "sin_family offset 0");
static_assert(offsetof(sockaddr_in6, sin6_family) == 0, "sin6_family offset 0");
static_assert(offsetof(sockaddr_in, sin_port) == 2, "sin_port offset 2");
static_assert(offsetof(sockaddr_in6, sin6_port) == 2, "sin6_port offset 2");

InetAddress::InetAddress(uint16_t port, bool loopbackOnly, bool ipv6){
    static_assert(offsetof(InetAddress, m_addr6) == 0, "m_addr6 offset 0");     // offsetof宏定义，用来获取结构成员相对于结构开头的偏移量
    static_assert(offsetof(InetAddress, m_addr) == 0, "m_addr offset 0");       // 这里断言m_addr/m_addr6相对于类InetAddress偏移量为0

    if(ipv6){
        memset(&m_addr6, 0, sizeof(m_addr6));
        m_addr6.sin6_family = AF_INET6;

        in6_addr ip = loopbackOnly ? in6addr_loopback : in6addr_any;
        m_addr6.sin6_addr = ip;
        
        m_addr6.sin6_port = sockets::host_to_network16(port);
    }
    else{
        memset(&m_addr, 0, sizeof(m_addr));
        m_addr.sin_family = AF_INET;

        in_addr_t ip = loopbackOnly ? INADDR_LOOPBACK : INADDR_ANY;
        m_addr.sin_addr.s_addr = sockets::host_to_network32(ip);

        m_addr.sin_port = sockets::host_to_network16(port);
    }
}

InetAddress::InetAddress(const std::string& ip, uint16_t port, bool ipv6){
    if(ipv6){
        memset(&m_addr6, 0, sizeof(m_addr6));
        sockets::from_ip_and_port(ip.c_str(), port, &m_addr6);
    }
    else{
        memset(&m_addr, 0, sizeof(m_addr));
        sockets::from_ip_and_port(ip.c_str(), port, &m_addr);
    }
}

std::string InetAddress::to_ip() const {
    char buf[64] = "";
    sockets::to_ip(this->get_sockaddr(), buf, sizeof(buf));
    return buf;
}

std::string InetAddress::to_ip_and_port() const {
    char buf[64] = "";
    sockets::to_ip_and_port(this->get_sockaddr(), buf, sizeof(buf));
    return buf;
}

uint16_t InetAddress::to_port() const {
    return sockets::network_to_host16(this->get_portNetEndian());
}