#include "rvbl/interrupt/rvbl_hart.h"
#include "rvbl/machine/rvbl_machine.h"
#include "rvbl/test/rvbl_test.h"

void supervisor_software_interrupt_handler(volatile rvbl_interrupt_hart_context *context)
{
    rvbl_uint32_t *count = (rvbl_uint32_t *)context->extra;

    rvbl_mip_ssip_clear();
    ++(*count);
}

rvbl_bool_t is_sswi_supported(void)
{
    const rvbl_uint32_t PRORIETARY_SINGLECORE_RHX = 0x80000402;
    const rvbl_uint32_t ARCV_RHX_1_0 = 0x00020100;
    const rvbl_uint32_t NORDIC_VPR_IMP_ID = 0xE010502;
    const rvbl_uint32_t NORDIC_VPR_ARCH_ID = 0x8000246E;
    const rvbl_uint32_t arch_id = rvbl_marchid_read();
    const rvbl_uint32_t imp_id = rvbl_mimpid_read();

    if ((arch_id == PRORIETARY_SINGLECORE_RHX) && (imp_id == ARCV_RHX_1_0)) {
        rvbl_uint32_t arcv_clint_build = 0;

        __asm__ volatile("csrr %0, 0x0FD4" : "=r"(arcv_clint_build) : :);

        // SSWI [3] Indicates supervisor level interrupt device.
        return (arcv_clint_build & (1UL << 3)) ? rvbl_true : rvbl_false;
    } else if ((arch_id == NORDIC_VPR_ARCH_ID) && (imp_id == NORDIC_VPR_IMP_ID)) {
        return rvbl_false;
    } else {
        return rvbl_true;
    }
}

int main(void)
{
    rvbl_uint32_t count = 0;

    rvbl_hart_hang_if_not(0);
    rvbl_test_initialize();
    ASSERT_EQ(rvbl_alloc_initialize(&rvbl_test_allocator), rvbl_result_success);

    if (is_sswi_supported()) {
        ASSERT_EQ(
            rvbl_interrupt_hart_current_initialize_traps(&rvbl_test_allocator, 0, NULL, &count),
            rvbl_result_success
        );
        ASSERT_EQ(
            rvbl_interrupt_hart_current_set_trap_handler(
                rvbl_interrupt_hart_privilege_m,
                RVBL_INTERRUPT(mcause_code_values_Interrupt_SupervisorSoftware),
                supervisor_software_interrupt_handler
            ),
            rvbl_result_success
        );

        count = 0;
        rvbl_mie_ssie_set();
        ASSERT_EQ(rvbl_interrupt_hart_current_enable_traps(), rvbl_result_success);

        ASSERT_EQ(count, 0);
        rvbl_mip_ssip_set();

        while (count == 0) {
            __asm__ volatile("add zero, zero, zero\n\t");
        }

        ASSERT_EQ(count, 1);

        ASSERT_EQ(rvbl_interrupt_hart_current_disable_traps(), rvbl_result_success);
        rvbl_mie_ssie_clear();

        PASS();
    } else {
        SKIP();
    }

    return 0;
}
