#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <vector>
#include <queue> 
#include <atomic>
#include <functional>

#include <pthread.h>

#include "tools/base/noncopyable.h"
#include "tools/base/CountDownLatch.h"

class ThreadPool : noncopyable{
public:
    typedef std::function<void()> Task;

    ThreadPool(size_t numThreads);
    
    ~ThreadPool();

public:
    void start();

    void call(const Task& task);

    void stop();

private:
    static void* thread_func(void* arg);

private:
    int m_threadNum;
    std::atomic<bool> m_stop;           // 线程池是否停止工作
    std::atomic<int> m_runningNum;      // 记录正在运行的线程数，用于调整线程池大小

    CountDownLatch m_latch;             // 倒计时，用主线程同步
    std::vector<pthread_t> m_threads;   // 记录创建的线程，用于join

    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;
    std::queue<Task> m_taskQue;         // 任务队列
};

#endif // THREADPOOL_H_