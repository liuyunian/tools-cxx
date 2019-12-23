#include <vector>
#include <map>

#include <string.h>     // memset

#include <tools/log/log.h>
#include <tools/base/Exception.h>
#include <tools/socket/Socket.h>
#include <tools/socket/SocketsOps.h>
#include <tools/socket/InetAddress.h>
#include <tools/socket/ServerSocket.h>
#include <tools/poller/Channel.h>
#include <tools/poller/Poller.h>

#define LISTEN_PORT 9000
#define BUFFER_SZ 1024

int main(){
  ServerSocket ss(sockets::create_nonblocking_socket(AF_INET));
  InetAddress addr(LISTEN_PORT);
  ss.set_reuse_address(true);
  ss.bind(addr);
  ss.listen();
  LOG_INFO("server is listening...");

  Channel listenChannel(ss.get_sockfd());
  listenChannel.enable_reading();

  Poller *poller = Poller::new_default_Poller();
  poller->update_channel(&listenChannel);

  std::map<Channel*, Socket> connPool;
  int nready;
  char buf[BUFFER_SZ];
  ssize_t len = 0;

  Poller::ChannelList activeChannel;
  for(;;){
    activeChannel.clear();
    poller->poll(-1, &activeChannel);

    for(auto &channel : activeChannel){
      if(!(channel->get_revents() & Poller::READ_EVENT)){
        continue;
      }

      if(channel->get_fd() == ss.get_sockfd()){
        try{
          Socket connSocket = ss.accept_nonblocking(nullptr);
          Channel *connChannel = new Channel(connSocket.get_sockfd());
          connChannel->enable_reading();
          poller->update_channel(connChannel);
          connPool.insert({connChannel, connSocket});
        }
        catch(const Exception &e){
          LOG_WARN("accept error");
        }
      }
      else{
        auto connSocketIter = connPool.find(channel);
        len = connSocketIter->second.read(buf, BUFFER_SZ);
        if(len < 0){
          LOG_WARN("read error");
        }
        else if(len == 0){
          LOG_INFO("client disconnects");
          poller->remove_channel(connSocketIter->first);
          delete connSocketIter->first;
          connPool.erase(connSocketIter);
        }
        else{
          connSocketIter->second.write(buf, len);
          memset(buf, 0, BUFFER_SZ);
        }
      }
    }
  }

  return 0;
}