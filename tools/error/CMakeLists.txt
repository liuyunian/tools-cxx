# ============================= build =============================================
# static library
ADD_LIBRARY(tools_error STATIC error.cpp)

# ============================= install ==============================================
# *.a -- install path = /usr/local/lib
INSTALL(TARGETS tools_error ARCHIVE DESTINATION lib)

# *.h -- install path = /usr/local/include/tools/error
FILE(GLOB HEADERS "*.h")
INSTALL(FILES ${HEADERS} DESTINATION include/tools/error)