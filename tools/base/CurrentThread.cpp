#include <stdio.h>      // snprintf
#include <unistd.h>     // syscall
#include <sys/syscall.h>  // SYS_gettid

#include "CurrentThread.h"

namespace CurrentThread{

static __thread int t_cachedTid = 0;                                                        // 用来存放线程tid
static __thread char t_tidString[32];                                                       // 用来存放tid字符串形式，比如"1234"
static __thread int t_tidStringLength = 6;                                                  // tid字符串形式的长度，最大65535，所以长度为6

static void cache_tid();

int get_tid(){
    cache_tid();
    return t_cachedTid;
}

const char * get_tidString(){                                                               // 返回tid字符串形式，用在日志打印中
    cache_tid();
    return t_tidString;
}

int get_tidStringLength(){                                                                  // 返回tid字符串形式的长度，用在日志打印中
    cache_tid();
    return t_tidStringLength;
}

void cache_tid(){
    if(__builtin_expect(t_cachedTid == 0, 0)){
        t_cachedTid = ::syscall(SYS_gettid);                                                // 通过gettid获取真实的线程id -- tid
        t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%d", t_cachedTid);   // tid的字符串形式
    }
}

} // namespace CurrentThread