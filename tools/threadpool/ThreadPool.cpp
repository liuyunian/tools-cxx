#include <iostream>

#include <unistd.h>
#include <pthread.h>

#include "ThreadPool.h"
#include "tools/log/log.h"

ThreadPool::ThreadPool(size_t numThreads) : 
    m_threadNum(numThreads),
    m_stop(false),
    m_runningNum(0),
    m_latch(numThreads),
    m_threads(numThreads),
    m_mutex(PTHREAD_MUTEX_INITIALIZER),
    m_cond(PTHREAD_COND_INITIALIZER)
{}

ThreadPool::~ThreadPool(){
    stop();
}

void ThreadPool::start(){
    for(int i = 0; i < m_threadNum; ++ i){
        pthread_t ptid;
        pthread_create(&ptid, NULL, thread_func, this);
        m_threads.push_back(ptid);
    }

    m_latch.wait();
}

void ThreadPool::call(const Task& task){
    pthread_mutex_lock(&m_mutex);
    m_taskQue.push(task);
    pthread_mutex_unlock(&m_mutex);

    pthread_cond_signal(&m_cond);

    if(m_runningNum == m_threadNum){
        LOG_WARN("all thread is running! please think about increase thread numbers");
    }
}

void ThreadPool::stop(){
    if(m_stop){
        return;
    }
    m_stop = true;

    pthread_cond_broadcast(&m_cond);

    for(auto& ptid : m_threads){
        pthread_join(ptid, NULL);
    }
    m_threads.clear();
}

void* ThreadPool::thread_func(void* arg){
    ThreadPool* pool = static_cast<ThreadPool*>(arg);

    for(;;){
        pthread_mutex_lock(&(pool->m_mutex));
        while(!pool->m_stop && pool->m_taskQue.empty()){
            if(pool->m_latch.get_count() != 0){ // 计数器不为0，表示线程创建阶段
                pool->m_latch.count_down();
            }

            pthread_cond_wait(&(pool->m_cond), &(pool->m_mutex));
        }

        if(pool->m_stop){
            pthread_mutex_unlock(&(pool->m_mutex));
            break;
        }

        Task task = std::move(pool->m_taskQue.front());
        pool->m_taskQue.pop();
        pthread_mutex_unlock(&(pool->m_mutex));

        ++ pool->m_runningNum;
        task();
        -- pool->m_runningNum;
    }

    return (void *)0;
}