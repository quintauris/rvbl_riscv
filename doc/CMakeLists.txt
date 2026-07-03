#[[
  Copyright 2026 Quintauris GmbH
  Licensed under the Apache License, Version 2.0 (the "License").
  https://www.apache.org/licenses/LICENSE-2.0
]]

cmake_minimum_required(VERSION 3.28)
project(core_doc)

include(../../core/cmake/support.cmake)

rvbl_layer_documentation(
  SOURCE_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}
  TARGET_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/html
  RVBL_ROOT
  ${CMAKE_CURRENT_SOURCE_DIR}/../..
  DEVICE_MODELS
  ${CMAKE_CURRENT_SOURCE_DIR}/../model/aclint/model.json
  ${CMAKE_CURRENT_SOURCE_DIR}/../model/aplic/model.json
  ${CMAKE_CURRENT_SOURCE_DIR}/../model/imsic/model.json
  ${CMAKE_CURRENT_SOURCE_DIR}/../model/plic/model.json
  ${CMAKE_CURRENT_SOURCE_DIR}/../model/clic/model.json
  REGISTER_SET_MODELS
  ${CMAKE_CURRENT_SOURCE_DIR}/../model/privileged/model.json
  ${CMAKE_CURRENT_SOURCE_DIR}/../model/privileged/model_32.json
  ${CMAKE_CURRENT_SOURCE_DIR}/../model/privileged/supervisor.json
  ${CMAKE_CURRENT_SOURCE_DIR}/../model/privileged/supervisor_32.json
  ${CMAKE_CURRENT_SOURCE_DIR}/../model/privileged/rnmi.json
  ${CMAKE_CURRENT_SOURCE_DIR}/../model/imsic/machine.json
  ${CMAKE_CURRENT_SOURCE_DIR}/../model/imsic/supervisor.json
  ${CMAKE_CURRENT_SOURCE_DIR}/../model/clic/registers.json
  PARAMETERS
  PAGE_SIZE=4096)
