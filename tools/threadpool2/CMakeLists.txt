# ============================= build =============================================
# static library
ADD_LIBRARY(tools_threadpool2 STATIC ThreadPool2.cpp)
TARGET_LINK_LIBRARIES(tools_threadpool2 pthread)

# ============================= install ==============================================
# libtools_threadpool2.a -- install path = /usr/local/lib
INSTALL(TARGETS tools_threadpool2 ARCHIVE DESTINATION lib)

# *.h -- install path = /usr/local/include/tools_cxx
FILE(GLOB HEADERS "*.h")
INSTALL(FILES ${HEADERS} DESTINATION include/tools_cxx)