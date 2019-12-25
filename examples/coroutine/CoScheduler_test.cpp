#include <iostream>

#include <tools/coroutine/CoScheduler.h>
#include <tools/base/CurrentThread.h>

void test_func(CoScheduler *csd){
  std::cout << "test_func start tid = " << CurrentThread::get_tid() << "\n";
  csd->yield();
  std::cout << "test_func end tid = " << CurrentThread::get_tid() << std::endl;
}

void create_in_co(CoScheduler *csd){
  std::cout << "create_in_co start tid = " << CurrentThread::get_tid() << "\n";
  csd->yield();
  std::cout << "create_in_co end tid = " << CurrentThread::get_tid() << std::endl;

  csd->quit();
}

void test_func2(CoScheduler *csd){
  std::cout << "test_func2 start tid = " << CurrentThread::get_tid() << "\n";
  csd->yield();
  std::cout << "test_func2 end tid = " << CurrentThread::get_tid() << std::endl;

  csd->create_coroutine(create_in_co);
}

int main(){
  CoScheduler csd;
  csd.create_coroutine(test_func);
  csd.create_coroutine(test_func2);

  csd.start();

  return 0;
}