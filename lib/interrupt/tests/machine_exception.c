#include "rvbl/core/rvbl_abi.h"
#include "rvbl/core/rvbl_types.h"
#include "rvbl/interrupt/rvbl_hart.h"
#include "rvbl/machine/rvbl_machine.h"
#include "rvbl/test/rvbl_test.h"

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
    ASSERT_EQ(rvbl_interrupt_hart_current_initialize_traps(NULL, &count), rvbl_result_success);
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
