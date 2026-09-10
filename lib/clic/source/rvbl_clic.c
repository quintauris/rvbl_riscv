/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#include "rvbl/clic/rvbl_clic.h"
#include "rvbl/hardware/rvbl_hardware.h"
#include "rvbl/machine/rvbl_configuration.h"
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
        const rvbl_uint32_t clicintbits =
            RVBL_REGISTER_FIELD_READ(clic, registers, instance, info, clicintctlbits);
        const rvbl_uint32_t level_bits =
            RVBL_REGISTER_FIELD_READ(clic, registers, instance, cfg, nlbits);
        ;
        const rvbl_uint32_t mask = (level << (8 - level_bits)) | (priority << clicintbits);

        // RVBL_REGISTER_FIELD_WRITE(clic, registers, instance, intctl, input, mask);
        RVBL_INDEXED_REGISTER_FIELD_WRITE(clic, registers, instance, _int, input, ctl, mask);

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
        const rvbl_uint32_t clicintbits =
            RVBL_REGISTER_FIELD_READ(clic, registers, instance, info, clicintctlbits);
        const rvbl_uint32_t level_bits =
            RVBL_REGISTER_FIELD_READ(clic, registers, instance, cfg, nlbits);
        ;
        const rvbl_uint32_t priority_bits = clicintbits - level_bits;
        rvbl_uint32_t value =
            RVBL_INDEXED_REGISTER_FIELD_READ(clic, registers, instance, _int, input, ctl);

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
        RVBL_INDEXED_REGISTER_FIELD_WRITE(clic, registers, instance, _int, input, ip, pending);

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
        *pending = RVBL_INDEXED_REGISTER_FIELD_READ(clic, registers, instance, _int, input, ip)
                       ? rvbl_true
                       : rvbl_false;

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
        RVBL_INDEXED_REGISTER_FIELD_WRITE(clic, registers, instance, _int, input, ie, enable);

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
        *enabled = RVBL_INDEXED_REGISTER_FIELD_READ(clic, registers, instance, _int, input, ie)
                       ? rvbl_true
                       : rvbl_false;

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
        RVBL_INDEXED_REGISTER_FIELD_WRITE(clic, registers, instance, _int, input, attr_shv, shv);
        RVBL_INDEXED_REGISTER_FIELD_WRITE(
            clic,
            registers,
            instance,
            _int,
            input,
            attr_trig,
            (enum rvbl_clic_registers__int_attr_trig_t)trigger
        );
        RVBL_INDEXED_REGISTER_FIELD_WRITE(
            clic,
            registers,
            instance,
            _int,
            input,
            attr_mode,
            (enum rvbl_clic_registers__int_attr_mode_t)mode
        );

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

extern void rvbl_test_log(const char *format, ...);

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

        *shv = RVBL_INDEXED_REGISTER_FIELD_READ(clic, registers, instance, _int, input, attr_shv)
                   ? rvbl_true
                   : rvbl_false;
        *trigger = (rvbl_clic_trigger_t)RVBL_INDEXED_REGISTER_FIELD_READ(
            clic, registers, instance, _int, input, attr_trig
        );
        *mode = (rvbl_clic_mode_t)RVBL_INDEXED_REGISTER_FIELD_READ(
            clic, registers, instance, _int, input, attr_mode
        );

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
    const rvbl_uint32_t hart_id =
        RVBL_REGISTER_READ(riscv_hart, privileged, &rvbl_riscv_hart_instance_0, mhartid);
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

    RVBL_REGISTER_WRITE(
        clic,
        control_status_registers,
        &rvbl_clic_instance_default,
        mtvt,
        (rvbl_uword_t)machine_trap_vector_table
    );

    return rvbl_result_success;
}

rvbl_clic_trap_handler *rvbl_clic_internal_select_trap_handler(
    const rvbl_clic_mode_t privilege, rvbl_uword_t interrupt_code, rvbl_clic_hart_context *context
)
{
    const rvbl_uint32_t hart_id =
        RVBL_REGISTER_READ(riscv_hart, privileged, &rvbl_riscv_hart_instance_0, mhartid);

    (void)privilege;
    (void)context;
    return &interrupt_handler_tables[hart_id][interrupt_code & 0xFFFF];
}

rvbl_result_t rvbl_clic_set_interrupt_handler(
    const struct rvbl_clic_t *instance,
    const rvbl_clic_interrupt_input_t input,
    const rvbl_clic_trap_handler handler
)
{
    const rvbl_uint32_t hart_id =
        RVBL_REGISTER_READ(riscv_hart, privileged, &rvbl_riscv_hart_instance_0, mhartid);

    if ((input >= RVBL_CLIC_MIN_INTERRUPT_INPUT_ID) &&
        (input <= (instance->interrupt_inputs_parameter - 1)) && (input <= total_traps)) {
        interrupt_handler_tables[hart_id][RISCV_RESERVED_EXCEPTIONS + input] = handler;

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
