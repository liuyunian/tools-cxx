#include <iostream>

#include <unistd.h>
#include <pthread.h>

#include "ThreadPool.h"
#include "tools/log/log.h"

pthread_mutex_t ThreadPool::m_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ThreadPool::m_cond = PTHREAD_COND_INITIALIZER;

std::atomic<bool> ThreadPool::m_stop(false);        // 线程池是否停止工作
std::atomic<int> ThreadPool::m_runningNum(0);       // 记录正在运行的线程数
std::queue<ThreadPool::Task> ThreadPool::m_taskQue; // 任务队列

ThreadPool::ThreadPool() : 
    m_threadNum(0),
    m_threadVec(0){}

ThreadPool::~ThreadPool(){
    stop();
}

void ThreadPool::create(size_t numThreads){
    ThreadItem * newThread;
    
    m_threadNum = numThreads;
    m_threadVec.reserve(m_threadNum);
    for(int i = 0; i < m_threadNum; ++ i){
        newThread = new ThreadItem();
        m_threadVec.push_back(newThread);
        pthread_create(&newThread->ptid, NULL, thread_entryFunc, newThread);
    }

label:
    for(auto & thread : m_threadVec){   // 等待创建的线程都准备好
        if(!thread->isRunning){
            usleep(10 * 1000); 
            goto label;
        }
    }
}

void * ThreadPool::thread_entryFunc(void * arg){
    ThreadItem * thread = static_cast<ThreadItem *>(arg);

    for(;;){
        pthread_mutex_lock(&m_mutex);
        while(!m_stop && m_taskQue.empty()){
            if(!thread->isRunning){
                thread->isRunning = true;
            }

            pthread_cond_wait(&m_cond, &m_mutex);
        }

        if(m_stop){
            pthread_mutex_unlock(&m_mutex);
            break;
        }

        Task task = std::move(m_taskQue.front());
        m_taskQue.pop();
        pthread_mutex_unlock(&m_mutex);

        ++ m_runningNum;
        task();
        -- m_runningNum;
    }

    return (void *)0;
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

    for(auto & thread : m_threadVec){
        pthread_join(thread->ptid, NULL);
        delete thread;
    }
    m_threadVec.clear();
}