#include <tools/log/log.h>
#include <tools/socket/InetAddress.h>

int main(){
    // ipv4
    InetAddress addr(9000);                                 // ip = INADDR_ANY      port = 9000 
    InetAddress addr2(9001, InetAddress::IPv4, true);       // ip = INADDR_LOOPBACK port = 9001
    InetAddress addr3("10.0.0.1", 9002);                    // ip = 10.0.0.1        port = 9002

    LOG_INFO("addr = %p, ip = %s, port = %u", &addr, addr.to_ip().c_str(), addr.to_port());
    LOG_INFO("addr2 = %p, ip = %s, port = %u", &addr2, addr2.to_ip().c_str(), addr2.to_port());
    LOG_INFO("addr3 = %p, ip:port = %s", &addr3, addr3.to_ip_and_port().c_str());

    // ipv6

    return 0;
}