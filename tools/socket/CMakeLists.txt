# ============================= build =============================================
# static library
set(SOCKET_SRC 
    SocketsOps.cpp
    InetAddress.cpp
    Socket.cpp
    ServerSocket.cpp)

ADD_LIBRARY(tools_socket STATIC ${SOCKET_SRC})
TARGET_LINK_LIBRARIES(tools_socket tools_log tools_base)

# ============================= install ==============================================
# libtools_socket.a -- install path = /usr/local/lib
INSTALL(TARGETS tools_socket ARCHIVE DESTINATION lib)

# *.h -- install path = /usr/local/include/tools/socket
FILE(GLOB HEADERS "*.h")
INSTALL(FILES ${HEADERS} DESTINATION include/tools/socket)