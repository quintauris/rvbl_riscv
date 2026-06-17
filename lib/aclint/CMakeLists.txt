#[[
  Copyright 2026 Quintauris GmbH
  Licensed under the Apache License, Version 2.0 (the "License").
  https://www.apache.org/licenses/LICENSE-2.0
]]

rvbl_static_library(
  NAME
  aclint
  SOURCES
  "${CMAKE_CURRENT_LIST_DIR}/source/rvbl_aclint.c"
  "${CMAKE_CURRENT_LIST_DIR}/include/rvbl/aclint/rvbl_aclint.h"
  INCLUDES
  "${CMAKE_CURRENT_LIST_DIR}/include"
  LIBRARIES
  machine
  abi)

rvbl_test(NAME aclint LIBRARIES interrupt aclint)
