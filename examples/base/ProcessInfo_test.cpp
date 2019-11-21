#include <iostream>

#include <tools/base/ProcessInfo.h>

int main(){
    std::cout << "pid = " << ProcessInfo::get_pid() << '\n';

    std::cout << "pidString = " << ProcessInfo::get_pid_string() << '\n';

    std::cout << "hostname = " << ProcessInfo::get_hostname() << std::endl;

    return 0;
}