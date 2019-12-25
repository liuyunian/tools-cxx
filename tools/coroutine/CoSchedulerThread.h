#ifndef CO_SCHEDULER_THREAD_H_
#define CO_SCHEDULER_THREAD_H_

#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "tools/base/noncopyable.h"
#include "tools/coroutine/CoScheduler.h"

class CoSchedulerThread : noncopyable {
public:
  CoSchedulerThread();
  ~CoSchedulerThread();

  CoScheduler* start();

private:
  void thread_func();

private:
  CoScheduler *m_scheduler;
  std::unique_ptr<std::thread> m_thread;
  std::mutex m_mutex;
  std::condition_variable m_cond;
};

#endif // CO_SCHEDULER_THREAD_H_