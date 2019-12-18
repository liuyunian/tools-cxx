#include <iostream>
#include <tools/base/Exception.h>

class Bar{
public:
  void test(){
    throw Exception("hello world");
  }
};

void foo(){
  Bar b;
  b.test();
}

int main(){
  try{
    foo();
  }
  catch(Exception &e){
    std::cout << e.what() << '\n';
    std::cout << e.stack_trace() << std::endl;
  }
  
  return 0;
}

/*
hello world
./build/base/Exception_test(Exception::Exception(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)+0x4e) [0x7f40dac020a4]
./build/base/Exception_test(Bar::test()+0x5d) [0x7f40dac01f95]
./build/base/Exception_test(foo()+0x23) [0x7f40dac01d8d]
./build/base/Exception_test(main+0x1d) [0x7f40dac01dc1]
/lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xe7) [0x7f40d9e71b97]
./build/base/Exception_test(_start+0x2a) [0x7f40dac01c8a]

hello world
./build/base/Exception_test(_ZN9ExceptionC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE+0x4e) [0x7fb1d1201cb4]
./build/base/Exception_test(_ZN3Bar4testEv+0x5d) [0x7fb1d1201ba5]
./build/base/Exception_test(_Z3foov+0x23) [0x7fb1d120199d]
./build/base/Exception_test(main+0x1d) [0x7fb1d12019d1]
/lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xe7) [0x7fb1d0271b97]
./build/base/Exception_test(_start+0x2a) [0x7fb1d120189a]
*/