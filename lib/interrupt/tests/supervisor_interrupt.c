#include "rvbl/abi/rvbl_abi.h"
#include "rvbl/boot/rvbl_extension.h"
#include "rvbl/interrupt/rvbl_hart.h"
#include "rvbl/test/rvbl_test.h"

typedef mstatus_mpp_values privilege;

static privilege expected_privilege = mstatus_mpp_values_machine;

void interrupt_handler(volatile rvbl_interrupt_hart_context *context)
{
    rvbl_uint32_t *count = (rvbl_uint32_t *)context->extra;

    rvbl_test_supervisor_interrupt_control.cleanup_interrupt();
    ++(*count);
}

typedef enum call_index
{
    call_index_switch_mode = 0,
} call_index;

void environment_call_handler(volatile rvbl_interrupt_hart_context *context)
{
    const call_index index = (call_index)context->arguments[0];
    ASSERT_EQ(rvbl_mstatus_mpp_read(), expected_privilege);

    switch (index) {
    case call_index_switch_mode:
        rvbl_mstatus_mpp_write((privilege)context->arguments[1]);
        context->program_counter += (XLEN / 8);
        break;
    }
}

void default_handler(volatile rvbl_interrupt_hart_context *context)
{
    rvbl_test_log("context mcause 0x%X", context->cause);
    FAIL();
}

#if 0
rvbl_bool_t is_stimecmp_supported(void)
{
#if !defined(_RVBL_DEVICE_ACLINT_H_)
    return rvbl_false;
#elif defined(_RVBL_DEVICE_QUINTAURIS_DISCOVERY_H_)
    ASSERT_EQ(
        rvbl_quintauris_discovery_vendor_id_read(&rvbl_quintauris_discovery_instance_default),
        quintauris_discovery_vendor_id_values_quintauris
    );
    ASSERT_EQ(
        rvbl_quintauris_discovery_implementation_id_platform_read(
            &rvbl_quintauris_discovery_instance_default
        ),
        quintauris_discovery_implementation_id_platform_values_rt_europa
    );

    switch (rvbl_quintauris_discovery_implementation_id_variant_read(
        &rvbl_quintauris_discovery_instance_default
    )) {
    // RT-Europa Na implementation is based off Andes D45-SE processor, which has no stimecmp CSR
    case quintauris_discovery_implementation_id_variant_values_na_fpga:
    case quintauris_discovery_implementation_id_variant_values_na_qemu:
        return rvbl_false;
    default:
        return rvbl_true;
    }
#else
#define HPMICRO_VENDOR_ID 0x0000031E
#define HPMICRO_HPM6280_CPU_ID 0x80000045

    if ((rvbl_mvendorid_read() == HPMICRO_VENDOR_ID) &&
        (rvbl_marchid_read() == HPMICRO_HPM6280_CPU_ID)) {
        return rvbl_false;
    } else {
        return rvbl_true;
    }
#endif
}
#endif

int main(void)
{
    volatile rvbl_uint32_t count = 0;

    rvbl_hart_hang_if_not(0);
    rvbl_test_initialize();
    ASSERT_EQ(rvbl_alloc_initialize(&rvbl_test_allocator), rvbl_result_success);

    if (rvbl_extension_present(misa_extensions_values_SupervisorMode) &&
        rvbl_test_supervisor_interrupt_control.can_trigger_interrupt) {
        ASSERT_EQ(
            rvbl_interrupt_hart_current_initialize_traps(
                &rvbl_test_allocator, 0, default_handler, (rvbl_pointer_t)&count
            ),
            rvbl_result_success
        );

        ASSERT_EQ(
            rvbl_interrupt_hart_current_set_trap_handler(
                rvbl_interrupt_hart_privilege_m,
                RVBL_EXCEPTION(mcause_code_values_Exception_EnvironmentCallFromMMode),
                environment_call_handler
            ),
            rvbl_result_success
        );
        ASSERT_EQ(
            rvbl_interrupt_hart_current_set_trap_handler(
                rvbl_interrupt_hart_privilege_m,
                RVBL_EXCEPTION(mcause_code_values_Exception_EnvironmentCallFromSMode),
                environment_call_handler
            ),
            rvbl_result_success
        );
        ASSERT_EQ(
            rvbl_interrupt_hart_current_set_trap_handler(
                rvbl_interrupt_hart_privilege_m,
                RVBL_EXCEPTION(mcause_code_values_Exception_EnvironmentCallFromUMode),
                environment_call_handler
            ),
            rvbl_result_success
        );
        ASSERT_EQ(
            rvbl_interrupt_hart_current_set_trap_handler(
                rvbl_interrupt_hart_privilege_s,
                RVBL_INTERRUPT(rvbl_test_supervisor_interrupt_control.interrupt_code),
                interrupt_handler
            ),
            rvbl_result_success
        );

        ASSERT_EQ(
            rvbl_interrupt_hart_current_set_delegation(
                RVBL_INTERRUPT(rvbl_test_supervisor_interrupt_control.interrupt_code), rvbl_true
            ),
            rvbl_result_success
        );
        rvbl_test_supervisor_interrupt_control.prepare_interrupt();
        expected_privilege = mstatus_mpp_values_machine;

        ASSERT_EQ(rvbl_interrupt_hart_current_enable_traps(), rvbl_result_success);
        rvbl_system_call_1(call_index_switch_mode, mstatus_mpp_values_supervisor);
        expected_privilege = mstatus_mpp_values_supervisor;

        ASSERT_EQ(count, 0);

        rvbl_test_supervisor_interrupt_control.trigger_interrupt();

        while (count == 0) {
            __asm__ volatile("wfi\n\t");
        }

        ASSERT_EQ(count, 1);

        expected_privilege = mstatus_mpp_values_supervisor;
        rvbl_system_call_1(call_index_switch_mode, mstatus_mpp_values_machine);
        ASSERT_EQ(rvbl_interrupt_hart_current_disable_traps(), rvbl_result_success);

        PASS();
    } else {
        SKIP();
    }

    return 0;
}
