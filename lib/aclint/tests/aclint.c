#include "rvbl/aclint/rvbl_aclint.h"
#include "rvbl/interrupt/rvbl_hart.h"
#include "rvbl/machine/rvbl_configuration.h"
#include "rvbl/machine/rvbl_machine.h"
#include "rvbl/test/rvbl_test.h"
#include "rvbl/type/rvbl_types.h"

static volatile rvbl_uint32_t count = 0;

void machine_timer_handler(volatile rvbl_interrupt_hart_context *context)
{
    context->program_counter += (XLEN / 8);
    ++count;
    rvbl_aclint_set_compare(
        &rvbl_aclint_instance_default,
        0,
        rvbl_aclint_mtime_read(&rvbl_aclint_instance_default) + RVBL_CONFIGURATION_MTIME_FREQUENCY
    );
}

int main(void)
{
    rvbl_hart_hang_if_not(0);
    rvbl_test_initialize();

#if defined(_RVBL_DEVICE_ACLINT_H_)
    ASSERT_EQ(rvbl_interrupt_hart_current_initialize_traps(NULL, NULL), rvbl_result_success);
    ASSERT_EQ(
        rvbl_interrupt_hart_current_set_trap_handler(
            rvbl_interrupt_hart_privilege_m,
            RVBL_INTERRUPT(mcause_code_values_Interrupt_MachineTimer),
            machine_timer_handler
        ),
        rvbl_result_success
    );

    rvbl_aclint_set_compare(&rvbl_aclint_instance_default, 0, 0);
    rvbl_aclint_set_compare(
        &rvbl_aclint_instance_default,
        0,
        rvbl_aclint_mtime_read(&rvbl_aclint_instance_default) + RVBL_CONFIGURATION_MTIME_FREQUENCY
    );
    count = 0;
    rvbl_mie_mtie_set();
    ASSERT_EQ(rvbl_interrupt_hart_current_enable_traps(), rvbl_result_success);
    ASSERT_EQ(count, 0);

    while (count == 0) {
        __asm__ volatile("add zero, zero, zero\n\t");
    }

    ASSERT_EQ(count, 1);
    ASSERT_EQ(rvbl_interrupt_hart_current_disable_traps(), rvbl_result_success);
    rvbl_mie_mtie_clear();

    PASS();
#else
    SKIP();
#endif

    return 0;
}
