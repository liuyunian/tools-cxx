#ifndef LOGFILE_H_
#define LOGFILE_H_

#include <string>
#include <fstream>
#include <memory>
#include <mutex>

#include "tools/base/noncopyable.h"

class Timestamp;

class LogFile : noncopyable {
public:
  LogFile(const std::string &basename, 
          off_t rollSize = 64*1024*1024,  // 默认日志文件大小64MB
          int flushInterval = 3,          // 默认flush间隔是3s
          int checkEveryN = 1024);        // 计数器上限默认是1024

  ~LogFile() = default;

  void append(const char *logLine, int len);

  void flush();

  void setCheckEveryN(int n){
    m_checkEveryN = n;
  }

  void setFlushInterval(int interval){
    m_flushInterval = interval;
  }

private:
  void roll_file();

  void append_unlocked(const char *logLine, int len);

  std::string get_file_name(Timestamp &now);            // 获取日志文件名，日志文件名：basename + 当前时间 + 主机名 + 进程id + .log

private:
  const std::string m_basename; // 日志文件名开始的部分
  const off_t m_rollSize;       // 日志文件多大时需要切换日志文件，单位字节
  int m_flushInterval;          // 将日志信息flush到日志文件的时间间隔
  int m_checkEveryN;            // 计数器上限，当计数器的值 == m_checkEveryN，判断是否需要将日志信息flush到日志文件中？日志文件是否需要更换？

  int m_count;                    // 计数器

  int64_t m_lastRoll;             // 上一次日志滚动的时间
  int64_t m_lastFlush;            // 上一次日志滚动的时间
  int64_t m_startPeriod;          // 用于判断是否处于同一天

  std::mutex m_mutex;
  std::unique_ptr<std::ofstream> m_fout;
  off_t m_writenBytes;
};

#endif // LOGFILE_H_