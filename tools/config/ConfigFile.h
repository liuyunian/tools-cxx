#ifndef CONFIGFILE_H_
#define CONFIGFILE_H_

#include <string>
#include <sstream>
#include <map>

#include <assert.h>

#include "tools/base/noncopyable.h"
#include "tools/base/Exception.h"

class ConfigFile : noncopyable {
public:
  ConfigFile() = default;

  ~ConfigFile() = default;

  void load(const char* path);

  template<typename T>
  const T get(const std::string& itemName){
    auto iter = m_configItemStore.find(itemName);
    if(iter == m_configItemStore.end()){
      throw Exception("can't find in ConfigFile::get");
    }

    T retVal;
    if(std::is_same<T, bool>::value){
      std::stringstream(iter->second) >> std::boolalpha >> retVal;
    }
    else{
      std::stringstream(iter->second) >> retVal;
    }
    
    return retVal;
  }

private:
  void trim_space(std::string& string);

private:
  std::map<std::string, std::string> m_configItemStore;
};

#endif // CONFIGFILE_H_