#include <stdarg.h> // va_list va_start va_end
#include <errno.h>  // errno
#include <stdio.h>  //stdout stderr
#include <string.h> // strlen strcat
#include <stdlib.h> // exit abort

#include "log.h"
#include "../base/Timestamp.h"
#include "../base/CurrentThread.h"

#define LINE_SZ 4096

static LogLevel init_logLevel(){
    if (::getenv("LOG_DEBUG")){                         // 如果定义了环境变量MUDUO_LOG_TRACE，日志级别设置为TRACE
        return DEBUG;
    }                                                                  
    else{
        return INFO;
    }
}

static LogLevel g_logLevel = init_logLevel();

const char* LogLevelName[NUM_LOG_LEVELS] =
{
  "[DEBUG]",
  "[INFO]",
  "[WARN]",
  "[ERROR]",
  "[FATAL]",
};

static void 
log_printf(int logLevel, 
            int errnoSave, 
            const char * fmt, 
            va_list ap, 
            const char * fileName, 
            int line);

void log_setlevel(LogLevel level){
    g_logLevel = level;
}

void log_fatal(SourceFile file, int line, const char * fmt, ...){
    va_list ap;
    va_start(ap, fmt);

    log_printf(FATAL, 0, fmt, ap, file.data_, line);

    va_end(ap);
    abort();
}

void log_sysfatal(SourceFile file, int line, const char * fmt, ...){
    va_list ap;
    va_start(ap, fmt);

    log_printf(FATAL, errno, fmt, ap, file.data_, line);

    va_end(ap);
    abort();
}

void log_err(SourceFile file, int line, const char * fmt, ...){
    va_list ap;
    va_start(ap, fmt);

    log_printf(ERROR, 0, fmt, ap, file.data_, line);

    va_end(ap);
    return;
}

void log_syserr(SourceFile file, int line, const char * fmt, ...){
    va_list ap;
    va_start(ap, fmt);

    log_printf(ERROR, errno, fmt, ap, file.data_, line);

    va_end(ap);
    return;
}

void log_warn(SourceFile file, int line, const char * fmt, ...){
    va_list ap;
    va_start(ap, fmt);

    log_printf(WARN, 0, fmt, ap, file.data_, line);

    va_end(ap);
}

void log_info(SourceFile file, int line, const char * fmt, ...){
    if(g_logLevel > INFO){
        return;
    }

    va_list ap;
    va_start(ap, fmt);

    log_printf(INFO, 0, fmt, ap, file.data_, line);

    va_end(ap);
}

void log_debug(SourceFile file, int line, const char * fmt, ...){
    if(g_logLevel > DEBUG){
        return;
    }

    va_list ap;
    va_start(ap, fmt);

    log_printf(DEBUG, 0, fmt, ap, file.data_, line);

    va_end(ap);
}

static void 
log_printf(int logLevel, int errnoSave, const char * fmt, va_list ap, const char * fileName, int line){
    int bufLen;
    char buf[LINE_SZ + 1] = {0};
    
    Timestamp now(Timestamp::now());
    bufLen = snprintf(buf, LINE_SZ, "%s %s %s: ", 
                                    now.to_formattedString().c_str(),   // 时间戳
                                    CurrentThread::get_tidString(),     // 线程ID
                                    LogLevelName[logLevel]);            // 日志级别

    vsnprintf(buf + bufLen, LINE_SZ - bufLen, fmt, ap);
    bufLen = strlen(buf);

    if(errnoSave){
        snprintf(buf + bufLen, LINE_SZ - bufLen, " (%d: %s)", errnoSave, strerror(errnoSave));
    }

    bufLen = strlen(buf);
    snprintf(buf + bufLen, LINE_SZ - bufLen, " - %s:%d\n", fileName, line);

    fflush(stdout);
    fputs(buf, stderr);
    fflush(stderr);

    return;
}
