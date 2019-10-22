#ifndef COUNTDOWNLATCH_H_
#define COUNTDOWNLATCH_H_

#include <mutex>
#include <condition_variable>

#include "tools/base/noncopyable.h"

class CountDownLatch : noncopyable {
public:
    explicit CountDownLatch(int count);

    CountDownLatch() = default;

    int get_count() const;                  // 获取计数器当前的值

    void wait();                            // 同步等待计数器减为0

    void countDown();                       // 计数器减1

private:
    mutable std::mutex m_mutex;
    std::condition_variable m_cond;
    int m_count;
};

#endif // COUNTDOWNLATCH_H_