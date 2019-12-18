#include <vector>

#include <cxxabi.h>     // __cxa_demangle
#include <execinfo.h>   // backtrace backtrace_symbols

#include "tools/base/Exception.h"

Exception::Exception(const std::string& what) : 
  std::runtime_error(what)
{
  fill_stack_trace();
}

void Exception::fill_stack_trace(){
  const int maxFrames = 200;                            // 返回堆栈框架的最大个数
  void *frame[maxFrames];                               // 存放堆栈框架的的返回地址
  int nptrs = ::backtrace(frame, maxFrames);
  char **strings = ::backtrace_symbols(frame, nptrs);   // 字符串数组
  if(strings == nullptr){
    return;
  }

  size_t len = 256;           
  char *demangled = static_cast<char*>(::malloc(len));  // 用于存放demangled之后的结果
  for(int i = 1; i < nptrs; ++ i){
    char *leftPar = nullptr;                            // 左括号
    char *plus = nullptr;                               // 加号
    for(char *p = strings[i]; *p; ++ p){                // 找到左括号和加号的位置，两者之间的内容是需要demangle的
      if(*p == '(')
        leftPar = p;
      else if(*p == '+')
        plus = p;
    }

    *plus = '\0';
    int status = 0;
    char *ret = abi::__cxa_demangle(leftPar+1, demangled, &len, &status);
    *plus = '+';
    if(status != 0){
      m_stack.append(strings[i]);
    }
    else{
      demangled = ret;
      m_stack.append(strings[i], leftPar+1);
      m_stack.append(demangled);
      m_stack.append(plus);
    }

    m_stack.push_back('\n');
  }

  free(demangled);
  free(strings);
}