/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#include "rvbl/abi/rvbl_abi.h"
#include "rvbl/alloc/rvbl_alloc_bump.h"
#include "rvbl/clic/rvbl_clic.h"
#include "rvbl/interrupt/rvbl_hart.h"
#include "rvbl/machine/rvbl_machine.h"
#include "rvbl/test/rvbl_test.h"
#include "rvbl/type/rvbl_types.h"

static const rvbl_clic_interrupt_input_t interrupt_number = 16;
static volatile rvbl_uword_t counter = 0;

static __attribute__((aligned(4))) rvbl_uint8_t heap[4096];
static rvbl_alloc_area area = {heap, sizeof(heap)};
static rvbl_alloc_allocator allocator = {&rvbl_alloc_bump_allocator, 1, &area};

static void unexpected_interrupt_handler(volatile rvbl_clic_hart_context *context)
{
    (void)context;
    FAIL();
}

static void interrupt_handler(volatile rvbl_clic_hart_context *context)
{
    (void)context;
    counter = 1;
}

static __attribute__((naked)) void direct_interrupt_handler(void)
{
    __asm__ volatile("addi gp, zero, 1");
    __asm__ volatile("mret");
}

int main(void)
{
    const struct rvbl_clic_t *device = &rvbl_clic_instance_default;
    rvbl_bool_t enable = rvbl_false, vectored = rvbl_false;
    rvbl_clic_trigger_t trigger = rvbl_clic_trigger_negative_edge;
    rvbl_clic_mode_t mode = rvbl_clic_mode_user;

    rvbl_hart_hang_if_not(0);
    rvbl_test_initialize();

    counter = 0;

    rvbl_test_log("start");

    ASSERT(
        (RVBL_REGISTER_FIELD_READ(clic, registers, device, cfg, nlbits) +
         RVBL_REGISTER_FIELD_READ(clic, registers, device, cfg, nmbits)) > 0
    );
    ASSERT(RVBL_REGISTER_FIELD_READ(clic, registers, device, info, num_interrupt) > 0);

    ASSERT_EQ(rvbl_clic_allocate(&allocator, 32), rvbl_result_success);
    ASSERT_EQ(rvbl_clic_initialize(&allocator, unexpected_interrupt_handler), rvbl_result_success);
    ASSERT_EQ(
        rvbl_clic_set_interrupt_handler(device, interrupt_number, interrupt_handler),
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

    rvbl_test_log("1");
    rvbl_test_log("1a");

    ASSERT_EQ(
        rvbl_clic_get_interrupt_attributes(device, interrupt_number, &vectored, &trigger, &mode),
        rvbl_result_success
    );

    rvbl_test_log("2");

    ASSERT_EQ(rvbl_interrupt_hart_current_enable_traps(), rvbl_result_success);
    ASSERT_EQ(counter, 0);

    rvbl_test_log("3");

    ASSERT_EQ(
        rvbl_clic_set_interrupt_pending(device, interrupt_number, rvbl_true), rvbl_result_success
    );

    rvbl_test_log("a");

    while (counter == 0) {
        __asm__ volatile("wfi\n\t");
    }

    ASSERT_EQ(counter, 1);

    ASSERT_EQ(
        rvbl_clic_set_interrupt_handler_direct(device, interrupt_number, direct_interrupt_handler),
        rvbl_result_success
    );

    rvbl_global_pointer_register_write(0);
    ASSERT_EQ(
        rvbl_clic_set_interrupt_pending(device, interrupt_number, rvbl_true), rvbl_result_success
    );

    rvbl_test_log("b");

    while (counter == 0) {
        __asm__ volatile("wfi\n\t");
    }

    rvbl_test_log("c");

    ASSERT_EQ(rvbl_global_pointer_register_read(), (void *)1);

    ASSERT_EQ(rvbl_interrupt_hart_current_disable_traps(), rvbl_result_success);

    PASS();

    return 0;
}
