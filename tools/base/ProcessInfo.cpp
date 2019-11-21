#include <string.h> // memset
#include <unistd.h> // gethostname
#include <stdlib.h> // getpid

#include <tools/base/ProcessInfo.h>

pid_t ProcessInfo::get_pid(){
    return ::getpid();
}

std::string ProcessInfo::get_pid_string(){
    return std::to_string(get_pid());
}

std::string ProcessInfo::get_hostname(){
    // HOST_NAME_MAX 64
    // _POSIX_HOST_NAME_MAX 255
    char buf[256];
    ::memset(buf, 0, sizeof(buf));
    if (::gethostname(buf, sizeof buf) < 0){
        return "unknownhost";
    }

    return buf;
}