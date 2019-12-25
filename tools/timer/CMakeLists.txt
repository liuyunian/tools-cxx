SET(TIMER_SRC
  Timer.cpp
  TimerScheduler.cpp
  TimerSchedulerThread.cpp
)

SET(TIMER_HDR
  Timer.h
  TimerId.h
  TimerScheduler.h
  TimerSchedulerThread.h
)

# ============================= build =============================================
# static library
ADD_LIBRARY(tools_timer STATIC ${TIMER_SRC})

# ============================= install ==============================================
# libtools_timer.a -- install path = /usr/local/lib
INSTALL(TARGETS tools_timer ARCHIVE DESTINATION lib)

# *.h -- install path = /usr/local/include/tools/timer
INSTALL(FILES ${TIMER_HDR} DESTINATION include/tools/timer)