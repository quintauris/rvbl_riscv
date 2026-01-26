rvbl_static_library(
  NAME
  aplic
  SOURCES
  "${CMAKE_CURRENT_LIST_DIR}/source/rvbl_aplic.c"
  "${CMAKE_CURRENT_LIST_DIR}/include/rvbl/aplic/rvbl_aplic.h"
  INCLUDES
  "${CMAKE_CURRENT_LIST_DIR}/include"
  LIBRARIES
  machine)

rvbl_test(NAME aplic LIBRARIES interrupt aplic)
