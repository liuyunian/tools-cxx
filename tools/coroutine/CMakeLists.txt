# ============================= build =============================================
# static library
ADD_LIBRARY(tools_co STATIC Scheduler.cpp SchedulerThread.cpp)

# ============================= install ==============================================
# libtools_co.a -- install path = /usr/local/lib
INSTALL(TARGETS tools_co ARCHIVE DESTINATION lib)

# *.h -- install path = /usr/local/include/tools/coroutine
INSTALL(FILES Scheduler.h SchedulerThread.h DESTINATION include/tools/coroutine)