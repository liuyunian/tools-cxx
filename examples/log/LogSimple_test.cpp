#include <errno.h>
#include <tools/logSimple/LogSimple.h>

int main(){
    log_set_level(DEBUG);

    LOG_DEBUG("debug, %d", 0);
    LOG_INFO("info, %f", 1.1);
    LOG_WARN("warn, %c", 's');
    LOG_ERR("error, %s", "test");
    
    errno = 2;
    LOG_SYSERR("syserr, %x", 0xef);

    // LOG_FATAL("fatal, %u", -1);
    
    // int a;
    // errno = 3;
    // LOG_SYSFATAL("sysfatal, %p", &a);

    // check_not_null
    int b = 5;
    int* p = &b;
    CHECK_NOT_NULLPTR(p);

    p = nullptr;
    CHECK_NOT_NULLPTR(p);

    return 0;
}