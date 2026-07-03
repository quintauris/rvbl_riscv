/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#include "rvbl/interrupt/rvbl_hart.h"
#include "rvbl/test/rvbl_test.h"

void interrupt_handler(volatile rvbl_interrupt_hart_context *context)
{
    rvbl_uint32_t *count = (rvbl_uint32_t *)context->extra;

    rvbl_test_machine_interrupt_control.cleanup_interrupt();

    ++(*count);
}

int main(void)
{
#ifndef _RT_EUROPA_MG_H_
    rvbl_uint32_t count = 0;
#endif

    rvbl_hart_hang_if_not(0);
    rvbl_test_initialize();
    ASSERT_EQ(rvbl_alloc_initialize(&rvbl_test_allocator), rvbl_result_success);

    // RT-Europa Mg can't seem to be able to return from a machine trap, as in,
    // it can't execute an MRET instruction properly. Thus, this test is disabled.
    //
    // See https://quintauris.atlassian.net/browse/RTE-67.
#ifndef _RT_EUROPA_MG_H_
    if (rvbl_test_machine_interrupt_control.can_trigger_interrupt) {
        ASSERT_EQ(
            rvbl_interrupt_hart_current_initialize_traps(&rvbl_test_allocator, 0, NULL, &count),
            rvbl_result_success
        );
        ASSERT_EQ(
            rvbl_interrupt_hart_current_set_trap_handler(
                rvbl_interrupt_hart_privilege_m,
                RVBL_INTERRUPT(rvbl_test_machine_interrupt_control.interrupt_code),
                interrupt_handler
            ),
            rvbl_result_success
        );

        count = 0;

        rvbl_test_machine_interrupt_control.prepare_interrupt();
        ASSERT_EQ(rvbl_interrupt_hart_current_enable_traps(), rvbl_result_success);
        ASSERT_EQ(count, 0);

        rvbl_test_machine_interrupt_control.trigger_interrupt();

        while (count == 0) {
            __asm__ volatile("wfi\n\t");
        }

        ASSERT_EQ(count, 1);
        ASSERT_EQ(rvbl_interrupt_hart_current_disable_traps(), rvbl_result_success);

        PASS();
    } else
#endif
    {
        SKIP();
    }

    return 0;
}
