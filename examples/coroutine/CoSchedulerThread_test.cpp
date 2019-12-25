#include <iostream>

#include <unistd.h> // sleep

#include <tools/coroutine/CoSchedulerThread.h>

void test_func(CoScheduler *csd){
  std::cout << "test_func start tid = " << CurrentThread::get_tid() << "\n";
  csd->yield();
  std::cout << "test_func end tid = " << CurrentThread::get_tid() << std::endl;

}

void create_in_co(CoScheduler *csd){
  std::cout << "create_in_co start tid = " << CurrentThread::get_tid() << "\n";
  csd->yield();
  std::cout << "create_in_co end tid = " << CurrentThread::get_tid() << std::endl;
}

void test_func2(CoScheduler *csd){
  std::cout << "test_func2 start tid = " << CurrentThread::get_tid() << "\n";
  csd->yield();
  std::cout << "test_func2 end tid = " << CurrentThread::get_tid() << std::endl;

  csd->create_coroutine(create_in_co);
}

int main(){
  CoSchedulerThread cst;
  CoScheduler *csd = cst.start();

  csd->create_coroutine(test_func);
  csd->create_coroutine(test_func2);

  sleep(2);

  return 0;
}