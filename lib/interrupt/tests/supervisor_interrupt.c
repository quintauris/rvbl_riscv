#include "rvbl/abi/rvbl_abi.h"
#include "rvbl/boot/rvbl_extension.h"
#include "rvbl/core/rvbl_types.h"
#include "rvbl/interrupt/rvbl_hart.h"
#include "rvbl/machine/rvbl_configuration.h"
#include "rvbl/machine/rvbl_machine.h"
#include "rvbl/test/rvbl_test.h"

typedef mstatus_mpp_values privilege;

static volatile rvbl_uint32_t count = 0;
static privilege expected_privilege = mstatus_mpp_values_machine;

void init_stimecmp()
{
    const rvbl_uint64_t time =
        rvbl_aclint_mtime_read(&rvbl_aclint_instance_default) + RVBL_CONFIGURATION_MTIME_FREQUENCY;

    rvbl_menvcfgh_stce_set();
    rvbl_sie_stie_clear();
    rvbl_stimecmp_write(time & 0x00000000FFFFFFFF);
    rvbl_stimecmph_write((time & 0xFFFFFFFF00000000) >> 32);
    rvbl_sie_stie_set();
}

void supervisor_timer_handler(volatile rvbl_interrupt_hart_context *context)
{
    context->program_counter += (XLEN / 8);
    ++count;
    rvbl_sie_stie_clear();    // no more interrupts
    rvbl_sstatus_spp_clear(); // back to use mode (0)
}

void environment_call_handler(volatile rvbl_interrupt_hart_context *context)
{
    const privilege parameter = (privilege)context->arguments[0];
    ASSERT_EQ(rvbl_mstatus_mpp_read(), expected_privilege);

    context->program_counter += (XLEN / 8);
    rvbl_mstatus_mpp_write(parameter);
}

void default_handler(volatile rvbl_interrupt_hart_context *context)
{
    context->program_counter += (XLEN / 8);
    FAIL();
}

rvbl_bool_t is_stimecmp_supported(void)
{
#if defined(_RVBL_DEVICE_QUINTAURIS_DISCOVERY_H_)
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

int main(void)
{
    rvbl_hart_hang_if_not(0);
    rvbl_test_initialize();

    if (rvbl_extension_present(misa_extensions_values_SupervisorMode) && is_stimecmp_supported()) {
        ASSERT_EQ(
            rvbl_interrupt_hart_current_initialize_traps(default_handler, NULL), rvbl_result_success
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
                RVBL_INTERRUPT(scause_code_values_Interrupt_SupervisorTimer),
                supervisor_timer_handler
            ),
            rvbl_result_success
        );

        ASSERT_EQ(
            rvbl_interrupt_hart_current_set_delegation(
                RVBL_INTERRUPT(scause_code_values_Interrupt_SupervisorTimer), rvbl_true
            ),
            rvbl_result_success
        );
        init_stimecmp();
        ASSERT_EQ(rvbl_interrupt_hart_current_enable_traps(), rvbl_result_success);

        expected_privilege = mstatus_mpp_values_machine;
        rvbl_system_call_1(mstatus_mpp_values_user, 0);

        ASSERT_EQ(count, 0);

        while (count == 0) {
            __asm__ volatile("add zero, zero, zero\n\t");
        }

        ASSERT_EQ(count, 1);

        expected_privilege = mstatus_mpp_values_user;
        rvbl_system_call_1(mstatus_mpp_values_machine, 0);

        ASSERT_EQ(rvbl_interrupt_hart_current_disable_traps(), rvbl_result_success);
        PASS();
    } else {
        SKIP();
    }

    return 0;
}
