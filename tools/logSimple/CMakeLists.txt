# ============================= build =============================================
# static library
ADD_LIBRARY(tools_logSimple STATIC LogSimple.cpp)
TARGET_LINK_LIBRARIES(tools_logSimple tools_base)

# ============================= install ==============================================
# *.a -- install path = /usr/local/lib
INSTALL(TARGETS tools_logSimple ARCHIVE DESTINATION lib)

# *.h -- install path = /usr/local/include/tools/logSimple
INSTALL(FILES LogSimple.h DESTINATION include/tools/logSimple)