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
  virtual void poll(int timeoutMs, ChannelList *activeChannels) = 0;

  virtual void update_channel(Channel *channel) = 0;

  virtual void remove_channel(Channel *channel) = 0;

  static Poller* new_default_Poller();

  static const int READ_EVENT;
  static const int WRITE_EVENT;
  static const int ERROR_EVENT;
  static const int CLOSE_EVENT;

protected:
  std::map<int, Channel*> m_channelStore;
};

#endif // POLLER_H_