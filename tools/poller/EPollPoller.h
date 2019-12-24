#ifndef EPOLLPOLLER_H_
#define EPOLLPOLLER_H_

#include <vector>

#include <sys/epoll.h> // epoll_event

#include "tools/poller/Poller.h"

class Channel;

class EPollPoller : public Poller {
public:
  EPollPoller();

  virtual ~EPollPoller();

  virtual ChannelList& poll(int timeoutMs) override;
  virtual void update_channel(Channel *channel) override;
  virtual void remove_channel(Channel *channel) override;

private:
  void fill_active_channels(int numEvents);
  void update(int op, Channel *channel);

private:
  int m_epfd;
  std::vector<struct epoll_event> m_eventList;
};

#endif // EPOLLPOLLER_H_