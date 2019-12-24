#include <algorithm>    // iter_swap
#include <assert.h>     // assert
#include <poll.h>       // poll

#include "tools/log/log.h"
#include "tools/poller/PollPoller.h"
#include "tools/poller/Channel.h"

Poller::ChannelList& PollPoller::poll(int timeoutMs){
  int numEvents = ::poll(m_pollfdList.data(), m_pollfdList.size(), timeoutMs);
  if(numEvents < 0){
    if(errno != EINTR){                            // EINTR错误不用报错
      LOG_SYSERR("PollPoller::poll()");
    }
  }
  else if(numEvents == 0){
    LOG_DEBUG("nothing happended");
  }
  else{
    fill_active_channels(numEvents);
  }

  return m_activeChannels;
}

void PollPoller::fill_active_channels(int numEvents){
  std::map<int, Channel*>::const_iterator iter;
  Channel *channel;
  m_activeChannels.clear();
  for(auto pollfd = m_pollfdList.begin(); pollfd != m_pollfdList.end() && numEvents > 0; ++ pollfd){
    if(pollfd->revents > 0){
      -- numEvents;

      iter = m_channelStore.find(pollfd->fd);
      assert(iter != m_channelStore.end());
      channel = iter->second;
      assert(channel->get_fd() == pollfd->fd);
      channel->set_revents(pollfd->revents);
      m_activeChannels.push_back(channel);
    }
  }
}

void PollPoller::update_channel(Channel *channel){
  int fd = channel->get_fd();

  // 在poll模式中index用来记录在pollfdList中的位置
  // index < 0表示是一个新Channel，也就是说这里的操作是添加
  if(channel->get_index() < 0){                                           
    assert(m_channelStore.find(fd) == m_channelStore.end());
    m_channelStore.insert({fd, channel});

    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = static_cast<short>(channel->get_events());
    pfd.revents = 0;
    m_pollfdList.push_back(pfd);

    channel->set_index(m_pollfdList.size()-1);
  }
  else{
    assert(m_channelStore.find(fd) != m_channelStore.end());
    assert(m_channelStore[fd] == channel);

    int index = channel->get_index();
    assert(index >= 0 && index < m_pollfdList.size());

    struct pollfd &pfd = m_pollfdList[index];
    assert(pfd.fd == channel->get_fd());
    pfd.events = static_cast<short>(channel->get_events());
    pfd.revents = 0;
  }
}

void PollPoller::remove_channel(Channel *channel){
  int fd = channel->get_fd();
  assert(m_channelStore.find(fd) != m_channelStore.end());
  assert(m_channelStore[fd] == channel);
  m_channelStore.erase(fd);

  int index = channel->get_index();
  assert(index >= 0 && index < m_pollfdList.size());
  struct pollfd &pfd = m_pollfdList[index];
  assert(pfd.fd == channel->get_fd());

  if(index == m_pollfdList.size()-1){                                 // m_pollfdList中的最后一个元素
    m_pollfdList.pop_back();
  }
  else{                                                               // 移除元素的时间复杂度为O(1)
    int channelAtEnd = m_pollfdList.back().fd;
    iter_swap(m_pollfdList.begin()+index, m_pollfdList.end()-1);      // 当前要移除的元素迭代器与最后一个元素迭代器交换
    m_channelStore[channelAtEnd]->set_index(index);
    m_pollfdList.pop_back();
  }

  channel->set_index(-1);
}
