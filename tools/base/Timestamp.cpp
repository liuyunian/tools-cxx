#include <stdio.h>      // snprintf
#include <inttypes.h>	// PRId64

#include "Timestamp.h"

std::string Timestamp::to_string() const {
  char buf[32] = {0};
  int64_t seconds = m_microsecondsSinceEpoch / kmicrosecondsPerSecond;
  int64_t microseconds = m_microsecondsSinceEpoch % kmicrosecondsPerSecond;
  snprintf(buf, sizeof(buf)-1, "%" PRId64 ".%06" PRId64 "", seconds, microseconds);	        // PRId64: int64_t表示64位有符号整数，在32位系统上是long long，在64位系统中是long，所以在32位系统中打印的格式是%lld，在64位系统中打印格式位%ld
  return buf;																				                                        // 所以如果要跨平台打印in64_t就要使用一个统一的格式，也就是这里的PRId64z，其本质是字符串"lld"(32bit) or "ld"(64bit)
}

std::string Timestamp::to_formatted_string(bool showMicroseconds) const {
  char buf[64] = {0};
  time_t seconds = static_cast<time_t>(m_microsecondsSinceEpoch / kmicrosecondsPerSecond);
  struct tm tm_time;
  localtime_r(&seconds, &tm_time);

  if(showMicroseconds){
    int microseconds = static_cast<int>(m_microsecondsSinceEpoch % kmicrosecondsPerSecond);
    snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",                           // 用于日志
            tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
            tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
            microseconds);
  }
  else{
    snprintf(buf, sizeof(buf), "%4d%02d%02d-%02d%02d%02d",                                  // 用于日志文件名
            tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
            tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
  }
  
  return buf;
}

Timestamp Timestamp::now(){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  int64_t seconds = tv.tv_sec;
  return Timestamp(seconds * kmicrosecondsPerSecond + tv.tv_usec);
}