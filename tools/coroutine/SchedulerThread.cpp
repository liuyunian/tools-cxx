#include "tools/coroutine/SchedulerThread.h"

namespace co {

SchedulerThread::SchedulerThread() : 
  m_scheduler(nullptr)
{}

SchedulerThread::~SchedulerThread(){
  if(m_scheduler != nullptr){
    m_scheduler->quit();
    m_thread->join();
  }
}

Scheduler* SchedulerThread::start(){
  m_thread.reset(new std::thread(std::bind(&SchedulerThread::thread_func, this)));
  Scheduler *s = nullptr;
  {
    std::unique_lock<std::mutex> ul(m_mutex);
    while(m_scheduler == nullptr){
      m_cond.wait(ul);
    }

    s = m_scheduler;
  }

  return s;
}

void SchedulerThread::thread_func(){
  Scheduler s;

  {
    std::unique_lock<std::mutex> ul(m_mutex);
    m_scheduler = &s;
  }

  m_cond.notify_one();
  s.start();
}

} // namespace co