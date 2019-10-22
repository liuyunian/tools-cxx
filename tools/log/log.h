#ifndef LOG_H
#define LOG_H

#include <stdarg.h> // va_list va_start va_end

enum LogLevel{
    DEBUG,      // 用于调试
    INFO,       // 提示
    WARN,       // 警告
    ERROR,      // 错误
    FATAL,      // 致命错误
    NUM_LOG_LEVELS  // 日志级别数
};

extern void log_set_level(LogLevel level);

extern void log_fatal(const char* file, int line, const char* fmt, ...);

extern void log_sysfatal(const char* file, int line, const char* fmt, ...);

extern void log_err(const char* file, int line, const char* fmt, ...);

extern void log_syserr(const char* file, int line, const char* fmt, ...);

extern void log_warn(const char* file, int line, const char* fmt, ...);

extern void log_info(const char* file, int line, const char* fmt, ...);

extern void log_debug(const char* file, int line, const char* fmt, ...);


#define LOG_FATAL(fmt, ...) log_fatal(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_SYSFATAL(fmt, ...) log_sysfatal(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_ERR(fmt, ...) log_err(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_SYSERR(fmt, ...) log_syserr(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_WARN(fmt, ...) log_warn(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_INFO(fmt, ...) log_info(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_DEBUG(fmt, ...) log_debug(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

// Taken from muduo/logging.h
// Check that the input is non nullptr.  This very useful in constructor initializer lists                  -- 检查输入参数不为nullptr，这对于构造函数初始化列表非常有用
#define CHECK_NOT_NULLPTR(val) \
  ::Check_Not_Nullptr(__FILE__, __LINE__, "'" #val "' Must be non nullptr", (val))

// A small helper for CHECK_NOTNULL().
template <typename T>
T* Check_Not_Nullptr(const char* file, int line, const char* names, T* ptr){                                // 检查参数ptr不是nullptr，如果ptr为nullptr，输出FATAL日志信息
    if (ptr == nullptr){
        log_fatal(file, line, names);
    }
    return ptr;
}

#endif