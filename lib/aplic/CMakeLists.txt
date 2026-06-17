#[[
  Copyright 2026 Quintauris GmbH
  Licensed under the Apache License, Version 2.0 (the "License").
  https://www.apache.org/licenses/LICENSE-2.0
]]

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
