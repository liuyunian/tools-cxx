# ============================= build =============================================
# static library
ADD_LIBRARY(tools_threadpool STATIC ThreadPool.cpp)
TARGET_LINK_LIBRARIES(tools_threadpool tools_log pthread)

# ============================= install ==============================================
# libtools_threadpool.a -- install path = /usr/local/lib
INSTALL(TARGETS tools_threadpool ARCHIVE DESTINATION lib)

# *.h -- install path = /usr/local/include/tools_cxx
FILE(GLOB HEADERS "*.h")
INSTALL(FILES ${HEADERS} DESTINATION include/tools_cxx)