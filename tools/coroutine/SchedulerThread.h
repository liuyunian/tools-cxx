#ifndef SCHEDULER_THREAD_H_
#define SCHEDULER_THREAD_H_

#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "tools/base/noncopyable.h"
#include "tools/coroutine/Scheduler.h"

namespace co {

class SchedulerThread : noncopyable {
public:
  SchedulerThread();
  ~SchedulerThread();

  Scheduler* start();

private:
  void thread_func();

private:
  Scheduler *m_scheduler;
  std::unique_ptr<std::thread> m_thread;
  std::mutex m_mutex;
  std::condition_variable m_cond;
};

} // namespace co

#endif // SCHEDULER_THREAD_H_