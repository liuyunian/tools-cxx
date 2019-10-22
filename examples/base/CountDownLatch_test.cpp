#include <thread>

#include <unistd.h>

#include <tools/log/log.h>
#include <tools/base/CurrentThread.h>
#include <tools/base/CountDownLatch.h>

int main(){
    CountDownLatch cdl(2);

    std::thread t1([&cdl]{
        LOG_INFO("thread t1 start, pid = %d, tid = %d", getpid(), CurrentThread::get_tid());
        std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // sleep 2s
        LOG_INFO("thread t1 stop, pid = %d, tid = %d", getpid(), CurrentThread::get_tid());

        cdl.countDown();
    });

    std::thread t2([&cdl]{
        LOG_INFO("thread t2 start, pid = %d, tid = %d", getpid(), CurrentThread::get_tid());
        std::this_thread::sleep_for(std::chrono::milliseconds(5000)); // sleep 2s
        LOG_INFO("thread t2 stop, pid = %d, tid = %d", getpid(), CurrentThread::get_tid());

        cdl.countDown();
    });

    cdl.wait();
    LOG_INFO("main, pid = %d, tid = %d", getpid(), CurrentThread::get_tid());

    t1.join();
    t2.join();

    return 0;
}