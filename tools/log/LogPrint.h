#ifndef LOGPRINT_H_
#define LOGPRINT_H_

#include <stdarg.h> // va_list
#include <stdint.h> // uint64_t

#include "tools/base/noncopyable.h"
#include "tools/log/LogBuffer.h"

class LogPrint : noncopyable {
public:
    LogPrint();
    ~LogPrint() = default;

    void printf(const char* fmt, ...);

    // for unit test
    void printf_buffer();

private:
    enum HexFlag {
        No,
        Lower,
        Upper
    };

    void vprintf(const char* fmt, va_list args);

    void printf_num(uint64_t uint64, HexFlag hex, char padding, uint8_t width);

private:
    static const int SMALL_BUFFER;

    LogBuffer m_buffer;
};

#endif // LOGPRINT_H_