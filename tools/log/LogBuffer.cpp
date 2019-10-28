#include "tools/log/LogBuffer.h"

LogBuffer::LogBuffer(int capacity) : 
    m_capacity(capacity),
    m_data(new char[m_capacity]),
    m_current(m_data)
{
    bzero();
    set_cookie(cookie_start);
}

LogBuffer::~LogBuffer(){
    delete[] m_data;
    set_cookie(cookie_end); 
}

void LogBuffer::append(const char* buf, size_t len){
    size_t cpLen = m_current + len < end() ? len : (end() - m_current);
    memcpy(m_current, buf, cpLen);
    m_current += cpLen;
}

void LogBuffer::append_char(const char ch){
    if(available()){
        *m_current = ch;
        ++ m_current;
    }
}

void LogBuffer::cookie_start(){}

void LogBuffer::cookie_end(){}