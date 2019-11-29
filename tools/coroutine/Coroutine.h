#ifndef COROUTINE_H_
#define COROUTINE_H_

#include <vector>
#include <memory>
#include <functional>

#include "tools/base/noncopyable.h"

namespace co {

class Scheduler;

class Coroutine : noncopyable {
public:
  enum Status {
    READY,
    SUSPENDED,  // 暂停
    RUNNING,
    FINISHED
  };

  typedef std::function<void(Scheduler*)> Task;
  Coroutine(size_t stackSize, const Task &task) : 
    m_task(std::move(task)), 
    m_status(READY), 
    m_stack(stackSize)
  {}

  void set_status(Status status){
    m_status = status;
  }

  Status get_status() const {
    return m_status;
  }

  ucontext_t* get_contextptr(){
    return &m_context;
  }

  char* get_stack(){
    return m_stack.data();
  }

public:
  const Task m_task;

private:
  Status m_status;
  ucontext_t m_context;
  std::vector<char> m_stack;
};

} // namespce co



#endif // COROUTINE_H_