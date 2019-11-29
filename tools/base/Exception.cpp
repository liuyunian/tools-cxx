#include "tools/base/Exception.h"

Exception::Exception(const std::string& what) : 
  std::runtime_error(what)
{
  
}