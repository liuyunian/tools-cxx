ADD_EXECUTABLE(TimerScheduler_test TimerScheduler_test.cpp)
TARGET_LINK_LIBRARIES(TimerScheduler_test tools_cxx)

ADD_EXECUTABLE(TimerSchedulerThread_test TimerSchedulerThread_test.cpp)
TARGET_LINK_LIBRARIES(TimerSchedulerThread_test tools_cxx pthread)

ADD_EXECUTABLE(alarm_test alarm_test.cpp)