#include <iostream>

#include <tools/coroutine/Scheduler.h>
#include <tools/base/CurrentThread.h>

void test_func(co::Scheduler *sd){
  std::cout << "test_func start tid = " << CurrentThread::get_tid() << "\n";
  sd->yield();
  std::cout << "test_func end tid = " << CurrentThread::get_tid() << std::endl;
}

void create_in_co(co::Scheduler *sd){
  std::cout << "create_in_co start tid = " << CurrentThread::get_tid() << "\n";
  sd->yield();
  std::cout << "create_in_co end tid = " << CurrentThread::get_tid() << std::endl;

  sd->quit();
}

void test_func2(co::Scheduler *sd){
  std::cout << "test_func2 start tid = " << CurrentThread::get_tid() << "\n";
  sd->yield();
  std::cout << "test_func2 end tid = " << CurrentThread::get_tid() << std::endl;

  sd->create_coroutine(create_in_co);
}

int main(){
  co::Scheduler sd;
  sd.create_coroutine(test_func);
  sd.create_coroutine(test_func2);

  sd.start();

  return 0;
}