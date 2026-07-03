#[[
  Copyright 2026 Quintauris GmbH
  Licensed under the Apache License, Version 2.0 (the "License").
  https://www.apache.org/licenses/LICENSE-2.0
]]

rvbl_static_library(
  NAME
  interrupt
  SOURCES
  "${CMAKE_CURRENT_LIST_DIR}/source/\
rvbl_hart${rvbl_toolchain_specific_suffix}.s"
  "${CMAKE_CURRENT_LIST_DIR}/source/rvbl_hart.c"
  "${CMAKE_CURRENT_LIST_DIR}/include/rvbl/interrupt/rvbl_hart.h"
  INCLUDES
  "${CMAKE_CURRENT_LIST_DIR}/include"
  LIBRARIES
  abi
  machine
  alloc)

rvbl_test(NAME machine_exception LIBRARIES interrupt)
rvbl_test(NAME machine_interrupt LIBRARIES interrupt)
rvbl_test(NAME supervisor_exception LIBRARIES interrupt)
rvbl_test(NAME supervisor_interrupt LIBRARIES interrupt)
