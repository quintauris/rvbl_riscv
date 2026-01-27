rvbl_static_library(
  NAME
  memory
  SOURCES
  "${CMAKE_CURRENT_LIST_DIR}/include/rvbl/memory/rvbl_pmp.h"
  "${CMAKE_CURRENT_LIST_DIR}/include/rvbl/memory/rvbl_coherence.h"
  "${CMAKE_CURRENT_LIST_DIR}/source/rvbl_pmp.c"
  INCLUDES
  "${CMAKE_CURRENT_LIST_DIR}/include"
  LIBRARIES
  test)

rvbl_test(NAME pmp LIBRARIES memory)
rvbl_test(NAME coherence LIBRARIES memory)
rvbl_test(NAME cache LIBRARIES memory)
