#include <iostream>

#include <string.h>         // memset
#include <assert.h>         // assert
#include <unistd.h>         // close
#include <sys/timerfd.h>    // timerfd_*
#include <sys/eventfd.h>    // eventfd

#include "tools/log/log.h"
#include "tools/timer/TimerScheduler.h"

__thread TimerScheduler *tTimerScheduler = nullptr;

const int kPollTimeMs = 10000;

static int create_timerfd(){
  int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
  if(timerfd < 0){
    LOG_SYSFATAL("Failed to create timerfd in create_timerfd()");
  }

  return timerfd;
}

static int create_eventfd(){
  int eventfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if(eventfd < 0){
    LOG_SYSFATAL("Failed to create eventfd in create_eventfd()");
  }

  return eventfd;
}

static struct timespec get_time_from_now(Timestamp when){
  int64_t microseconds = when.get_difference(Timestamp::now());
  if(microseconds < 100){ // 间隔不足100us，按100us算
    microseconds = 100;
  }

  struct timespec ts;
  ts.tv_sec = static_cast<time_t>(microseconds / Timestamp::kmicrosecondsPerSecond);
  ts.tv_nsec = static_cast<long>((microseconds % Timestamp::kmicrosecondsPerSecond) * 1000);

  return ts;
}

static void reset_timerfd(int timerfd, Timestamp expiration){
  struct itimerspec newValue;
  struct itimerspec oldValue;
  memset(&newValue, 0, sizeof(newValue));
  memset(&oldValue, 0, sizeof(oldValue));

  newValue.it_value = get_time_from_now(expiration);
  if(::timerfd_settime(timerfd, 0, &newValue, &oldValue) < 0){
    LOG_SYSFATAL("Failed to reset timerfd in reset_timerfd()");
  }
}

static void read_timerfd(int timerfd){
  uint64_t howmany;
  ssize_t n = ::read(timerfd, &howmany, sizeof(howmany));
  if(n != sizeof(howmany)){
    LOG_SYSERR("read_timerfd error");
  }
}

TimerScheduler::TimerScheduler() : 
  m_tid(CurrentThread::get_tid()),
  m_quit(false),
  m_poller(Poller::create_poller()),
  m_timerfd(create_timerfd()),
  m_timerfdChannel(m_poller.get(), m_timerfd),
  m_callingExpiredTimer(false),
  m_eventfd(create_eventfd()),
  m_eventfdChannel(m_poller.get(), m_eventfd)
{
  assert(tTimerScheduler == nullptr);               // 确保一个线程中只有一个TimerScheduler对象
  tTimerScheduler = this;

  m_timerfdChannel.set_read_callback(std::bind(&TimerScheduler::handle_event, this));
  m_timerfdChannel.enable_reading();

  m_eventfdChannel.set_read_callback(std::bind(&TimerScheduler::read_eventfd, this));
  m_eventfdChannel.enable_reading();
}

TimerScheduler::~TimerScheduler(){
  close(m_timerfd);
  close(m_eventfd);
  
  for(auto &timerEntry : m_timers){
    delete timerEntry.second;
  }
}

void TimerScheduler::start(){
  m_quit = false;
  while(!m_quit){
    Poller::ChannelList activeChannels = m_poller->poll(kPollTimeMs);
    for(auto &channelPtr : activeChannels){
      channelPtr->handle_event();
    }

    handle_pending_functor();
  }
}

void TimerScheduler::quit(){
  m_quit = true;

  if(!is_in_timerQueue_thread()){
    write_eventfd();
  }
}

TimerId TimerScheduler::run_at(const Timestamp time, const TimerCallback &cb){
  return add_timer(cb, time, 0);
}

TimerId TimerScheduler::run_after(double delay, const TimerCallback &cb){
  return run_at(Timestamp::now().add_time(delay), cb);
}

TimerId TimerScheduler::run_every(double interval, const TimerCallback &cb){
  return add_timer(cb, Timestamp::now().add_time(interval), interval);
}

void TimerScheduler::cancel(TimerId timerId){
  if(is_in_timerQueue_thread()){
    cancel_timer(timerId);
  }
  else{
    {
      std::unique_lock<std::mutex> ul(m_mutex);
      m_pendingFunctors.push_back(std::bind(&TimerScheduler::cancel_timer, this, timerId));
    }

    write_eventfd();
  }
}

TimerId TimerScheduler::add_timer(const TimerCallback &cb, Timestamp when, double interval){
  Timer *timer = new Timer(std::move(cb), when, interval);

  if(is_in_timerQueue_thread()){
    insert(timer);
  }
  else{
    {
      std::unique_lock<std::mutex> ul(m_mutex);
      m_pendingFunctors.push_back(std::bind(&TimerScheduler::insert, this, timer));
    }

    write_eventfd();
  }

  return TimerId(timer, timer->get_sequence());
}

void TimerScheduler::cancel_timer(TimerId timerId){
  assert(m_timers.size() == m_activeTimers.size());

  ActiveTimerEntry entry(timerId.m_timer, timerId.m_sequence);
  auto iter = m_activeTimers.find(entry);
  if(iter != m_activeTimers.end()){
    m_timers.erase(TimerEntry(iter->first->get_expiration(), iter->first));

    delete iter->first;
    m_activeTimers.erase(iter);
  }
  else if(m_callingExpiredTimer){
    m_cancelingTimers.insert(entry);
  }

  assert(m_timers.size() == m_activeTimers.size());
}

void TimerScheduler::insert(Timer *timer){
  assert(m_timers.size() == m_activeTimers.size());

  Timestamp when = timer->get_expiration();
  auto iter = m_timers.begin();
  if(iter == m_timers.end() || when < iter->first){
    reset_timerfd(m_timerfd, timer->get_expiration());
  }

  {
    std::pair<TimerSet::iterator, bool> result = m_timers.insert(TimerEntry(when, timer));
    assert(result.second);
  }

  {
    std::pair<ActiveTimerSet::iterator, bool> result = m_activeTimers.insert(ActiveTimerEntry(timer, timer->get_sequence()));
    assert(result.second);
  }

  assert(m_timers.size() == m_activeTimers.size());
}

void TimerScheduler::handle_event(){
  Timestamp now(Timestamp::now());

  read_timerfd(m_timerfd);

  std::vector<TimerEntry> expiredTimers = get_expired(now);

  m_callingExpiredTimer = true;
  m_cancelingTimers.clear();
  for(auto &timerEntry : expiredTimers){
    timerEntry.second->run();
  }
  m_callingExpiredTimer = false;

  reset(expiredTimers, now);
}

std::vector<TimerScheduler::TimerEntry> TimerScheduler::get_expired(Timestamp now){
  assert(m_timers.size() == m_activeTimers.size());
  std::vector<TimerEntry> expiredTimers;

  TimerEntry sentry(now, reinterpret_cast<Timer*>(UINTPTR_MAX));      // 关键在于时间戳
  auto end = m_timers.lower_bound(sentry);
  assert(end == m_timers.end() || now < end->first);
  std::copy(m_timers.begin(), end, back_inserter(expiredTimers));
  m_timers.erase(m_timers.begin(), end);

  for(auto &timerEntry : expiredTimers){
    ActiveTimerEntry entry(timerEntry.second, timerEntry.second->get_sequence());
    m_activeTimers.erase(entry);
  }

  assert(m_timers.size() == m_activeTimers.size());
  return expiredTimers;
}

void TimerScheduler::reset(const std::vector<TimerEntry> &expiredTimers, Timestamp now){
  for(auto &timerEntry : expiredTimers){
    ActiveTimerEntry entry(timerEntry.second, timerEntry.second->get_sequence());
    if(timerEntry.second->is_repeated() && m_cancelingTimers.find(entry) == m_cancelingTimers.end()){
      timerEntry.second->restart(now);
      insert(timerEntry.second);
    }
    else{
      delete timerEntry.second;
    }
  }

  Timestamp newExpire;
  if(!m_timers.empty()){
    newExpire = m_timers.begin()->second->get_expiration();
  }

  if(newExpire.is_valid()){
    reset_timerfd(m_timerfd, newExpire);
  }
}

void TimerScheduler::handle_pending_functor(){
  std::vector<Functor> functors;

  {
    std::unique_lock<std::mutex> ul(m_mutex);
    functors.swap(m_pendingFunctors);
  }

  for(auto &functor : functors){
    functor();
  }
}

void TimerScheduler::read_eventfd(){
  uint64_t one;
  ssize_t n = ::read(m_eventfd, &one, sizeof(one));
  if(n != sizeof(one)){
    LOG_SYSERR("read_eventfd error");
  }
}

void TimerScheduler::write_eventfd(){
  uint64_t one = 1;
  ssize_t n = ::write(m_eventfd, &one, sizeof(one));
  if(n != sizeof(one)){
    LOG_SYSERR("write_eventfd error");
  }
}