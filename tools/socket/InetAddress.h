#ifndef INETADDRESS_H_
#define INETADDRESS_H_

#include <string>

#include <assert.h>
#include <netinet/in.h>

#include "tools/base/copyable.h"

class InetAddress : public copyable {
public:
  static const sa_family_t IPv4 = AF_INET;
  static const sa_family_t IPv6 = AF_INET6;

  explicit InetAddress(uint16_t port = 0, sa_family_t family = IPv4, bool loopbackOnly = false);

  InetAddress(const std::string &ip, uint16_t port, sa_family_t family = IPv4);

  explicit InetAddress(const struct sockaddr_in &addr) : 
    m_addr(addr){}

  explicit InetAddress(const struct sockaddr_in6 &addr) : 
    m_addr6(addr){}

  const struct sockaddr* get_sockaddr() const {
    return reinterpret_cast<const struct sockaddr*>(&m_addr6);
  }

  void set_sockaddr_in6(const struct sockaddr_in6 &addr6){
    m_addr6 = addr6;
  }

  sa_family_t get_family() const { 
    return m_addr.sin_family;
  }

  /*
    * @brief 获取网络序ip -- 限制于ipv4
    */
  uint32_t get_netendian_ip() const {
    assert(get_family() == AF_INET);
    return m_addr.sin_addr.s_addr;
  }

  /*
    * @brief 获取网络序port
    */
  uint16_t get_netendian_port() const {
    return m_addr.sin_port;
  }

  std::string to_ip() const;

  std::string to_ip_and_port() const;

  uint16_t to_port() const;

private:
  union{
    struct sockaddr_in m_addr;      // IPv4地址
    struct sockaddr_in6 m_addr6;    // IPv6地址
  };
};

#endif // INETADDRESS_H_