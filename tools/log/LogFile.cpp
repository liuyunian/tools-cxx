#include <iostream>

#include "tools/log/LogFile.h"
#include "tools/base/Timestamp.h"
#include "tools/base/ProcessInfo.h"


LogFile::LogFile(const std::string& basename, off_t rollSize, int flushInterval, int checkEveryN) : 
    m_basename(basename),
    m_rollSize(rollSize),
    m_flushInterval(flushInterval),
    m_checkEveryN(checkEveryN),
    m_count(0),
    m_lastRoll(0),
    m_lastFlush(0),
    m_startPeriod(0),
    m_writenBytes(0)
{
    roll_file();
}

void LogFile::append(const char *logLine, int len){
    std::unique_lock<std::mutex> ul(m_mutex);
    append_unlocked(logLine, len);
}

void LogFile::append_unlocked(const char *logLine, int len){
    m_fout->write(logLine, len);    // 隐式类型转换 int -> std::streamsize
    m_writenBytes += len;           // 隐式类型转换 int -> off_t

    if(m_writenBytes > m_rollSize){
        roll_file();
    }
    else{
        ++ m_count;
        if(m_count >= m_checkEveryN){
            m_count = 0;

            Timestamp now = Timestamp::now();
            if(now.get_period() != m_startPeriod){
                roll_file();
            }
            else if(now.get_seconds_since_epoch() - m_lastFlush > m_flushInterval){
                m_lastFlush = now.get_seconds_since_epoch();
                m_fout->flush();
            }
        }
    }
}

void LogFile::flush(){
    std::unique_lock<std::mutex> ul(m_mutex);
    m_fout->flush();
}

void LogFile::roll_file(){
    Timestamp now;
    std::string fileName = get_file_name(now);

    m_lastRoll = m_lastFlush = now.get_seconds_since_epoch();
    m_startPeriod = now.get_period();

    m_fout.reset(new std::ofstream(fileName, std::ios::out | std::ios::app));
    m_writenBytes = 0;
}

std::string LogFile::get_file_name(Timestamp &now){
    now = Timestamp::now();

    std::string fileName(m_basename);
    fileName.reserve(m_basename.size() + 64);
    fileName += ".";
    fileName += now.to_formatted_string(false);
    fileName += ".";
    fileName += ProcessInfo::get_hostname();
    fileName += ".";
    fileName += ProcessInfo::get_pid_string();
    fileName += ".log";

    return fileName;
}

void LogFile::print_file_name(){
    Timestamp now;
    std::cout << get_file_name(now) << std::endl;
}