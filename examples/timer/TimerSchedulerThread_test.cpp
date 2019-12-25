#include <iostream>

#include <unistd.h> // sleep

#include <tools/base/Timestamp.h>
#include <tools/base/CurrentThread.h>

#include <tools/timer/TimerId.h>
#include <tools/timer/TimerScheduler.h>
#include <tools/timer/TimerSchedulerThread.h>

int cnt = 0;
TimerScheduler *gScheduler;

void print(const char* msg){
  std::cout << "now: " << Timestamp::now().to_formatted_string() << " msg: " << msg << " tid = " << CurrentThread::get_tid() << std::endl;

  ++ cnt;
  if(cnt == 20){
    gScheduler->quit();
  }
}

void cancel(TimerId timer){
  gScheduler->cancel(timer);
  std::cout << "cancelled at " << Timestamp::now().to_formatted_string() << std::endl;
}

int main(){
  TimerSchedulerThread tst;
  gScheduler = tst.start();

  print("main");
  gScheduler->run_after(1, std::bind(print, "once1"));
  gScheduler->run_after(1.5, std::bind(print, "once1.5"));
  gScheduler->run_after(2.5, std::bind(print, "once2.5"));
  gScheduler->run_after(3.5, std::bind(print, "once3.5"));
  TimerId t45 = gScheduler->run_after(4.5, std::bind(print, "once4.5"));
  gScheduler->run_after(4.2, std::bind(cancel, t45));
  gScheduler->run_after(4.8, std::bind(cancel, t45));
  gScheduler->run_every(2, std::bind(print, "every2"));
  TimerId t3 = gScheduler->run_every(3, std::bind(print, "every3"));
  gScheduler->run_after(9.001, std::bind(cancel, t3));


  sleep(30);
}