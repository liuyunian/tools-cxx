#include <iostream>

#include <signal.h>
#include <unistd.h>

void startTimer(int timeoutS, void (*handler)(int)){
    signal(SIGALRM, handler);
    alarm(timeoutS);
}

void cancalTimer(){
    alarm(0);
}

void timeEvent(int signo){
    std::cout << "on time" << std::endl;
}

int main(){
    startTimer(5, timeEvent);                   // 定时5秒

    int i = 10;
    while(i > 0){                               // 主线程休眠10s
        std::cout << "sleep 1s" << std::endl;;
        sleep(1);
        -- i;

        // cancalTimer();                       // 可以取消定时
    }

    return 0;
}

