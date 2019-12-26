#ifndef TIMERSCHEDULER_H_
#define TIMERSCHEDULER_H_

#include <set>
#include <mutex>
#include <vector>
#include <memory>
#include <atomic>

#include "tools/base/noncopyable.h"
#include "tools/base/Timestamp.h"
#include "tools/base/CurrentThread.h"
#include "tools/poller/Poller.h"
#include "tools/poller/Channel.h"
#include "tools/timer/Timer.h"
#include "tools/timer/TimerId.h"

class TimerScheduler : noncopyable {
public:
  TimerScheduler();
  ~TimerScheduler();

  void start();

  void quit();

  TimerId run_at(const Timestamp time, const TimerCallback &cb);

  TimerId run_after(double delay, const TimerCallback &cb);

  TimerId run_every(double interval, const TimerCallback &cb);

  void cancel(TimerId timerId);

private:
  typedef std::pair<Timestamp, Timer*> TimerEntry;
  typedef std::set<TimerEntry> TimerSet;

  typedef std::pair<Timer*, int64_t> ActiveTimerEntry;
  typedef std::set<ActiveTimerEntry> ActiveTimerSet;

  bool is_in_timerQueue_thread() const {
    return m_tid == CurrentThread::get_tid();
  }

  TimerId add_timer(const TimerCallback &cb, Timestamp when, double interval);

  void cancel_timer(TimerId timerId);

  void insert(Timer *timer);

  void handle_event();

  std::vector<TimerEntry> get_expired(Timestamp now);

  void reset(const std::vector<TimerEntry> &expiredTimers, Timestamp now);

  void handle_pending_functor();

  void read_eventfd();

  void write_eventfd();

private:
  const int m_tid;                        // 所属线程的tid
  std::atomic<bool> m_quit;               // 停止定时

  std::unique_ptr<Poller> m_poller;
  const int m_timerfd;
  Channel m_timerfdChannel;

  bool m_callingExpiredTimer;             // 是否正在调用过期的定时器
  TimerSet m_timers;
  ActiveTimerSet m_activeTimers;
  ActiveTimerSet m_cancelingTimers;

  const int m_eventfd;
  Channel m_eventfdChannel;
  std::mutex m_mutex;
  typedef std::function<void()> Functor;
  std::vector<Functor> m_pendingFunctors;
};

#endif // TIMERSCHEDULER_H_