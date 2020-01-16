#include <iostream>
#include <string>

// #include <stdlib.h>

#include <tools/log/log.h>
#include <tools/base/Exception.h>
#include <tools/config/ConfigFile.h>

int main(){
  ConfigFile cf;

  // std::cout << getenv("PWD") << std::endl;             // 通过查看环境变量来确定配置文件的路径应该如何指定
  const char *configFileName = "config/config_test.conf";

  try{
    cf.load(configFileName);
  }
  catch(const Exception &e){
    LOG_FATAL("%s", e.what());
  }

  std::cout << cf.get<std::string>("StringTest") << '\n';

  int int_val = cf.get<int>("IntTest");
  std::cout << int_val << '\n';

  double double_val = cf.get<double>("DoubleTest");
  std::cout << double_val << '\n';

  bool bool_val = cf.get<bool>("BoolTest");
  std::cout << std::boolalpha << bool_val << std::endl;

  return 0;
}