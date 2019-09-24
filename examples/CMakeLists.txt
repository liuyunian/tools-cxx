CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(Tools_cxx_test)

ADD_COMPILE_OPTIONS(-std=c++11)
INCLUDE_DIRECTORIES(/usr/local/include)
LINK_DIRECTORIES(/usr/local/lib)

ADD_EXECUTABLE(error_test error_test.cpp)
TARGET_LINK_LIBRARIES(error_test tools_error)

ADD_EXECUTABLE(Timestamp_test base_Timestamp_test.cpp)
TARGET_LINK_LIBRARIES(Timestamp_test tools_base)

ADD_EXECUTABLE(CurrentThread_test base_CurrentThread_test.cpp)
TARGET_LINK_LIBRARIES(CurrentThread_test tools_base)

ADD_EXECUTABLE(log_test log_test.cpp)
TARGET_LINK_LIBRARIES(log_test tools_log tools_base)

ADD_EXECUTABLE(timer_test timer_test.cpp)
TARGET_LINK_LIBRARIES(timer_test tools_timer pthread)

ADD_EXECUTABLE(alarm_test alarm_test.cpp)

ADD_EXECUTABLE(Singleton_test base_Singleton_test.cpp)
TARGET_LINK_LIBRARIES(Singleton_test tools_base pthread)

ADD_EXECUTABLE(config_test config_test.cpp)
TARGET_LINK_LIBRARIES(config_test tools_config tools_log tools_base)

ADD_EXECUTABLE(config_Singleton_test config_Singleton_test.cpp)
TARGET_LINK_LIBRARIES(config_Singleton_test tools_config tools_log tools_base pthread)