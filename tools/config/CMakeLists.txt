# ============================= build =============================================
# static library
ADD_LIBRARY(tools_config STATIC ConfigFile.cpp)
TARGET_LINK_LIBRARIES(tools_config tools_log tools_base)

# ============================= install ==============================================
# libtools_config.a -- install path = /usr/local/lib
INSTALL(TARGETS tools_config ARCHIVE DESTINATION lib)

# *.h -- install path = /usr/local/include/tools_cxx
FILE(GLOB HEADERS "*.h")
INSTALL(FILES ${HEADERS} DESTINATION include/tools_cxx)