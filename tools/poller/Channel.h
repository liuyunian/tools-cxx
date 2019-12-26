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
    m_events |= kReadEvent;
  }

  void disable_reading(){
    m_events &= ~kReadEvent;
  }

  void enable_writing(){
    m_events |= kWriteEvent;
  }

  void disable_writing(){
    m_events &= ~kWriteEvent;
  }

  typedef std::function<void()> EventCallback;
  void set_read_callback(const EventCallback &cb){
    m_readCallback = cb;
  }

  void set_write_callback(const EventCallback &cb){
    m_writeCallback = cb;
  }

  void set_error_callback(const EventCallback &cb){
    m_errorCallback = cb;
  }

  void set_close_callback(const EventCallback &cb){
    m_closeCallback = cb;
  }

  void handle_event();

private:
  // events
  static const int kReadEvent;
  static const int kWriteEvent;

  // revents
  static const int READ_EVENT;
  static const int WRITE_EVENT;
  static const int ERROR_EVENT;
  static const int CLOSE_EVENT;

  const int m_fd;     // 负责的文件描述符
  int m_events;       // 关心的事件
  int m_revents;      // 发生的事件
  int m_index;        // used by Poller，既用来表征在m_pollfdList中的位置又用于区分add/update操作

  EventCallback m_readCallback;   // 可读事件回调
  EventCallback m_writeCallback;  // 可写事件回调
  EventCallback m_closeCallback;  // 连接关闭事件回调
  EventCallback m_errorCallback;  // 错误事件回调
};

#endif // CHANNLE_H_