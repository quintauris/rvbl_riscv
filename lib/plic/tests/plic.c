/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#include "rvbl/hardware/rvbl_hardware.h"
#include "rvbl/interrupt/rvbl_hart.h"
#include "rvbl/machine/rvbl_machine.h"
#include "rvbl/plic/rvbl_plic.h"
#include "rvbl/test/rvbl_test.h"

static volatile rvbl_uint32_t count = 0;
static rvbl_plic_interrupt_source_t source = 1;

void machine_software_interrupt_handler(volatile rvbl_interrupt_hart_context *context)
{
    (void)context;
    rvbl_plic_interrupt_source_t claimed = 0;
    ++count;
    rvbl_plic_claim_interrupt(&rvbl_plic_instance_default, 0, &claimed);
    ASSERT_EQ(claimed, source);
    rvbl_plic_complete_interrupt(&rvbl_plic_instance_default, 0, claimed);
}

int main(void)
{
    rvbl_plic_interrupt_priority_t priority = 0;
    rvbl_bool_t enabled;

    rvbl_hart_hang_if_not(0);
    rvbl_test_initialize();
    ASSERT_EQ(rvbl_alloc_initialize(&rvbl_test_allocator), rvbl_result_success);

    source = RVBL_PLIC_MIN_INTERRUPT_SOURCE_ID;

    ASSERT_EQ(RVBL_REGISTER_READ(plic, registers, &rvbl_plic_instance_default, reserved), 0);

    ASSERT_EQ(
        rvbl_interrupt_hart_current_initialize_traps(&rvbl_test_allocator, 0, NULL, NULL),
        rvbl_result_success
    );
    ASSERT_EQ(
        rvbl_interrupt_hart_current_set_trap_handler(
            rvbl_interrupt_hart_privilege_m,
            RVBL_INTERRUPT(rvbl_riscv_hart_privileged_mcause_code_values_Interrupt_MachineExternal),
            machine_software_interrupt_handler
        ),
        rvbl_result_success
    );

    RVBL_REGISTER_FIELD_SET(riscv_hart, privileged, &rvbl_riscv_hart_instance_0, mie, meie);
    ASSERT_EQ(rvbl_interrupt_hart_current_enable_traps(), rvbl_result_success);
    ASSERT_EQ(count, 0);

    ASSERT_EQ(
        rvbl_plic_get_interrupt_priority(&rvbl_plic_instance_default, source, &priority),
        rvbl_result_success
    );
    ASSERT_EQ(
        rvbl_plic_set_interrupt_priority(&rvbl_plic_instance_default, source, 1),
        rvbl_result_success
    );
    ASSERT_EQ(
        rvbl_plic_get_interrupt_priority(&rvbl_plic_instance_default, source, &priority),
        rvbl_result_success
    );
    ASSERT_EQ(priority, 1);
    priority = 0;

    ASSERT_EQ(
        rvbl_plic_get_interrupt_enable(&rvbl_plic_instance_default, source, 0, &enabled),
        rvbl_result_success
    );
    ASSERT_EQ(
        rvbl_plic_set_interrupt_enable(&rvbl_plic_instance_default, source, 0, rvbl_true),
        rvbl_result_success
    );
    ASSERT_EQ(
        rvbl_plic_get_interrupt_enable(&rvbl_plic_instance_default, source, 0, &enabled),
        rvbl_result_success
    );
    ASSERT_EQ(enabled, rvbl_true);
    enabled = rvbl_false;

    ASSERT_EQ(count, 0);
    ASSERT_EQ(
        rvbl_plic_get_interrupt_pending(&rvbl_plic_instance_default, source, &enabled),
        rvbl_result_success
    );
    ASSERT_EQ(enabled, rvbl_false);

    if (rvbl_plic_instance_default.interrupt_pending_writable_parameter) {
        ASSERT_EQ(
            rvbl_plic_set_interrupt_pending(&rvbl_plic_instance_default, source),
            rvbl_result_success
        );

        while (count == 0) {
            __asm__ volatile("wfi\n\t");
        }

        ASSERT_EQ(
            rvbl_plic_get_interrupt_pending(&rvbl_plic_instance_default, source, &enabled),
            rvbl_result_success
        );
        ASSERT_EQ(
            enabled, rvbl_false
        ); // handler above has claimed/completed it, so interrupt shall no longer be pending
        ASSERT_EQ(count, 1);
    }

    ASSERT_EQ(rvbl_interrupt_hart_current_disable_traps(), rvbl_result_success);
    RVBL_REGISTER_FIELD_CLEAR(riscv_hart, privileged, &rvbl_riscv_hart_instance_0, mie, meie);

    PASS();

    return 0;
}
