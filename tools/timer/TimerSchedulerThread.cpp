#include "tools/timer/TimerSchedulerThread.h"

TimerSchedulerThread::TimerSchedulerThread() : 
  m_scheduler(nullptr)
{}

TimerSchedulerThread::~TimerSchedulerThread(){
  if(m_scheduler != nullptr){
    m_scheduler->quit();
    m_thread->join();
  }
}

TimerScheduler* TimerSchedulerThread::start(){
  m_thread.reset(new std::thread(std::bind(&TimerSchedulerThread::thread_func, this)));
  TimerScheduler *s = nullptr;
  {
    std::unique_lock<std::mutex> ul(m_mutex);
    while(m_scheduler == nullptr){
      m_cond.wait(ul);
    }

    s = m_scheduler;
  }

  return s;
}

void TimerSchedulerThread::thread_func(){
  TimerScheduler s;

  {
    std::unique_lock<std::mutex> ul(m_mutex);
    m_scheduler = &s;
  }

  m_cond.notify_one();
  s.start();
}