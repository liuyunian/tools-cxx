# ============================= source ============================================
SET(base_srcs 
    Timestamp.cpp 
    CurrentThread.cpp
)

# ============================= build =============================================
# static library
ADD_LIBRARY(tools_base STATIC ${base_srcs})

# ============================= install ===========================================
# *.a -- install path = /usr/local/lib
INSTALL(TARGETS tools_base ARCHIVE DESTINATION lib)

# *.h -- install path = /usr/local/include/tools_cxx
FILE(GLOB HEADERS "*.h")
INSTALL(FILES ${HEADERS} DESTINATION include/tools_cxx)