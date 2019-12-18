#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <stdexcept>

#include "tools/base/noncopyable.h"

class Exception : public std::runtime_error
{
public:
  Exception(const std::string& what);
  ~Exception() noexcept override = default;

  std::string stack_trace() const noexcept {
    return m_stack;
  }

private:
  void fill_stack_trace();

private:
  std::string m_stack;
};

#endif // EXCEPTION_H_