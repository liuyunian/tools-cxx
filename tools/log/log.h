#ifndef LOG_H_
#define LOG_H_

#include <functional>

#include <stdarg.h> // va_xxx

#include "tools/base/noncopyable.h"
#include "tools/log/LogPrint.h"

class Log : noncopyable {
public:
    enum LogLevel {
        DEBUG,          // 用于调试
        INFO,           // 提示
        WARN,           // 警告
        ERROR,          // 错误
        FATAL,          // 致命错误
        NUM_LOG_LEVELS  // 日志级别数
    };

    explicit Log(LogLevel logLevel);
    explicit Log(bool toAbort);
    ~Log();

    void log(const char *file, int line, const char* fmt, ...);

    static LogLevel get_threshold_level();
    static LogLevel init_threshold_level();
    static void set_level(LogLevel level);

    typedef std::function<void(const char* msg, int len)> OutputFunc;
    static void set_output(OutputFunc output);
    static void default_output(const char* msg, int len);

    typedef std::function<void()> FlushFunc;
    static void set_flush(FlushFunc flush);
    static void default_flush();

private:
    static LogLevel m_thresholdLevel;
    static OutputFunc m_output;
    static FlushFunc m_flush;

    LogLevel m_curLevel;
    int m_errnoSave;
    LogPrint m_print;
};

inline Log::LogLevel Log::get_threshold_level(){  // 内联函数
    return m_thresholdLevel;
}

#define LOG_DEBUG(fmt, ...) if(Log::get_threshold_level() <= Log::DEBUG) \
    Log(Log::DEBUG).log(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_INFO(fmt, ...) if(Log::get_threshold_level() <= Log::INFO) \
    Log(Log::INFO).log(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_WARN(fmt, ...) if(Log::get_threshold_level() <= Log::WARN) \
    Log(Log::WARN).log(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_ERR(fmt, ...) Log(Log::ERROR).log(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_FATAL(fmt, ...) Log(Log::FATAL).log(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_SYSERR(fmt, ...) Log(false).log(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_SYSFATAL(fmt, ...) Log(true).log(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

#endif // LOG_H_