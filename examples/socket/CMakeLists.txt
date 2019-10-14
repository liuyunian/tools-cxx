ADD_EXECUTABLE(Endian_test Endian_test.cpp)
TARGET_LINK_LIBRARIES(Endian_test tools_socket tools_log tools_base)

ADD_EXECUTABLE(InetAddress_test InetAddress_test.cpp)
TARGET_LINK_LIBRARIES(InetAddress_test tools_socket tools_log tools_base)

ADD_EXECUTABLE(echoClient_test echoClient_test.cpp)
TARGET_LINK_LIBRARIES(echoClient_test tools_socket tools_log tools_base)

ADD_EXECUTABLE(echoServer_test echoServer_test.cpp)
TARGET_LINK_LIBRARIES(echoServer_test tools_socket tools_log tools_base)

ADD_EXECUTABLE(echoServer_poll_test echoServer_poll_test.cpp)
TARGET_LINK_LIBRARIES(echoServer_poll_test tools_socket tools_log tools_base)

IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
    ADD_EXECUTABLE(echoServer_epoll_test echoServer_epoll_test.cpp)
    TARGET_LINK_LIBRARIES(echoServer_epoll_test tools_socket tools_log tools_base)
ENDIF(CMAKE_SYSTEM_NAME MATCHES "Linux")