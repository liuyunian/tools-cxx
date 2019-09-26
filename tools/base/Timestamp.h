#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include <string>

#include <stdint.h>     // int64_t
#include <sys/time.h>   // time_t

#include "copyable.h"

class Timestamp : copyable{
public:
    Timestamp() : 
        m_microsecondsSinceEpoch(0){}

    explicit Timestamp(int64_t microseconds) : 
        m_microsecondsSinceEpoch(microseconds){}

    ~Timestamp() = default;

    std::string to_string() const;
    std::string to_formattedString(bool showMicroseconds = true) const;

    int64_t get_microsecondsSinceEpoch() const {
        return m_microsecondsSinceEpoch;
    }

    static Timestamp invalid(){
        return Timestamp();
    }

    static Timestamp now();

    static const int k_microsecondsPerSecond = 1000 * 1000;

private:
    int64_t m_microsecondsSinceEpoch;
};

inline bool operator<(Timestamp lhs, Timestamp rhs) {                                        // 为什么不重载类内的运算符呢？
  return lhs.get_microsecondsSinceEpoch() < rhs.get_microsecondsSinceEpoch();
}

inline bool operator==(Timestamp lhs, Timestamp rhs){
  return lhs.get_microsecondsSinceEpoch() == rhs.get_microsecondsSinceEpoch();
}


/**
 * @brief 给指定的时间戳增加second秒
 * @return 返回增加之后的时间戳
*/
inline Timestamp add_time(Timestamp timestamp, double second){
    int64_t delta = static_cast<int64_t>(second * Timestamp::k_microsecondsPerSecond);
    return Timestamp(timestamp.get_microsecondsSinceEpoch() + delta);
}

#endif // TIMESTAMP_H_