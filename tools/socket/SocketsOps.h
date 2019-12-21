#ifndef SOCKETSOPS_H_
#define SOCKETSOPS_H_

#include <arpa/inet.h>  // sockaddr

namespace sockets {
// core
int create_socket(int family);
int create_nonblocking_socket(int family);

void close(int sockfd);

// server
void bind(int sockfd, const struct sockaddr *addr);

void listen(int sockfd);

int accept(int sockfd);
int accept(int sockfd, struct sockaddr_in6 *addr);

int accept_nonblocking(int sockfd);
int accept_nonblocking(int sockfd, struct sockaddr_in6 *addr);

// client
void connect(int sockfd, const struct sockaddr *addr);

// other
void set_reuse_address(int sockfd, bool on);
void set_reuse_port(int sockfd, bool on);
void set_keep_alive(int sockfd, bool on);
void set_no_delay(int sockfd, bool on);

void to_ip(const struct sockaddr *addr, char *buf, size_t size);
void to_ip_and_port(const struct sockaddr *addr, char *buf, size_t size);

void from_ip_and_port(const char *ip, uint16_t port, struct sockaddr_in *addr);
void from_ip_and_port(const char *ip, uint16_t port, struct sockaddr_in6 *addr);

struct sockaddr_in6 get_local_addr(int sockfd);
struct sockaddr_in6 get_peer_addr(int sockfd);

} // namespace sockets

#endif // SOCKETSOPS_H_