#include <iostream>
#include <tools/base/CurrentThread.h>

int main(){
    std::cout << CurrentThread::get_tid() << std::endl;
    std::cout << CurrentThread::get_tid_string() << std::endl;
    std::cout << CurrentThread::get_tid_string_length() << std::endl;

    return 0;
}