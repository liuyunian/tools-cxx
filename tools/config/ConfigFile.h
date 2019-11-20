#ifndef CONFIGFILE_H_
#define CONFIGFILE_H_

#include <string>
#include <sstream>
#include <map>

#include <assert.h>

#include "tools/base/noncopyable.h"

class ConfigFile : noncopyable {
public:
    ConfigFile() = default;

    ~ConfigFile() = default;

    bool load(const char* path);

    template<typename T>
    const T get(const std::string& itemName){
        auto iter = m_configItemStore.find(itemName);
        assert(iter != m_configItemStore.end());

        T retVal;
        std::stringstream(iter->second) >> retVal;

        return retVal;
    }

private:
    void trim_space(std::string& string);

private:
    std::map<std::string, std::string> m_configItemStore;
};

#endif // CONFIGFILE_H_