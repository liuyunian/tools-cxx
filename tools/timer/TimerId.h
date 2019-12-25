#ifndef TIMEIDR_H_
#define TIMEIDR_H_

#include <stdint.h> // int64_t

#include "tools/base/copyable.h"

class Timer;

class TimerId : copyable {
public:
  TimerId() : 
    m_timer(nullptr),
    m_sequence(0){}

  TimerId(Timer *timer, int64_t seq) : 
    m_timer(timer),
    m_sequence(seq){}

  // 采用默认的析构函数、拷贝构造函数、赋值运算符函数

  friend class TimerScheduler;    // 友元类，可以在TimerQueue类中访问TimeId类的private成员

private:
  Timer *m_timer;
  int64_t m_sequence;
};

#endif // TIMEIDR_H_