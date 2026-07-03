#[[
  Copyright 2026 Quintauris GmbH
  Licensed under the Apache License, Version 2.0 (the "License").
  https://www.apache.org/licenses/LICENSE-2.0
]]

rvbl_static_library(
  NAME
  imsic
  SOURCES
  "${CMAKE_CURRENT_LIST_DIR}/source/rvbl_hart.c"
  "${CMAKE_CURRENT_LIST_DIR}/source/rvbl_file.c"
  "${CMAKE_CURRENT_LIST_DIR}/include/rvbl/imsic/rvbl_common.h"
  "${CMAKE_CURRENT_LIST_DIR}/include/rvbl/imsic/rvbl_hart.h"
  "${CMAKE_CURRENT_LIST_DIR}/include/rvbl/imsic/rvbl_file.h"
  INCLUDES
  "${CMAKE_CURRENT_LIST_DIR}/include"
  LIBRARIES
  machine)

rvbl_test(NAME imsic LIBRARIES interrupt imsic)
