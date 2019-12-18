#include <iostream>
#include <tools/base/Exception.h>

class ExceptionTest{
public:
  ExceptionTest(){
    throw Exception("hello world");
  }
};

void test_func(){
  ExceptionTest b;
}

int main(){
  try{
    test_func();
  }
  catch(Exception &e){
    std::cout << e.what() << '\n';
    std::cout << e.stack_trace() << std::endl;
  }
  
  return 0;
}