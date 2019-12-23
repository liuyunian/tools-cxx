#ifndef CHANNLE_H_
#define CHANNLE_H_

#include <functional>

#include "tools/base/noncopyable.h"

class Channel : noncopyable {
public:
  Channel(int fd);
  ~Channel() = default;

  int get_fd() const {
    return m_fd;
  }

  int get_events() const {
    return m_events;
  }

  int get_revents() const {
    return m_revents;
  }

  void set_revents(int revents){
    m_revents = revents;
  }

  int get_index() const {
    return m_index;
  }

  void set_index(int index){
    m_index = index;
  }

  void enable_reading(){
    m_events |= k_readEvent;
  }

  void disable_reading(){
    m_events &= ~k_readEvent;
  }

  void enable_writing(){
    m_events |= k_writeEvent;
  }

  void disable_writing(){
    m_events &= ~k_writeEvent;
  }

private:
  static const int k_readEvent;
  static const int k_writeEvent;

  const int m_fd;     // 负责的文件描述符
  int m_events;       // 关心的事件
  int m_revents;      // 发生的事件
  int m_index;        // used by Poller，既用来表征在m_pollfdList中的位置又用于区分add/update操作

  // EventCallback m_readCallback;   // 可读事件回调函数
  // EventCallback m_writeCallback;  // 可写事件回调函数
};

#endif // CHANNLE_H_