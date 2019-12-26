#include <poll.h>       // EPOLLXXX
#include <sys/epoll.h>  // POLLXXX

#include "tools/log/log.h"
#include "tools/poller/Channel.h"

// On Linux, the constants of poll(2) and epoll(4) are expected to be the same.
static_assert(EPOLLIN == POLLIN,        "epoll uses same flag values as poll");
static_assert(EPOLLPRI == POLLPRI,      "epoll uses same flag values as poll");
static_assert(EPOLLOUT == POLLOUT,      "epoll uses same flag values as poll");
static_assert(EPOLLRDHUP == POLLRDHUP,  "epoll uses same flag values as poll");
static_assert(EPOLLERR == POLLERR,      "epoll uses same flag values as poll");
static_assert(EPOLLHUP == POLLHUP,      "epoll uses same flag values as poll");

const int Channel::READ_EVENT = POLLIN | POLLPRI | POLLRDHUP;
const int Channel::WRITE_EVENT = POLLOUT;
const int Channel::ERROR_EVENT = POLLERR;
const int Channel::CLOSE_EVENT = POLLHUP | (!POLLIN);

const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(int fd) : 
  m_fd(fd),
  m_events(0),
  m_revents(0),
  m_index(-1)
{}

void Channel::handle_event(){
  if(m_revents & READ_EVENT){
    LOG_DEBUG("fd = %d read event", m_fd);

    if(m_readCallback) m_readCallback();
  }

  if(m_revents & WRITE_EVENT){
    LOG_DEBUG("fd = %d write event", m_fd);

    if(m_writeCallback) m_writeCallback();
  }

  if(m_revents & ERROR_EVENT){
    LOG_DEBUG("fd = %d error event", m_fd);

    if(m_errorCallback) m_errorCallback();
  }

  if(m_revents & POLLNVAL){
    LOG_WARN("fd = %d error event POLLNVAL", m_fd);
    
    if(m_errorCallback) m_errorCallback();
  }

  if(m_revents & CLOSE_EVENT){
    LOG_DEBUG("fd = %d close event", m_fd);

    if(m_closeCallback) m_closeCallback();
  }
}