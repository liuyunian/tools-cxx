#ifndef LOGBUFFER_H_
#define LOGBUFFER_H_

#include <string>
#include <stdio.h>

#include <string.h> // memset

#include "tools/base/noncopyable.h"

class LogBuffer : noncopyable {
public:
    explicit LogBuffer(int capacity);

    ~LogBuffer();

    const char* data() const {
        return m_data;
    }

    bool available() const {
        return end() > m_current;
    }

    size_t length() const {
        return static_cast<size_t>(m_current - m_data);
    }

    // 空间不足则会截断后append
    void append(const char* buf, size_t len);

    // 空间不足则不会append
    void append_char(const char ch);


    // for GDB
    void set_cookie(void(*cookie)()){
        m_cookie = cookie;
    }

    // for unit test
    std::string to_string() const {
        return std::string(m_data, length());
    }

private:
    // 必须是outline？？
    static void cookie_start();
    static void cookie_end();

    /**
     * @brief 指向数组最后一个位置
     * 最后一个位置必须存放'/0'
    */
    const char* end() const {
        return m_data + m_capacity - 1;
    }

    void bzero(){
        memset(m_data, 0, m_capacity);
    }

private:
    int m_capacity;
    char* m_data;
    char* m_current;    // 指向当前可用的空间，注意最后一个字节保留存放'/0'
    void(*m_cookie)();  // 充当哨兵
};

#endif // LOGBUFFER_H_