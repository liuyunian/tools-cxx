#include <unistd.h>

#include <tools/log/log.h>
#include <tools/base/CurrentThread.h>
#include <tools/threadpool/ThreadPool.h>

void func1(){
    LOG_INFO("test thread func1 pid = %d, tid = %d", getpid(), CurrentThread::get_tid());
}

void func2(int num){
    LOG_INFO("test thread func2, num = %d, pid = %d, tid = %d", num, getpid(), CurrentThread::get_tid());
}

int main(){
    ThreadPool tp(5);
    tp.start();

    for(int i = 15; i > 0; -- i){
        if(i % 2 == 0){
            tp.call(func1);
        }
        else{
            tp.call(std::bind(func2, i));
        }
    }

    sleep(2);
    
    return 0;
}