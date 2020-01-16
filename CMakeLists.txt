CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(TOOLS_CXX)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

# ============================= source ===================================
SET(SRC
  # base
  tools/base/CountDownLatch.cpp
  tools/base/CurrentThread.cpp
  tools/base/Exception.cpp
  tools/base/ProcessInfo.cpp
  tools/base/Timestamp.cpp 
  # config
  tools/config/ConfigFile.cpp
  # coroutine
  tools/coroutine/CoScheduler.cpp 
  tools/coroutine/CoSchedulerThread.cpp
  # log
  tools/log/log.cpp
  tools/log/LogBuffer.cpp
  tools/log/LogFile.cpp
  tools/log/LogPrint.cpp
  #poller
  tools/poller/Channel.cpp
  tools/poller/EPollPoller.cpp
  tools/poller/PollPoller.cpp
  tools/poller/Poller.cpp
  #socket
  tools/socket/ConnSocket.cpp
  tools/socket/InetAddress.cpp
  tools/socket/ServerSocket.cpp
  tools/socket/SocketsOps.cpp
  #threadpool
  tools/threadpool/ThreadPool.cpp
  # timer
  tools/timer/Timer.cpp
  tools/timer/TimerScheduler.cpp
  tools/timer/TimerSchedulerThread.cpp
)

SET(BASE_HDR
  tools/base/copyable.h
  tools/base/CountDownLatch.h
  tools/base/CurrentThread.h
  tools/base/Exception.h
  tools/base/noncopyable.h
  tools/base/ProcessInfo.h
  tools/base/Singleton.h
  tools/base/Timestamp.h
)

SET(CONFIG_HDR
  tools/config/ConfigFile.h
)

SET(COROUTINE_HDR
  tools/coroutine/Coroutine.h 
  tools/coroutine/CoScheduler.h 
  tools/coroutine/CoSchedulerThread.h
)


SET(LOG_HDR
  tools/log/log.h
  tools/log/LogFile.h
  tools/log/LogPrint.h
)

SET(POLLER_HDR
  tools/poller/Channel.h
  tools/poller/Poller.h
)

SET(SOCKET_HDR
  tools/socket/ConnSocket.h
  tools/socket/Endian.h
  tools/socket/InetAddress.h
  tools/socket/ServerSocket.h
  tools/socket/SocketsOps.h
)

SET(THREADPOOL_HDR
  tools/threadpool/ThreadPool.h
)

SET(TIMER_HDR
  tools/timer/Timer.h
  tools/timer/TimerId.h
  tools/timer/TimerScheduler.h
  tools/timer/TimerSchedulerThread.h
)

# ============================= build ====================================
# static library
ADD_LIBRARY(tools_cxx STATIC ${SRC})

# ============================= install ====================================
# libtools_cxx.a -- install path = /usr/local/lib
INSTALL(TARGETS tools_cxx ARCHIVE DESTINATION lib)

# *.h -- install path = /usr/local/include/tools/xx
INSTALL(FILES ${BASE_HDR} DESTINATION include/tools/base)
INSTALL(FILES ${CONFIG_HDR} DESTINATION include/tools/config)
INSTALL(FILES ${COROUTINE_HDR} DESTINATION include/tools/coroutine)
INSTALL(FILES ${LOG_HDR} DESTINATION include/tools/log)
INSTALL(FILES ${POLLER_HDR} DESTINATION include/tools/poller)
INSTALL(FILES ${SOCKET_HDR} DESTINATION include/tools/socket)
INSTALL(FILES ${THREADPOOL_HDR} DESTINATION include/tools/threadpool)
INSTALL(FILES ${TIMER_HDR} DESTINATION include/tools/timer)