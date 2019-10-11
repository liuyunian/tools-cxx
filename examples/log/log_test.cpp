#include <errno.h>
#include <tools/log/log.h>

int main(){
    log_setlevel(DEBUG);

    LOG_DEBUG("debug, %d", 0);
    LOG_INFO("info, %f", 1.1);
    LOG_WARN("warn, %c", 's');
    LOG_ERR("error, %s", "test");
    
    errno = 2;
    LOG_SYSERR("syserr, %x", 0xef);

    // LOG_FATAL("fatal, %u", -1);
    
    int a;
    errno = 3;
    LOG_SYSFATAL("sysfatal, %p", &a);

    return 0;
}