#include <iostream>
#include <tools/base/CurrentThread.h>

int main(){
    std::cout << CurrentThread::get_tid() << std::endl;
    std::cout << CurrentThread::get_tidString() << std::endl;
    std::cout << CurrentThread::get_tidStringLength() << std::endl;

    return 0;
}