#include <stdlib.h>     // getenv
#include <poll.h>       // EPOLLXXX
#include <sys/epoll.h>  // POLLXXX

#include "tools/poller/Poller.h"
#include "tools/poller/PollPoller.h"
#include "tools/poller/EPollPoller.h"

// On Linux, the constants of poll(2) and epoll(4) are expected to be the same.
static_assert(EPOLLIN == POLLIN,        "epoll uses same flag values as poll");
static_assert(EPOLLPRI == POLLPRI,      "epoll uses same flag values as poll");
static_assert(EPOLLOUT == POLLOUT,      "epoll uses same flag values as poll");
static_assert(EPOLLRDHUP == POLLRDHUP,  "epoll uses same flag values as poll");
static_assert(EPOLLERR == POLLERR,      "epoll uses same flag values as poll");
static_assert(EPOLLHUP == POLLHUP,      "epoll uses same flag values as poll");

const int Poller::READ_EVENT = POLLIN | POLLPRI | POLLRDHUP;
const int Poller::WRITE_EVENT = POLLOUT;
const int Poller::ERROR_EVENT = POLLERR;
const int Poller::CLOSE_EVENT = POLLHUP | (!POLLIN);

Poller* Poller::new_default_Poller(){
  if(::getenv("USE_POLL")){
    return new PollPoller();
  }
  else{                              
    return new EPollPoller();
  }
}