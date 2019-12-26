SET(SOCKET_SRC
  ConnSocket.cpp
  InetAddress.cpp
  ServerSocket.cpp
  SocketsOps.cpp
)

SET(SOCKET_HDS
  ConnSocket.h
  Endian.h
  InetAddress.h
  ServerSocket.h
  SocketsOps.h
)
# ============================= build =============================================
ADD_LIBRARY(tools_socket STATIC ${SOCKET_SRC})
TARGET_LINK_LIBRARIES(tools_socket tools_log tools_base)

# ============================= install ==============================================
# libtools_socket.a -- install path = /usr/local/lib
INSTALL(TARGETS tools_socket ARCHIVE DESTINATION lib)

# *.h -- install path = /usr/local/include/tools/socket
INSTALL(FILES ${SOCKET_HDS} DESTINATION include/tools/socket)