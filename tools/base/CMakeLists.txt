# ============================= source ============================================
SET(base_srcs 
    Timestamp.cpp 
    CountDownLatch.cpp
    CurrentThread.cpp
)

# ============================= build =============================================
# static library
ADD_LIBRARY(tools_base STATIC ${base_srcs})

# ============================= install ===========================================
# *.a -- install path = /usr/local/lib
INSTALL(TARGETS tools_base ARCHIVE DESTINATION lib)

# *.h -- install path = /usr/local/include/tools/base
FILE(GLOB HEADERS "*.h")
INSTALL(FILES ${HEADERS} DESTINATION include/tools/base)