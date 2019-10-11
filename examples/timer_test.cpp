#include <iostream>

#include <unistd.h>

#include <tools/timer/timer.h>

void timerEvent(Timer * timer){
    std::cout << "timer: " << timer << " on time" << std::endl;
}

int main(){
    Timer * timer1 = new Timer();
    Timer * timer2 = new Timer();

    timer1->async_once(1000, timerEvent, timer1);               // 单次定时，时间到之后执行timerEvent

    bool ret = timer1->async_once(5000, timerEvent, timer1);    // 再次装填定时任务，但是由于定时器1非空闲，所以执行失败
    if(!ret){
        std::cout << "Failed to start timer" << std::endl;
    }

    sleep(2);                                                   // 休眠2s，保证定时器1任务执行完成

    ret = timer1->async_once(5000, timerEvent, timer1);         // 再次装填定时任务，此时会执行成功
    if(!ret){
        std::cout << "Failed to start timer" << std::endl;
    }

    timer2->async_loop(1000, timerEvent, timer2);               // 循环定时，每隔5秒执行timerEvent

    sleep(10);                                                  // 主线程休眠10秒之后退出

    timer1->cancel_timer();                                     // 可以根据需要取消定时
    timer2->cancel_timer();
    delete timer1;
    delete timer2;

    return 0;
}