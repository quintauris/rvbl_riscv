#[[
  Copyright 2026 Quintauris GmbH
  Licensed under the Apache License, Version 2.0 (the "License").
  https://www.apache.org/licenses/LICENSE-2.0
]]

rvbl_static_library(
  NAME
  plic
  SOURCES
  "${CMAKE_CURRENT_LIST_DIR}/source/rvbl_plic.c"
  "${CMAKE_CURRENT_LIST_DIR}/include/rvbl/plic/rvbl_plic.h"
  INCLUDES
  "${CMAKE_CURRENT_LIST_DIR}/include"
  LIBRARIES
  machine)

rvbl_test(NAME plic LIBRARIES interrupt plic)
