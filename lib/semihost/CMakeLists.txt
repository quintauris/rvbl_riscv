#[[
  Copyright 2026 Quintauris GmbH
  Licensed under the Apache License, Version 2.0 (the "License").
  https://www.apache.org/licenses/LICENSE-2.0
]]

rvbl_static_library(
  NAME
  semihost
  SOURCES
  "${CMAKE_CURRENT_LIST_DIR}/source/\
rvbl_semihost${rvbl_toolchain_specific_suffix}.s"
  "${CMAKE_CURRENT_LIST_DIR}/source/rvbl_semihost.c"
  "${CMAKE_CURRENT_LIST_DIR}/include/rvbl/semihost/rvbl_semihost.h"
  INCLUDES
  "${CMAKE_CURRENT_LIST_DIR}/include"
  LIBRARIES
  type
  c)

rvbl_test(NAME semihost LIBRARIES semihost)
