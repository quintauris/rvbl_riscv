#[[
  Copyright 2026 Quintauris GmbH
  Licensed under the Apache License, Version 2.0 (the "License").
  https://www.apache.org/licenses/LICENSE-2.0
]]

rvbl_static_library(
  NAME
  clic
  SOURCES
  "${CMAKE_CURRENT_LIST_DIR}/source/rvbl_clic.c"
  "${CMAKE_CURRENT_LIST_DIR}/source/\
rvbl_clic${rvbl_toolchain_specific_suffix}.s"
  "${CMAKE_CURRENT_LIST_DIR}/include/rvbl/clic/rvbl_clic.h"
  INCLUDES
  "${CMAKE_CURRENT_LIST_DIR}/include"
  LIBRARIES
  machine
  interrupt
  alloc_bump)

rvbl_test(NAME clic LIBRARIES abi interrupt clic)
