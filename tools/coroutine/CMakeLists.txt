SET(CO_SRC
  CoScheduler.cpp 
  CoSchedulerThread.cpp
)

SET(CO_HDS
  Coroutine.h
  CoScheduler.h 
  CoSchedulerThread.h
)


# ============================= build =============================================
# static library
ADD_LIBRARY(tools_co STATIC ${CO_SRC})

# ============================= install ==============================================
# libtools_co.a -- install path = /usr/local/lib
INSTALL(TARGETS tools_co ARCHIVE DESTINATION lib)

# *.h -- install path = /usr/local/include/tools/coroutine
INSTALL(FILES ${CO_HDS} DESTINATION include/tools/coroutine)