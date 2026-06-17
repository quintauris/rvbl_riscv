#[[
  Copyright 2026 Quintauris GmbH
  Licensed under the Apache License, Version 2.0 (the "License").
  https://www.apache.org/licenses/LICENSE-2.0
]]

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

if((CMAKE_C_COMPILER_ID MATCHES "GNU") OR (CMAKE_C_COMPILER_ID MATCHES "Clang"))
  target_compile_options(test_cache PRIVATE -Wno-pedantic)
endif()
