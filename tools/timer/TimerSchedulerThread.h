#ifndef TIMER_SCHEDULER_THREAD_H_
#define TIMER_SCHEDULER_THREAD_H_

#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "tools/base/noncopyable.h"
#include "tools/timer/TimerScheduler.h"

class TimerSchedulerThread : noncopyable {
public:
  TimerSchedulerThread();
  ~TimerSchedulerThread();

  TimerScheduler* start();

private:
  void thread_func();

private:
  TimerScheduler *m_scheduler;
  std::unique_ptr<std::thread> m_thread;
  std::mutex m_mutex;
  std::condition_variable m_cond;
};

#endif // TIMER_SCHEDULER_THREAD_H_