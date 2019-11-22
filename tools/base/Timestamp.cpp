#include <stdio.h>      // snprintf
#include <inttypes.h>	// PRId64

#include "Timestamp.h"

std::string Timestamp::to_string() const {
    char buf[32] = {0};																		    // 表示微秒占6个字符，dot占一个字符，末尾的/0占一个字符，所以秒部分最多占24个字符，完全足够使用了
    int64_t seconds = m_microsecondsSinceEpoch / kmicrosecondsPerSecond;
    int64_t microseconds = m_microsecondsSinceEpoch % kmicrosecondsPerSecond;
    snprintf(buf, sizeof(buf)-1, "%" PRId64 ".%06" PRId64 "", seconds, microseconds);		    // PRId64: int64_t表示64位有符号整数，在32位系统上是long long，在64位系统中是long，所以在32位系统中打印的格式是%lld，在64位系统中打印格式位%ld
    return buf;																				    // 所以如果要跨平台打印in64_t就要使用一个统一的格式，也就是这里的PRId64z，其本质是字符串"lld"(32bit) or "ld"(64bit)
}

std::string Timestamp::to_formatted_string(bool showMicroseconds) const {
    char buf[64] = {0};
    time_t seconds = static_cast<time_t>(m_microsecondsSinceEpoch / kmicrosecondsPerSecond); 	// time_t本质是long int
    struct tm tm_time;																		    // 用于保存时间和日期
    // gmtime_r(&seconds, &tm_time);															// 将time_t类型表示的时间分解填充到struct tm结构中，与之类似的gmtime(const time_t * timer)函数，不用提供自行定义的struct tm对象
    localtime_r(&seconds, &tm_time);                                                            // localtime_r和gmtime_r区别在于时区，gmtime_r采用的UTC标准，而localtime_r则是采用本地时区（比如CST中国标准时间），与之类似的是localtime(const time_t * timer)函数

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
    gettimeofday(&tv, NULL);                                                                    // gettimeofday()获取当前的时间
    int64_t seconds = tv.tv_sec;
    return Timestamp(seconds * kmicrosecondsPerSecond + tv.tv_usec);
}