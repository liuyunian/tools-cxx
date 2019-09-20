# ============================= build =============================================
# static library
ADD_LIBRARY(tools_timer STATIC timer.cpp)

# ============================= install ==============================================
# libtools_timer.a -- install path = /usr/local/lib
INSTALL(TARGETS tools_timer ARCHIVE DESTINATION lib)

# *.h -- install path = /usr/local/include/tools_cxx
FILE(GLOB HEADERS "*.h")
INSTALL(FILES ${HEADERS} DESTINATION include/tools_cxx)