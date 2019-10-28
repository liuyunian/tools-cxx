# ============================= build =============================================
# static library
SET(SRC 
    log.cpp
    LogBuffer.cpp
    LogPrint.cpp)

ADD_LIBRARY(tools_log STATIC ${SRC})
TARGET_LINK_LIBRARIES(tools_log tools_base)

# ============================= install ==============================================
# *.a -- install path = /usr/local/lib
INSTALL(TARGETS tools_log ARCHIVE DESTINATION lib)

# *.h -- install path = /usr/local/include/tools/log
FILE(GLOB HEADERS "*.h")
INSTALL(FILES ${HEADERS} DESTINATION include/tools/log)