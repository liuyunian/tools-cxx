#include <stdio.h>  // fwrite
#include <errno.h>  // errno
#include <string.h> // strerrno 
#include <stdlib.h> // abort
#include <libgen.h> // basename

#include "tools/log/log.h"
#include "tools/base/Timestamp.h"
#include "tools/base/CurrentThread.h"

Log::LogLevel Log::m_thresholdLevel = init_threshold_level();
Log::OutputFunc Log::m_output = default_output;
Log::FlushFunc Log::m_flush = default_flush;

Log::Log(LogLevel logLevel) : 
  m_curLevel(logLevel),
  m_errnoSave(0){}

Log::Log(bool toAbort) : 
  m_curLevel(toAbort?FATAL:ERROR),
  m_errnoSave(errno){}

Log::~Log(){
  const LogBuffer& buffer(m_print.get_buffer());
  m_output(buffer.data(), buffer.length());

  if(m_curLevel == FATAL){
    m_flush();
    abort();
  }
}

const char* LogLevelName[Log::NUM_LOG_LEVELS] = {
  "[DEBUG]",
  "[INFO]",
  "[WARN]",
  "[ERROR]",
  "[FATAL]",
};

void Log::log(const char *file, int line, const char *fmt, ...){
  Timestamp now(Timestamp::now());
  m_print.printf("%s %s %s ", 
                  now.to_formatted_string().c_str(),    // 时间戳
                  CurrentThread::get_tid_string(),      // 线程ID
                  LogLevelName[m_curLevel]              // 日志级别
                );                                      
  
  va_list args;
  va_start(args, fmt);
  m_print.vprintf(fmt, args);                           // 正文
  va_end(args);

  if(m_errnoSave != 0){
    m_print.printf(" %s (errno = %d)", m_errnoSave);    // strerrno (errno = ?)
  }

  m_print.printf(" - %s: %d\n", 
                  ::basename(const_cast<char*>(file)),  // 所在文件名
                  line
                );                                      // 行号
}

Log::LogLevel Log::init_threshold_level(){
  if (::getenv("LOG_DEBUG")){                           // 如果定义了环境变量LOG_DEBUG，日志级别设置为DEBUG
    return DEBUG;
  }
  else if(::getenv("LOG_WARN")){                        // 日志级别设置为WARN
    return WARN;
  }                                                                  
  else{
    return INFO;
  }
}

void Log::default_output(const char* msg, int len){
  size_t n = fwrite(msg, 1, len, stdout);
  if(n < 0){
    perror("Failed to call fwrite int Log::default_output");
  }
}

void Log::default_flush(){
  fflush(stdout);
}

void Log::set_level(LogLevel level){
  m_thresholdLevel = level;
}

void Log::set_level(const std::string &level){
  if(level == "LOG_DEBUG"){
    m_thresholdLevel = DEBUG;
  }
  else if(level == "LOG_WARN"){
    m_thresholdLevel = WARN;
  }
  else{
    // 默认的INFO级别
  }
}