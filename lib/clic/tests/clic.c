#include "rvbl/clic/rvbl_clic.h"
#include "rvbl/interrupt/rvbl_hart.h"
#include "rvbl/machine/rvbl_clic.h"
#include "rvbl/machine/rvbl_machine.h"
#include "rvbl/test/rvbl_test.h"
#include "rvbl/type/rvbl_types.h"

static volatile rvbl_uint32_t count = 0;
static const rvbl_clic_interrupt_input_t interrupt = 16;

__attribute__((naked)) void direct_interrupt_handler(void)
{
    // Save sp to mscratch, registers a0 and a1 to interrupt stack
    __asm__ volatile("csrw mscratch, sp\n\t"
                     "la sp, interrupt_stack_top\n\t"
                     "addi sp, sp, -8\n\t"
                     "sw a0, 0(sp)\n\t"
                     "sw a1, 4(sp)\n\t");
    // Increase "count"
    __asm__ volatile("lw a1, %0\n\t"
                     "addi a1, a1, 1\n\t"
                     "sw a1, %0\n\t"
                     : "=m"(count)
                     : "m"(count));
    // Restore registers a0 and a1 from interrupt stack, sp from mscratch
    __asm__ volatile("lw a1, 4(sp)\n\t"
                     "lw a0, 0(sp)\n\t"
                     "csrr sp, mscratch\n\t"
                     "mret\n\t");
}

void unexpected_interrupt_handler(void) { FAIL(); }

int main(void)
{
    rvbl_bool_t enabled;
    const struct rvbl_clic_t *device = &rvbl_clic_instance_default;
    rvbl_bool_t enable = rvbl_false, vectored = rvbl_false;
    rvbl_clic_trigger_t trigger = rvbl_clic_trigger_negative_edge;
    rvbl_clic_mode_t mode = rvbl_clic_mode_user;

    rvbl_hart_hang_if_not(0);
    rvbl_test_initialize();

    ASSERT((rvbl_clic_cfg_nlbits_read(device) + rvbl_clic_cfg_nmbits_read(device)) > 0);
    ASSERT(rvbl_clic_info_num_interrupt_read(device) > 0);

    ASSERT_EQ(rvbl_clic_initialize(unexpected_interrupt_handler), rvbl_result_success);
    ASSERT_EQ(
        rvbl_clic_set_direct_interrupt_handler(device, interrupt, direct_interrupt_handler),
        rvbl_result_success
    );

    ASSERT_EQ(rvbl_clic_get_interrupt_enable(device, interrupt, &enable), rvbl_result_success);
    ASSERT(!enable);
    ASSERT_EQ(rvbl_clic_set_interrupt_enable(device, interrupt, rvbl_true), rvbl_result_success);
    ASSERT_EQ(rvbl_clic_get_interrupt_enable(device, interrupt, &enable), rvbl_result_success);
    ASSERT(enable);

    ASSERT_EQ(
        rvbl_clic_get_interrupt_attributes(device, interrupt, &vectored, &trigger, &mode),
        rvbl_result_success
    );

    ASSERT_EQ(rvbl_interrupt_hart_current_enable_traps(), rvbl_result_success);
    ASSERT_EQ(count, 0);

    ASSERT_EQ(rvbl_clic_set_interrupt_pending(device, interrupt, rvbl_true), rvbl_result_success);

    while (count == 0) {
        __asm__ volatile("add zero, zero, zero\n\t");
    }

    ASSERT_EQ(count, 1);
    ASSERT_EQ(rvbl_interrupt_hart_current_disable_traps(), rvbl_result_success);

    PASS();

    return 0;
}
