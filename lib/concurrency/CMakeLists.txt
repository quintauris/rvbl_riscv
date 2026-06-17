#[[
  Copyright 2026 Quintauris GmbH
  Licensed under the Apache License, Version 2.0 (the "License").
  https://www.apache.org/licenses/LICENSE-2.0
]]

rvbl_static_library(
  NAME
  concurrency
  SOURCES
  "${CMAKE_CURRENT_LIST_DIR}/include/rvbl/concurrency/rvbl_spinlock.h"
  "${CMAKE_CURRENT_LIST_DIR}/source/rvbl_spinlock.c"
  INCLUDES
  "${CMAKE_CURRENT_LIST_DIR}/include"
  LIBRARIES
  atomic)

rvbl_test(NAME spinlock LIBRARIES concurrency)
