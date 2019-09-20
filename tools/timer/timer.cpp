#include "timer.h"

Timer::Timer() : 
    m_bExpired(true), 
    m_bTryExpired(false), 
    m_bLoop(false),
    m_thread(nullptr){}

Timer::~Timer(){
    m_bTryExpired = true;
    delete_thread();
}

bool Timer::start_timer(unsigned int timeoutMs, std::function<void()> task, bool b_loop){
    if(!m_bExpired || m_bTryExpired){
        return false;                                                               // 任务未过期(即内部仍在存在或正在运行任务)
    }

    m_bExpired = false;
    m_bLoop = b_loop;
    
    delete_thread();                                                                // 再次执行任务时不再使用原来的线程
    m_thread = new std::thread([this, timeoutMs, task](){                           // lambda表达式作为线程的执行函数
        while(!m_bTryExpired) {
            m_condition.wait_for(m_mutex, std::chrono::milliseconds(timeoutMs));    // 休眠
            if (!m_bTryExpired) {
                task();                                                             // 执行任务回调函数
                
                if (!m_bLoop) {
                    break;
                }
            }
        }
        
        m_bExpired = true;                                                          // 任务执行完成(表示已有任务已过期)
        m_bTryExpired = false;                                                      // 为了下次再次装载任务
    });
    
    return true;
}

void Timer::cancel_timer(){
    if(m_bExpired || m_bTryExpired || !m_thread){
        return;
    }
    
    m_bTryExpired = true;
    delete_thread();
}

void Timer::delete_thread(){
    if(m_thread){
        m_condition.notify_one();   //休眠唤醒
        m_thread->join();           //等待线程退出
        delete m_thread;
        m_thread = nullptr;
    }
}