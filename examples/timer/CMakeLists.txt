ADD_EXECUTABLE(timer_test timer_test.cpp)
TARGET_LINK_LIBRARIES(timer_test tools_timer pthread)

ADD_EXECUTABLE(alarm_test alarm_test.cpp)