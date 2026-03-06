#include "rvbl/abi/rvbl_abi.h"
#include "rvbl/clic/rvbl_clic.h"
#include "rvbl/interrupt/rvbl_hart.h"
#include "rvbl/machine/rvbl_clic.h"
#include "rvbl/machine/rvbl_machine.h"
#include "rvbl/test/rvbl_test.h"
#include "rvbl/type/rvbl_types.h"

static const rvbl_clic_interrupt_input_t interrupt_number = 16;

__attribute__((naked)) void direct_interrupt_handler(void)
{
    __asm__ volatile("addi tp, tp, 1\n\t"
                     "mret\n\t");
}

void unexpected_interrupt_handler(void) { FAIL(); }

int main(void)
{
    const struct rvbl_clic_t *device = &rvbl_clic_instance_default;
    rvbl_bool_t enable = rvbl_false, vectored = rvbl_false;
    rvbl_clic_trigger_t trigger = rvbl_clic_trigger_negative_edge;
    rvbl_clic_mode_t mode = rvbl_clic_mode_user;

    rvbl_hart_hang_if_not(0);
    rvbl_test_initialize();

    // Used to check interrupt execution count
    rvbl_thread_pointer_register_write(0);

    ASSERT((rvbl_clic_cfg_nlbits_read(device) + rvbl_clic_cfg_nmbits_read(device)) > 0);
    ASSERT(rvbl_clic_info_num_interrupt_read(device) > 0);

    ASSERT_EQ(rvbl_clic_initialize(unexpected_interrupt_handler), rvbl_result_success);
    ASSERT_EQ(
        rvbl_clic_set_direct_interrupt_handler(device, interrupt_number, direct_interrupt_handler),
        rvbl_result_success
    );

    ASSERT_EQ(
        rvbl_clic_get_interrupt_enable(device, interrupt_number, &enable), rvbl_result_success
    );
    ASSERT(!enable);
    ASSERT_EQ(
        rvbl_clic_set_interrupt_enable(device, interrupt_number, rvbl_true), rvbl_result_success
    );
    ASSERT_EQ(
        rvbl_clic_get_interrupt_enable(device, interrupt_number, &enable), rvbl_result_success
    );
    ASSERT(enable);

    ASSERT_EQ(
        rvbl_clic_get_interrupt_attributes(device, interrupt_number, &vectored, &trigger, &mode),
        rvbl_result_success
    );

    ASSERT_EQ(rvbl_interrupt_hart_current_enable_traps(), rvbl_result_success);
    ASSERT_EQ(rvbl_thread_pointer_register_read(), 0);

    ASSERT_EQ(
        rvbl_clic_set_interrupt_pending(device, interrupt_number, rvbl_true), rvbl_result_success
    );

    while (rvbl_thread_pointer_register_read() == 0) {
        __asm__ volatile("add zero, zero, zero\n\t");
    }

    ASSERT_EQ(rvbl_thread_pointer_register_read(), (void *)1);
    ASSERT_EQ(rvbl_interrupt_hart_current_disable_traps(), rvbl_result_success);

    PASS();

    return 0;
}
