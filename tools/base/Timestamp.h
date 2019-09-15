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

    static Timestamp now();

    static const int k_microsecondsPerSecond = 1000 * 1000;

private:
    int64_t m_microsecondsSinceEpoch;
};

#endif // TIMESTAMP_H_