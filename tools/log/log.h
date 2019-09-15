#ifndef LOG_H
#define LOG_H

#include <stdarg.h> // va_list va_start va_end

#include <tools_cxx/SourceFile.h>

enum LogLevel{
    DEBUG,      // 用于调试
    INFO,       // 提示
    WARN,       // 警告
    ERROR,        // 错误
    FATAL,      // 致命错误
    NUM_LOG_LEVELS  // 日志级别数
};

extern void log_setlevel(LogLevel level);

extern void log_fatal(SourceFile file, int line, const char * fmt, ...);

extern void log_sysfatal(SourceFile file, int line, const char * fmt, ...);

extern void log_err(SourceFile file, int line, const char * fmt, ...);

extern void log_syserr(SourceFile file, int line, const char * fmt, ...);

extern void log_warn(SourceFile file, int line, const char * fmt, ...);

extern void log_info(SourceFile file, int line, const char * fmt, ...);

extern void log_debug(SourceFile file, int line, const char * fmt, ...);


#define LOG_FATAL(fmt, ...) log_fatal(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_SYSFATAL(fmt, ...) log_sysfatal(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_ERR(fmt, ...) log_err(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_SYSERR(fmt, ...) log_syserr(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_WRAN(fmt, ...) log_warn(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_INFO(fmt, ...) log_info(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_DEBUG(fmt, ...) log_debug(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

#endif