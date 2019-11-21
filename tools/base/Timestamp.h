#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include <string>

#include <stdint.h>     // int64_t
#include <sys/time.h>   // time_t

#include <boost/operators.hpp>

#include "tools/base/copyable.h"

class Timestamp : copyable,
                  public boost::equality_comparable<Timestamp>, // 继承boost::equality_comparable，要求实现==操作符函数，并自动提供'!='操作符函数
                  public boost::less_than_comparable<Timestamp> // 继承boost::less_than_comparable，要求实现<操作符函数，并自动提供'>' '<=' '>='操作符函数
{
public:
    Timestamp() : 
        m_microsecondsSinceEpoch(0){}

    explicit Timestamp(int64_t microseconds) : 
        m_microsecondsSinceEpoch(microseconds){}

    ~Timestamp() = default;

    bool is_valid(){
        return m_microsecondsSinceEpoch > 0;
    }

    int64_t get_microseconds_since_epoch() const {
        return m_microsecondsSinceEpoch;
    }

    int64_t get_seconds_since_epoch() const {
        return m_microsecondsSinceEpoch / kmicrosecondsPerSecond;
    }

    /**
     * @brief 以86400s为单位作为一个period，比如0-86399属于一个period
    */
    int64_t get_period() const {
        return m_microsecondsSinceEpoch / kmicrosecondsPerSecond / ksecondsPerDay;
    }

    std::string to_string() const;
    std::string to_formatted_string(bool showMicroseconds = true) const;

    /**
     * @brief timestamp + second
     * @return 返回增加之后的时间戳
    */
    Timestamp add_time(double second){
        int64_t delta = static_cast<int64_t>(second * kmicrosecondsPerSecond);
        return Timestamp(m_microsecondsSinceEpoch + delta);
    }

    /**
     * @brief 产生无效的时间戳
    */
    static Timestamp invalid(){
        return Timestamp();
    }

    static Timestamp now();

private:
    const static int kmicrosecondsPerSecond = 1000 * 1000;
    const static int ksecondsPerDay = 60*60*24;

    int64_t m_microsecondsSinceEpoch;
};

// 作为友元函数重载'=='和'<'运算符
inline bool operator==(Timestamp lhs, Timestamp rhs){
    return lhs.get_microseconds_since_epoch() == rhs.get_microseconds_since_epoch();
}

inline bool operator<(Timestamp lhs, Timestamp rhs) {
    return lhs.get_microseconds_since_epoch() < rhs.get_microseconds_since_epoch();
}

#endif // TIMESTAMP_H_