rvbl_static_library(
    NAME aclint
    SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/source/rvbl_aclint.c"
        "${CMAKE_CURRENT_LIST_DIR}/include/rvbl/aclint/rvbl_aclint.h"
    INCLUDES
        "${CMAKE_CURRENT_LIST_DIR}/include"
    LIBRARIES
        machine 
)

rvbl_test(NAME aclint LIBRARIES interrupt aclint)