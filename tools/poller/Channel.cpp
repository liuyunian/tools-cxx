#include <poll.h>

#include "tools/poller/Channel.h"

const int Channel::k_readEvent = POLLIN | POLLPRI;
const int Channel::k_writeEvent = POLLOUT;

Channel::Channel(int fd) : 
  m_fd(fd),
  m_events(0),
  m_revents(0),
  m_index(-1)
{}