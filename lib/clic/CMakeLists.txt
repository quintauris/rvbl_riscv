rvbl_static_library(
  NAME
  clic
  SOURCES
  "${CMAKE_CURRENT_LIST_DIR}/source/rvbl_clic.c"
  "${CMAKE_CURRENT_LIST_DIR}/include/rvbl/clic/rvbl_clic.h"
  INCLUDES
  "${CMAKE_CURRENT_LIST_DIR}/include"
  LIBRARIES
  machine
  interrupt)

rvbl_test(NAME clic LIBRARIES abi interrupt clic)
