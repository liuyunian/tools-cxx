#ifndef POLLER_H_
#define POLLER_H_

#include <vector>
#include <map>

#include "tools/base/noncopyable.h"

class Channel;

class Poller : noncopyable {
public:
  Poller() = default;
  virtual ~Poller() = default;

  typedef std::vector<Channel*> ChannelList;
  virtual ChannelList& poll(int timeoutMs) = 0;

  virtual void update_channel(Channel *channel) = 0;

  virtual void remove_channel(Channel *channel) = 0;

  static Poller* create_poller();

protected:
  ChannelList m_activeChannels;
  std::map<int, Channel*> m_channelStore;
};

#endif // POLLER_H_