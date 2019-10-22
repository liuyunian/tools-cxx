#include "tools/base/CountDownLatch.h"

CountDownLatch::CountDownLatch(int count) : m_count(count){}

int CountDownLatch::get_count() const {
    std::unique_lock<std::mutex> ul(m_mutex);
    return m_count;
}

void CountDownLatch::wait(){
    std::unique_lock<std::mutex> ul(m_mutex);
    while(m_count > 0){
        m_cond.wait(ul);
    }
}

void CountDownLatch::countDown(){
    std::unique_lock<std::mutex> ul(m_mutex);
    -- m_count;
    if(m_count == 0){
        m_cond.notify_all();
    }
}