rvbl_static_library(
  NAME
  atomic
  SOURCES
  "${CMAKE_CURRENT_LIST_DIR}/include/rvbl/atomic/rvbl_atomic.h"
  "${CMAKE_CURRENT_LIST_DIR}/source/rvbl_atomic\
${rvbl_toolchain_specific_suffix}.s"
  INCLUDES
  "${CMAKE_CURRENT_LIST_DIR}/include"
  LIBRARIES
  machine)

rvbl_test(NAME add LIBRARIES atomic)
rvbl_test(NAME atomic LIBRARIES atomic)
