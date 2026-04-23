rvbl_static_library(
  NAME
  abi
  SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/include/rvbl/abi/rvbl_abi.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/\
rvbl_abi${rvbl_toolchain_specific_suffix}.s"
  INCLUDES
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  LIBRARIES
  type)

rvbl_test(NAME global_pointer LIBRARIES abi)
rvbl_test(NAME stack_pointer LIBRARIES abi)
rvbl_test(NAME thread_pointer LIBRARIES abi)
rvbl_test(NAME system_call LIBRARIES abi)
