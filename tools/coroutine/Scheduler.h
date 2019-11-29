#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <map>
#include <vector>
#include <mutex>
#include <atomic>
#include <memory>
#include <functional>

#include <ucontext.h>

#include "tools/base/noncopyable.h"
#include "tools/coroutine/Coroutine.h"
#include "tools/base/CurrentThread.h"

namespace co {

class Scheduler : noncopyable{
public:
  Scheduler(size_t stackSize = 128*1024);
  ~Scheduler() = default;

  bool is_in_scheduler_thread() const {
    return m_tid == CurrentThread::get_tid();
  }

  void assert_in_scheduler_thread();

  void start();

  void quit();

  /**
   * @brief 移除执行权
   * 只能在所属的调度线程中执行
  */
  void yield();

  typedef std::function<void(Scheduler*)> Task;
  void create_coroutine(const Task &task);

private:
  static void schedule(void *arg);

  void handle_pending_tasks();

private:
  const size_t m_stackSize; // 栈空间大小
  const int m_tid;          // 所属的线程tid

  std::atomic<bool> m_quit; // 停止调度
  int m_coNum;              // 创建协程的个数
  int m_curCo;              // 记录当前正在运行的协程
  ucontext_t m_mainContext;
  std::map<int, std::unique_ptr<Coroutine>> m_coStore;

  std::mutex m_mutex;
  std::vector<Task> m_pendingTasks;
};

} // namespace co

#endif // SCHEDULER_H_