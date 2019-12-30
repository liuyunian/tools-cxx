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

  static LogLevel init_threshold_level();
  static LogLevel get_threshold_level(){
    return m_thresholdLevel;
  }
  static void set_level(LogLevel level);
  static void set_level(const std::string &level);

  typedef std::function<void(const char*, int)> OutputFunc;
  static void default_output(const char *msg, int len);
  static void set_output(const OutputFunc &output){
    m_output = output;
  }

  typedef std::function<void()> FlushFunc;
  static void default_flush();
  static void set_flush(const FlushFunc &flush){
    m_flush = flush;
  }

private:
  static LogLevel m_thresholdLevel;
  static OutputFunc m_output;
  static FlushFunc m_flush;

  LogLevel m_curLevel;
  int m_errnoSave;
  LogPrint m_print;
};

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