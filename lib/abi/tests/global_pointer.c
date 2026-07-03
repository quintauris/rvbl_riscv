/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#include "rvbl/abi/rvbl_abi.h"
#include "rvbl/compiler/rvbl_compiler.h"
#include "rvbl/test/rvbl_test.h"
#include "rvbl/type/rvbl_types.h"

int main(void)
{
    register const rvbl_pointer_t current = rvbl_global_pointer_register_read();

    rvbl_hart_hang_if_not(0);
    rvbl_test_initialize();

    rvbl_global_pointer_register_write(NULL);
    ASSERT_EQ(rvbl_global_pointer_register_read(), NULL);

    rvbl_global_pointer_register_write((rvbl_pointer_t)0xDEADFACE);
    ASSERT_EQ(rvbl_global_pointer_register_read(), (rvbl_pointer_t)0xDEADFACE);

    rvbl_global_pointer_register_write((rvbl_pointer_t)0xFFFFFFFF);
    ASSERT_EQ(rvbl_global_pointer_register_read(), (rvbl_pointer_t)0xFFFFFFFF);

    rvbl_global_pointer_register_write(current);

    PASS();

    return 0;
}
