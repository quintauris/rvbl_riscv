/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#include "rvbl/abi/rvbl_abi.h"
#include "rvbl/interrupt/rvbl_hart.h"
#include "rvbl/machine/rvbl_machine.h"
#include "rvbl/test/rvbl_test.h"
#include "rvbl/type/rvbl_types.h"

void machine_ecall_handler(volatile rvbl_interrupt_hart_context *context)
{
    rvbl_uint32_t *count = (rvbl_uint32_t *)context->extra;

    context->program_counter += (XLEN / 8);
    ++(*count);
}

int main(void)
{
    rvbl_uint32_t count = 0;

    rvbl_hart_hang_if_not(0);
    rvbl_test_initialize();
    ASSERT_EQ(rvbl_alloc_initialize(&rvbl_test_allocator), rvbl_result_success);
    ASSERT_EQ(
        rvbl_interrupt_hart_current_initialize_traps(&rvbl_test_allocator, 0, NULL, &count),
        rvbl_result_success
    );
    ASSERT_EQ(
        rvbl_interrupt_hart_current_set_trap_handler(
            rvbl_interrupt_hart_privilege_m,
            RVBL_EXCEPTION(mcause_code_values_Exception_EnvironmentCallFromMMode),
            machine_ecall_handler
        ),
        rvbl_result_success
    );

    count = 0;
    ASSERT_EQ(rvbl_interrupt_hart_current_enable_traps(), rvbl_result_success);
    ASSERT_EQ(count, 0);
    rvbl_system_call_0(0);
    ASSERT_EQ(count, 1);
    ASSERT_EQ(rvbl_interrupt_hart_current_disable_traps(), rvbl_result_success);

    PASS();

    return 0;
}
