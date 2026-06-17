/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#include "rvbl/clic/rvbl_clic.h"
#include "rvbl/machine/rvbl_configuration.h"
#include "rvbl/machine/rvbl_flpr.h"
#include "rvbl/type/rvbl_types.h"

rvbl_result_t rvbl_clic_set_interrupt_priority(
    const struct rvbl_clic_t *instance,
    const rvbl_clic_interrupt_input_t input,
    const rvbl_clic_interrupt_level_t level,
    const rvbl_clic_interrupt_priority_t priority
)
{
    if ((input >= RVBL_CLIC_MIN_INTERRUPT_INPUT_ID) &&
        (input <= (instance->interrupt_inputs_parameter - 1)) &&
        (level >= RVBL_CLIC_MIN_INTERRUPT_LEVEL) && (level <= RVBL_CLIC_MAX_INTERRUPT_LEVEL) &&
        (priority >= RVBL_CLIC_MIN_INTERRUPT_PRIORITY) &&
        (priority <= RVBL_CLIC_MAX_INTERRUPT_PRIORITY)) {
        const rvbl_uint32_t clicintbits = rvbl_clic_info_clicintbits_read(instance);
        const rvbl_uint32_t level_bits = rvbl_clic_cfg_nlbits_read(instance);
        const rvbl_uint32_t mask = (level << (8 - level_bits)) | (priority << clicintbits);

        rvbl_clic_intctl_write(instance, input, mask);

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_clic_get_interrupt_priority(
    const struct rvbl_clic_t *instance,
    const rvbl_clic_interrupt_input_t input,
    rvbl_clic_interrupt_level_t *level,
    rvbl_clic_interrupt_priority_t *priority
)
{
    if ((input >= RVBL_CLIC_MIN_INTERRUPT_INPUT_ID) &&
        (input <= (instance->interrupt_inputs_parameter - 1))) {
        const rvbl_uint32_t clicintbits = rvbl_clic_info_clicintbits_read(instance);
        const rvbl_uint32_t level_bits = rvbl_clic_cfg_nlbits_read(instance);
        const rvbl_uint32_t priority_bits = clicintbits - level_bits;
        rvbl_uint32_t value = rvbl_clic_intctl_read(instance, input);

        if (level_bits > 0) {
            value >>= 8 - clicintbits;

            if (priority_bits > 0) {
                *level = value >> priority_bits;
                *priority = value & ((1 << priority_bits) - 1);
            } else {
                *level = value;
            }
        }

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_clic_set_interrupt_pending(
    const struct rvbl_clic_t *instance,
    const rvbl_clic_interrupt_input_t input,
    const rvbl_bool_t pending
)
{
    if ((input >= RVBL_CLIC_MIN_INTERRUPT_INPUT_ID) &&
        (input <= (instance->interrupt_inputs_parameter - 1))) {
        rvbl_clic_intip_write(instance, input, pending);

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_clic_get_interrupt_pending(
    const struct rvbl_clic_t *instance,
    const rvbl_clic_interrupt_input_t input,
    rvbl_bool_t *pending
)
{
    if ((input >= RVBL_CLIC_MIN_INTERRUPT_INPUT_ID) &&
        (input <= (instance->interrupt_inputs_parameter - 1))) {
        *pending = rvbl_clic_intip_read(instance, input) ? rvbl_true : rvbl_false;

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_clic_set_interrupt_enable(
    const struct rvbl_clic_t *instance, const rvbl_clic_interrupt_input_t input, rvbl_bool_t enable
)
{
    if ((input >= RVBL_CLIC_MIN_INTERRUPT_INPUT_ID) &&
        (input <= (instance->interrupt_inputs_parameter - 1))) {
        rvbl_clic_intie_write(instance, input, enable);

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_clic_get_interrupt_enable(
    const struct rvbl_clic_t *instance,
    const rvbl_clic_interrupt_input_t input,
    rvbl_bool_t *enabled
)
{
    if ((input >= RVBL_CLIC_MIN_INTERRUPT_INPUT_ID) &&
        (input <= (instance->interrupt_inputs_parameter - 1))) {
        *enabled = rvbl_clic_intie_read(instance, input) ? rvbl_true : rvbl_false;

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_clic_set_interrupt_attributes(
    const struct rvbl_clic_t *instance,
    const rvbl_clic_interrupt_input_t input,
    const rvbl_bool_t shv,
    const rvbl_clic_trigger_t trigger,
    const rvbl_clic_mode_t mode
)
{
    if ((input >= RVBL_CLIC_MIN_INTERRUPT_INPUT_ID) &&
        (input <= (instance->interrupt_inputs_parameter - 1))) {
        rvbl_clic_intattr_shv_write(instance, input, shv);
        rvbl_clic_intattr_trig_write(instance, input, (clic_intattr_trig_values)trigger);
        rvbl_clic_intattr_mode_write(instance, input, (clic_intattr_mode_values)mode);

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_clic_get_interrupt_attributes(
    const struct rvbl_clic_t *instance,
    const rvbl_clic_interrupt_input_t input,
    rvbl_bool_t *shv,
    rvbl_clic_trigger_t *trigger,
    rvbl_clic_mode_t *mode
)
{
    if ((input >= RVBL_CLIC_MIN_INTERRUPT_INPUT_ID) &&
        (input <= (instance->interrupt_inputs_parameter - 1))) {

        *shv = rvbl_clic_intattr_shv_read(instance, input) ? rvbl_true : rvbl_false;
        *trigger = (rvbl_clic_trigger_t)rvbl_clic_intattr_trig_read(instance, input);
        *mode = (rvbl_clic_mode_t)rvbl_clic_intattr_mode_read(instance, input);

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

#define RISCV_RESERVED_EXCEPTIONS 16
#define RISCV_RESERVED_INTERRUPTS 16

static rvbl_clic_direct_interrupt_handler *machine_trap_vector_table = NULL;
static rvbl_clic_trap_handler **interrupt_handler_tables = NULL;
static rvbl_uword_t total_traps = RISCV_RESERVED_EXCEPTIONS + RISCV_RESERVED_INTERRUPTS;

extern void clic_machine_trap_wrapper(void);
static void nop(volatile rvbl_clic_hart_context *c) { (void)c; }

rvbl_result_t rvbl_clic_allocate(rvbl_alloc_allocator *allocator, rvbl_uword_t platform_interrupts)
{
    rvbl_uword_t i;

    total_traps = RISCV_RESERVED_EXCEPTIONS + RISCV_RESERVED_INTERRUPTS + platform_interrupts;
    machine_trap_vector_table =
        rvbl_alloc_aligned(allocator, total_traps * sizeof(rvbl_pointer_t), 64);

    if (machine_trap_vector_table == NULL) {
        return rvbl_result_error_out_of_memory;
    }

    for (i = 0; i < total_traps; ++i) {
        machine_trap_vector_table[i] = clic_machine_trap_wrapper;
    }

    interrupt_handler_tables =
        rvbl_alloc(allocator, sizeof(rvbl_pointer_t) * RVBL_CONFIGURATION_HART_COUNT);

    if (interrupt_handler_tables == NULL) {
        return rvbl_result_error_out_of_memory;
    }

    return rvbl_result_success;
}

rvbl_result_t rvbl_clic_initialize(rvbl_alloc_allocator *allocator, rvbl_clic_trap_handler handler)
{
    const rvbl_uint32_t hart_id = rvbl_mhartid_read();
    rvbl_uint32_t i;

    interrupt_handler_tables[hart_id] = rvbl_alloc(allocator, total_traps * sizeof(rvbl_pointer_t));

    if (interrupt_handler_tables[hart_id] == NULL) {
        return rvbl_result_error_out_of_memory;
    }

    if (handler == NULL) {
        handler = nop;
    }

    for (i = 0; i < total_traps; ++i) {
        interrupt_handler_tables[hart_id][i] = handler;
    }

    rvbl_mtvt_write((rvbl_uint32_t)machine_trap_vector_table);

    return rvbl_result_success;
}

rvbl_clic_trap_handler *rvbl_clic_internal_select_trap_handler(
    const rvbl_clic_mode_t privilege, rvbl_uword_t interrupt_code, rvbl_clic_hart_context *context
)
{
    (void)privilege;
    (void)context;
    return &interrupt_handler_tables[rvbl_mhartid_read()][interrupt_code & 0xFFFF];
}

rvbl_result_t rvbl_clic_set_interrupt_handler(
    const struct rvbl_clic_t *instance,
    const rvbl_clic_interrupt_input_t input,
    const rvbl_clic_trap_handler handler
)
{
    if ((input >= RVBL_CLIC_MIN_INTERRUPT_INPUT_ID) &&
        (input <= (instance->interrupt_inputs_parameter - 1)) && (input <= total_traps)) {
        interrupt_handler_tables[rvbl_mhartid_read()][RISCV_RESERVED_EXCEPTIONS + input] = handler;

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_clic_set_interrupt_handler_direct(
    const struct rvbl_clic_t *instance,
    const rvbl_clic_interrupt_input_t input,
    const rvbl_clic_direct_interrupt_handler handler
)
{
    if ((input >= RVBL_CLIC_MIN_INTERRUPT_INPUT_ID) &&
        (input <= (instance->interrupt_inputs_parameter - 1)) && (input <= total_traps)) {
        machine_trap_vector_table[RISCV_RESERVED_EXCEPTIONS + input] = handler;

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}
