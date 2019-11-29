#include <memory>

#include <assert.h>

#include "tools/coroutine/Scheduler.h"

namespace co {

__thread Scheduler* tScheduler = nullptr;

Scheduler::Scheduler(size_t stackSize) : 
  m_stackSize(stackSize),
  m_tid(CurrentThread::get_tid()),
  m_quit(false),
  m_coNum(0),
  m_curCo(-1)
{
  assert(tScheduler == nullptr);

  tScheduler = this;
}

void Scheduler::start(){
  m_quit = false;

  while(!m_quit){
    for(auto pos = m_coStore.begin(); pos != m_coStore.end();){
      switch(pos->second->get_status()){
        case Coroutine::READY: {
          ucontext_t *context = pos->second->get_contextptr();
          ::getcontext(context);
          context->uc_stack.ss_sp = pos->second->get_stack();
          context->uc_stack.ss_size = m_stackSize;
          context->uc_link = &m_mainContext;

          m_curCo = pos->first;
          pos->second->set_status(Coroutine::RUNNING);
          ::makecontext(context, reinterpret_cast<void(*)()>(schedule), 1, this);
          ::swapcontext(&m_mainContext, context);

          break;
        }

        case Coroutine::SUSPENDED: {
          m_curCo = pos->first;
          pos->second->set_status(Coroutine::RUNNING);
          ::swapcontext(&m_mainContext, pos->second->get_contextptr());

          break;
        }

        default: 
          break;
      }

      if(m_curCo == -1 && pos->second->get_status() == Coroutine::FINISHED){
        pos = m_coStore.erase(pos);
      }
      else{
        ++ pos;
      }
    }

    handle_pending_tasks();
  }
}

void Scheduler::quit(){
  m_quit = true;
}

void Scheduler::yield(){
  if(m_curCo == -1){
    return;
  }

  auto iter = m_coStore.find(m_curCo);
  assert(iter != m_coStore.end());

  iter->second->set_status(Coroutine::SUSPENDED);
  ::swapcontext(iter->second->get_contextptr(), &m_mainContext);
}

void Scheduler::create_coroutine(const Task &task){
  if(!is_in_scheduler_thread() || m_curCo != -1){             // 在调度线程外或者协程中执行创建协程任务
    {
      std::unique_lock<std::mutex> ul(m_mutex);
      m_pendingTasks.push_back(task);
    }
  }
  else{
    auto co = std::make_unique<Coroutine>(m_stackSize, task);
    m_coStore[m_coNum] = std::move(co);
    ++ m_coNum;
  }
}

void Scheduler::schedule(void *arg){
  Scheduler *sd = static_cast<Scheduler*>(arg);
  auto iter = sd->m_coStore.find(sd->m_curCo);
  assert(iter != sd->m_coStore.end());
  iter->second->m_task(sd);

  sd->m_curCo = -1;
  iter->second->set_status(Coroutine::FINISHED);
}

void Scheduler::handle_pending_tasks(){
  std::vector<Task> tasks;

  {
    std::unique_lock<std::mutex> ul(m_mutex);
    tasks.swap(m_pendingTasks);
  }

  for(auto &task : tasks){
    auto co = std::make_unique<Coroutine>(m_stackSize, task);
    m_coStore[m_coNum] = std::move(co);
    ++ m_coNum;
  }
}

} // namespace co