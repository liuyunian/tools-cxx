set(POLLER_SRC 
  Channel.cpp
  EPollPoller.cpp
  PollPoller.cpp
  Poller.cpp
)

set(POLLER_HDS 
  Channel.h
  Poller.h
)

# ============================= build =============================================
# static library
ADD_LIBRARY(tools_poller STATIC ${POLLER_SRC})
TARGET_LINK_LIBRARIES(tools_poller tools_log tools_base)

# ============================= install ==============================================
# libtools_socket.a -- install path = /usr/local/lib
INSTALL(TARGETS tools_poller ARCHIVE DESTINATION lib)

# *.h -- install path = /usr/local/include/tools/poller
INSTALL(FILES ${POLLER_HDS} DESTINATION include/tools/poller)