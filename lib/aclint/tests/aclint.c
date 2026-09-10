/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#include "rvbl/aclint/rvbl_aclint.h"
#include "rvbl/interrupt/rvbl_hart.h"
#include "rvbl/machine/rvbl_configuration.h"
#include "rvbl/machine/rvbl_machine.h"
#include "rvbl/test/rvbl_test.h"
#include "rvbl/type/rvbl_types.h"

static volatile rvbl_uint32_t count = 0;

void machine_timer_handler(volatile rvbl_interrupt_hart_context *context)
{
    (void)context;

    ++count;
    rvbl_aclint_set_compare(
        &rvbl_aclint_instance_default,
        0,
        RVBL_REGISTER_READ(aclint, mtimer_time, &rvbl_aclint_instance_default, mtime) +
            RVBL_CONFIGURATION_MTIME_FREQUENCY
    );
}

int main(void)
{
    rvbl_hart_hang_if_not(0);
    rvbl_test_initialize();
    ASSERT_EQ(rvbl_alloc_initialize(&rvbl_test_allocator), rvbl_result_success);

    ASSERT_EQ(
        rvbl_interrupt_hart_current_initialize_traps(&rvbl_test_allocator, 0, NULL, NULL),
        rvbl_result_success
    );
    ASSERT_EQ(
        rvbl_interrupt_hart_current_set_trap_handler(
            rvbl_interrupt_hart_privilege_m,
            RVBL_INTERRUPT(rvbl_riscv_hart_privileged_mcause_code_values_Interrupt_MachineTimer),
            machine_timer_handler
        ),
        rvbl_result_success
    );

    rvbl_aclint_set_compare(&rvbl_aclint_instance_default, 0, 0);
    rvbl_aclint_set_compare(
        &rvbl_aclint_instance_default,
        0,
        RVBL_REGISTER_READ(aclint, mtimer_time, &rvbl_aclint_instance_default, mtime) +
            RVBL_CONFIGURATION_MTIME_FREQUENCY
    );
    count = 0;
    RVBL_REGISTER_FIELD_SET(riscv_hart, privileged, &rvbl_riscv_hart_instance_0, mie, mtie);
    ASSERT_EQ(rvbl_interrupt_hart_current_enable_traps(), rvbl_result_success);
    ASSERT_EQ(count, 0);

    while (count == 0) {
        __asm__ volatile("add zero, zero, zero\n\t");
    }

    ASSERT_EQ(count, 1);
    ASSERT_EQ(rvbl_interrupt_hart_current_disable_traps(), rvbl_result_success);
    RVBL_REGISTER_FIELD_CLEAR(riscv_hart, privileged, &rvbl_riscv_hart_instance_0, mie, mtie);

    PASS();

    return 0;
}
