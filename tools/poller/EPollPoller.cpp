#include <unistd.h>     // close
#include <string.h>     // memset
#include <assert.h>     // assert
#include <poll.h>
#include <sys/epoll.h>

#include <tools/log/log.h>
#include <tools/base/Timestamp.h>

#include "tools/poller/EPollPoller.h"
#include "tools/poller/Channel.h"

const int kNew = -1;            // 新的Channel
const int kAdded = 1;           // 已经添加过的Channel
const int kEventListSize = 16;  // eventlist初始化大小

EPollPoller::EPollPoller() : 
  m_epfd(::epoll_create1(EPOLL_CLOEXEC)),
  m_eventList(kEventListSize)
{
  if(m_epfd < 0){
    LOG_SYSFATAL("Failed to created epfd in EPollPoller::EPollPoller()");
  }
}

EPollPoller::~EPollPoller(){
  close(m_epfd);
}

Poller::ChannelList& EPollPoller::poll(int timeoutMs){
  m_activeChannels.clear();
  int numEvents = ::epoll_wait(m_epfd, m_eventList.data(), m_eventList.size(), timeoutMs);
  if(numEvents < 0){
    if(errno != EINTR){                            // EINTR错误不用报错
      LOG_SYSERR("EPollPoller::poll()");
    }
  }
  else if(numEvents == 0){
    LOG_DEBUG("nothing happended");
  }
  else{
    fill_active_channels(numEvents);

    if(numEvents == m_eventList.size()){
      m_eventList.resize(m_eventList.size()*2);
    }
  }

  return m_activeChannels;
}

void EPollPoller::fill_active_channels(int numEvents){
  assert(numEvents <= m_eventList.size());

  Channel *channel;
  std::map<int, Channel*>::const_iterator iter;
  for(int i = 0; i < numEvents; ++ i){
      channel = static_cast<Channel*>(m_eventList[i].data.ptr);
      iter = m_channelStore.find(channel->get_fd());
      assert(iter != m_channelStore.end());
      assert(iter->second == channel);

      channel->set_revents(m_eventList[i].events);
      m_activeChannels.push_back(channel);
  }
}

void EPollPoller::update_channel(Channel *channel){
  /**
   * Channel的成员变量index用来区别该Channel对象EPollPoller的状态
   * kNew   -- 没有添加关注过，此时updata_channel表示添加关注的事件
   * kAdded -- 已经添加关注过，此时updata_channel表示修改关注的事件
  */ 
  const int index = channel->get_index();
  int fd = channel->get_fd();
  if(index == kNew){
    assert(m_channelStore.find(fd) == m_channelStore.end());
    m_channelStore.insert({fd, channel});
    channel->set_index(kAdded);
    update(EPOLL_CTL_ADD, channel);
  }
  else{ 
    assert(index == kAdded);
    assert(m_channelStore.find(fd) != m_channelStore.end());
    assert(m_channelStore[fd] == channel);

    update(EPOLL_CTL_MOD, channel);
  }
}

void EPollPoller::remove_channel(Channel *channel){
  int index = channel->get_index();
  int fd = channel->get_fd();
  assert(index == kAdded);
  assert(m_channelStore.find(fd) != m_channelStore.end());
  assert(m_channelStore[fd] == channel);
  m_channelStore.erase(fd);

  update(EPOLL_CTL_DEL, channel);

  channel->set_index(kNew);
}

void EPollPoller::update(int op, Channel *channel){
  struct epoll_event event;
  memset(&event, 0, sizeof(event));
  event.events = channel->get_events();
  event.data.ptr = channel;
  int fd = channel->get_fd();

  if(::epoll_ctl(m_epfd, op, fd, &event) < 0){
    if(op == EPOLL_CTL_DEL){
      LOG_SYSERR("EPollPoller::poll()");
    }
    else{
      LOG_SYSFATAL("EPollPoller::poll()");
    }
  }
}