#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <vector>
#include <queue> 
#include <atomic>
#include <functional>

#include <pthread.h>

class ThreadPool{
public:
    typedef std::function<void()> Task;

    ThreadPool();
    ~ThreadPool();

    struct ThreadItem{
        pthread_t ptid;
        std::atomic_bool isRunning;

        ThreadItem() : isRunning(false){}
    };

public:
    void create(size_t numThreads);

    void call(const Task& task);

    void stop();

private:
    static void * thread_entryFunc(void * arg);

private:
    static pthread_mutex_t m_mutex;
    static pthread_cond_t m_cond;

    static std::atomic<bool> m_stop;        // 线程池是否停止工作
    static std::atomic<int> m_runningNum;   // 记录正在运行的线程数，用于调整线程池大小

    static std::queue<Task> m_taskQue;      // 任务队列

    int m_threadNum;
    std::vector<ThreadItem *> m_threadVec;
};

#endif // THREADPOOL_H_