/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#include "rvbl/abi/rvbl_abi.h"
#include "rvbl/boot/rvbl_extension.h"
#include "rvbl/interrupt/rvbl_hart.h"
#include "rvbl/machine/rvbl_machine.h"
#include "rvbl/test/rvbl_test.h"
#include "rvbl/type/rvbl_types.h"

typedef enum rvbl_riscv_hart_privileged_32_mstatus_mpp_t privilege;

#ifndef _RT_EUROPA_MG_H_
static rvbl_uint32_t supervisor_count = 0;
static privilege expected_privilege = rvbl_riscv_hart_privileged_32_mstatus_mpp_values_machine;
#endif

#ifndef _RT_EUROPA_MG_H_
static void machine_environment_call_handler(volatile rvbl_interrupt_hart_context *context)
{
    const privilege parameter = (privilege)context->arguments[0];
    ASSERT_EQ(
        RVBL_REGISTER_FIELD_READ(
            riscv_hart, privileged_32, &rvbl_riscv_hart_instance_0, mstatus, mpp
        ),
        expected_privilege
    );

    context->program_counter += (XLEN / 8);
    RVBL_REGISTER_FIELD_WRITE(
        riscv_hart, privileged_32, &rvbl_riscv_hart_instance_0, mstatus, mpp, parameter
    )
}

static void supervisor_store_fault_handler(volatile rvbl_interrupt_hart_context *context)
{
    context->program_counter += (XLEN / 8);
    ++supervisor_count;
}

static void unsupported_exception_handler(volatile rvbl_interrupt_hart_context *context)
{
    (void)context;
    FAIL();
}
#endif

int main(void)
{
    rvbl_hart_hang_if_not(0);
    rvbl_test_initialize();
    ASSERT_EQ(rvbl_alloc_initialize(&rvbl_test_allocator), rvbl_result_success);

    // RT-Europa Mg can't seem to be able to return from a supervisor trap, as in,
    // it can't execute an SRET instruction properly. Thus, this test is disabled.
    //
    // See https://quintauris.atlassian.net/browse/RTE-67.
#ifndef _RT_EUROPA_MG_H_
    if (rvbl_extension_present(rvbl_riscv_hart_privileged_misa_extensions_values_SupervisorMode)) {
        ASSERT_EQ(
            rvbl_interrupt_hart_current_initialize_traps(
                &rvbl_test_allocator, 0, unsupported_exception_handler, NULL
            ),
            rvbl_result_success
        );
        ASSERT_EQ(
            rvbl_interrupt_hart_current_set_trap_handler(
                rvbl_interrupt_hart_privilege_m,
                RVBL_EXCEPTION(
                    rvbl_riscv_hart_privileged_mcause_code_values_Exception_EnvironmentCallFromMMode
                ),
                machine_environment_call_handler
            ),
            rvbl_result_success
        );
        ASSERT_EQ(
            rvbl_interrupt_hart_current_set_trap_handler(
                rvbl_interrupt_hart_privilege_m,
                RVBL_EXCEPTION(
                    rvbl_riscv_hart_privileged_mcause_code_values_Exception_EnvironmentCallFromSMode
                ),
                machine_environment_call_handler
            ),
            rvbl_result_success
        );
        ASSERT_EQ(
            rvbl_interrupt_hart_current_set_trap_handler(
                rvbl_interrupt_hart_privilege_m,
                RVBL_EXCEPTION(
                    rvbl_riscv_hart_privileged_mcause_code_values_Exception_EnvironmentCallFromUMode
                ),
                machine_environment_call_handler
            ),
            rvbl_result_success
        );
        ASSERT_EQ(
            rvbl_interrupt_hart_current_set_trap_handler(
                rvbl_interrupt_hart_privilege_s,
                RVBL_EXCEPTION(
                    rvbl_riscv_hart_privileged_mcause_code_values_Exception_StoreAccessFault
                ),
                supervisor_store_fault_handler
            ),
            rvbl_result_success
        );
        ASSERT_EQ(
            rvbl_interrupt_hart_current_set_trap_handler(
                rvbl_interrupt_hart_privilege_s,
                RVBL_EXCEPTION(
                    rvbl_riscv_hart_privileged_mcause_code_values_Exception_StoreAddressMisaligned
                ),
                supervisor_store_fault_handler
            ),
            rvbl_result_success
        );

        ASSERT_EQ(
            rvbl_interrupt_hart_current_set_delegation(
                RVBL_EXCEPTION(
                    rvbl_riscv_hart_privileged_mcause_code_values_Exception_StoreAccessFault
                ),
                rvbl_true
            ),
            rvbl_result_success
        );
        ASSERT_EQ(
            rvbl_interrupt_hart_current_set_delegation(
                RVBL_EXCEPTION(
                    rvbl_riscv_hart_privileged_mcause_code_values_Exception_StoreAddressMisaligned
                ),
                rvbl_true
            ),
            rvbl_result_success
        );

        ASSERT_EQ(rvbl_interrupt_hart_current_enable_traps(), rvbl_result_success);

        expected_privilege = rvbl_riscv_hart_privileged_32_mstatus_mpp_values_machine;
        rvbl_system_call_1(rvbl_riscv_hart_privileged_32_mstatus_mpp_values_user, 0);

        ASSERT_EQ(supervisor_count, 0);
        rvbl_test_cause_store_fault();
        ASSERT_EQ(supervisor_count, 1);

        expected_privilege = rvbl_riscv_hart_privileged_32_mstatus_mpp_values_user;
        rvbl_system_call_1(rvbl_riscv_hart_privileged_32_mstatus_mpp_values_supervisor, 0);

        expected_privilege = rvbl_riscv_hart_privileged_32_mstatus_mpp_values_supervisor;
        rvbl_system_call_1(rvbl_riscv_hart_privileged_32_mstatus_mpp_values_machine, 0);

        ASSERT_EQ(rvbl_interrupt_hart_current_disable_traps(), rvbl_result_success);
        PASS();
    } else
#endif
    {
        SKIP();
    }

    return 0;
}
