# poller
代码参考自[muduo网络库](https://github.com/chenshuo/muduo)，poller对poll和epoll进行封装，提供了统一的接口。封装目的主要有两个：复用和易用，以下封装希望屏蔽poll和epoll使用时的诸多细节，提供简单易用的接口  

## 框架
首先看一下整体类图：  
![poller类图](/tools/poller/poller.png)  
PollPoller和EPollPoller分别对poll和epoll进行了封装，两者继承自Poller，Poller提供了统一的接口  
Channel封装了事件分发机制，与Poller是聚合关系 

## Channel
Channel对象负责文件描述符事件的分发，每个Channel对象自始至终只负责一个文件描述符的事件分发，Channel对象中保存了其负责的文件描述符的事件处理函数，事件发生时会调用响应的事件处理函数。值得注意的是：Channel不持有对应的文件描述符，即不负责close
Channel.h如下：
``` C++
#ifndef CHANNLE_H_
#define CHANNLE_H_

#include <functional>

#include "tools/base/noncopyable.h"

class Poller;

class Channel : noncopyable {
public:
  Channel(Poller *poller, int fd);
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
    update();
  }

  void disable_reading(){
    m_events &= ~kReadEvent;
    update();
  }

  void enable_writing(){
    m_events |= kWriteEvent;
    update();
  }

  void disable_writing(){
    m_events &= ~kWriteEvent;
    update();
  }

  bool is_writing() const {
    return m_events & kWriteEvent;
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

  void remove();

private:
  void update();

private:
  // events
  static const int kReadEvent;
  static const int kWriteEvent;

  // revents
  static const int READ_EVENT;
  static const int WRITE_EVENT;
  static const int ERROR_EVENT;
  static const int CLOSE_EVENT;

  Poller *m_poller;   // 所属的m_poller
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
```
* Channel类不是线程安全的，可以由持有Poller的类负责线程安全
* 每个Channel都属于一个Poller对象，但是Poller不对Channel对象的生命周期负责
* 通过调用enable_xx函数和disable_xx函数来给持有的文件描述符注册事件和取消注册事件，其中enable_xx函数回去调用update函数，该函数会将该Channel对象指针传递到Poller中，而remove函数则是从Poller中移除该Channel对象指针
* 通过set_xxx_callback函数提供事件发生后的回调函数，事件发生后调用handle_event函数，根据m_revent记录发生的事件就是事件分发

## Poller
头文件如下：
``` C++
#ifndef POLLER_H_
#define POLLER_H_

#include <vector>
#include <map>

#include "tools/base/noncopyable.h"

class Channel;

class Poller : noncopyable {
public:
  Poller() = default;
  virtual ~Poller() = default;

  typedef std::vector<Channel*> ChannelList;
  virtual ChannelList& poll(int timeoutMs) = 0;

  virtual void update_channel(Channel *channel) = 0;

  virtual void remove_channel(Channel *channel) = 0;

  static Poller* create_poller();

protected:
  ChannelList m_activeChannels;
  std::map<int, Channel*> m_channelStore;
};

#endif // POLLER_H_
```
* Poller类以接口方式存在，定义了诸多的纯虚函数由子类实现
* m_channelStore记录了都需要关注哪些文件描述符的事件，m_activeChannel则是记录哪些文件描述符发生了事件
* poll函数阻塞等待事件发生，返回发生事件的Channel列表，updata_channel函数用于添加要关注事件的Channel或者修改已有的Channel所关注的事件，remove_channel()则是移除指定的Channel，不再关注其事件
* 静态函数create_poller创建一个Poller子类对象，默认是EPollPoller对象，可以通过定义环境变量USE_POLL切换poll

## PollPoller & EPollPoller
这部分代码不再罗列，见源码

## 使用示例
[EchoServer_poller.cpp](/examples/echo/EchoServer_poller.cpp)