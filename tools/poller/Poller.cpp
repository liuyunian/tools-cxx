#include <stdlib.h>     // getenv

#include "tools/poller/Poller.h"
#include "tools/poller/PollPoller.h"
#include "tools/poller/EPollPoller.h"

Poller* Poller::create_poller(){
  if(::getenv("USE_POLL")){
    return new PollPoller();
  }
  else{                              
    return new EPollPoller();
  }
}