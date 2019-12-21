ADD_EXECUTABLE(EchoClient EchoClient.cpp)
TARGET_LINK_LIBRARIES(EchoClient tools_socket tools_log tools_base)

ADD_EXECUTABLE(EchoServer EchoServer.cpp)
TARGET_LINK_LIBRARIES(EchoServer tools_socket tools_log tools_base)

ADD_EXECUTABLE(EchoServer_select EchoServer_select.cpp)
TARGET_LINK_LIBRARIES(EchoServer_select tools_socket tools_log tools_base)

ADD_EXECUTABLE(EchoServer_poll EchoServer_poll.cpp)
TARGET_LINK_LIBRARIES(EchoServer_poll tools_socket tools_log tools_base)

IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
  ADD_EXECUTABLE(EchoServer_epoll EchoServer_epoll.cpp)
  TARGET_LINK_LIBRARIES(EchoServer_epoll tools_socket tools_log tools_base)
ENDIF(CMAKE_SYSTEM_NAME MATCHES "Linux")

ADD_EXECUTABLE(EchoServer_coroutine EchoServer_coroutine.cpp)
TARGET_LINK_LIBRARIES(EchoServer_coroutine tools_co tools_socket tools_log tools_base pthread)