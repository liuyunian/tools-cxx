#include "tools/coroutine/CoSchedulerThread.h"

CoSchedulerThread::CoSchedulerThread() : 
  m_scheduler(nullptr)
{}

CoSchedulerThread::~CoSchedulerThread(){
  if(m_scheduler != nullptr){
    m_scheduler->quit();
    m_thread->join();
  }
}

CoScheduler* CoSchedulerThread::start(){
  m_thread.reset(new std::thread(std::bind(&CoSchedulerThread::thread_func, this)));
  CoScheduler *s = nullptr;
  {
    std::unique_lock<std::mutex> ul(m_mutex);
    while(m_scheduler == nullptr){
      m_cond.wait(ul);
    }

    s = m_scheduler;
  }

  return s;
}

void CoSchedulerThread::thread_func(){
  CoScheduler s;

  {
    std::unique_lock<std::mutex> ul(m_mutex);
    m_scheduler = &s;
  }

  m_cond.notify_one();
  s.start();
}