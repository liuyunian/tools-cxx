#include <iostream>
#include <string>

#include <tools/log/log.h>
#include <tools/config/ConfigFile.h>

int main(){
    ConfigFile cf;

    const char * configFileName = "config_test.conf";
    if(cf.load(configFileName)){
        LOG_FATAL("Failed to load config file: %s", configFileName);
    }

    std::cout << cf.get<std::string>("StringTest") << std::endl;

    int int_val = cf.get<int>("IntTest");
    std::cout << int_val << std::endl;

    double double_val = cf.get<double>("DoubleTest");
    std::cout << double_val << std::endl;

    return 0;
}