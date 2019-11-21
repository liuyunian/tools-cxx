#ifndef PROCESSINFO_H_
#define PROCESSINFO_H_

#include <string>

namespace ProcessInfo {

pid_t get_pid();

std::string get_pid_string();

std::string get_hostname();

} // namespace ProcessInfos

#endif // PROCESSINFO_H_