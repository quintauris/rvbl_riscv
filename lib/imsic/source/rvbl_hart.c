/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#include "rvbl/imsic/rvbl_hart.h"

#include "rvbl/imsic/rvbl_common.h"
#include "rvbl/machine/rvbl_machine.h"
#include "rvbl/type/rvbl_types.h"

#define MAXIMUM_IDENTITIES 2047

enum imsic_csr_id_t
{
    imsic_csr_id_base = 0x70,
    imsic_csr_id_eidelivery = imsic_csr_id_base + 0,
    imsic_csr_id_eithreshold = imsic_csr_id_base + 2,
    imsic_csr_id_eip_base = imsic_csr_id_base + 0x10,
    imsic_csr_id_eie_base = imsic_csr_id_base + 0x50

};

static rvbl_result_t read_register(
    const rvbl_imsic_hart_mode_t mode, const enum imsic_csr_id_t id, rvbl_uint32_t *const value
)
{
    switch (mode) {
    case rvbl_imsic_mode_machine:
        rvbl_miselect_write(id);
        *value = rvbl_mireg_read();
        break;
    case rvbl_imsic_mode_supervisor:
        rvbl_siselect_write(id);
        *value = rvbl_sireg_read();
        break;
    default:
        return rvbl_result_error_bounds;
    }

    return rvbl_result_success;
}

static rvbl_result_t write_register(
    const rvbl_imsic_hart_mode_t mode, const enum imsic_csr_id_t id, const rvbl_uint32_t value
)
{
    switch (mode) {
    case rvbl_imsic_mode_machine:
        rvbl_miselect_write(id);
        rvbl_mireg_write(value);
        break;
    case rvbl_imsic_mode_supervisor:
        rvbl_siselect_write(id);
        rvbl_sireg_write(value);
        break;
    default:
        return rvbl_result_error_bounds;
    }

    return rvbl_result_success;
}

static rvbl_result_t set_register_bit(
    const rvbl_imsic_hart_mode_t mode,
    const enum imsic_csr_id_t id,
    const rvbl_uint32_t bit,
    const rvbl_bool_t set
)
{
    rvbl_uint32_t value;
    rvbl_result_t result = read_register(mode, id, &value);

    if (result == rvbl_result_success) {
        if (set) {
            value |= (1 << bit);
        } else {
            value &= ~(1 << bit);
        }

        result = write_register(mode, id, value);
    }

    return result;
}

static rvbl_result_t get_register_bit(
    const rvbl_imsic_hart_mode_t mode,
    const enum imsic_csr_id_t id,
    const rvbl_uint32_t bit,
    rvbl_bool_t *const set
)
{
    rvbl_uint32_t value;
    rvbl_result_t result = read_register(mode, id, &value);

    if (result == rvbl_result_success) {
        *set = (value & (1 << bit)) ? rvbl_true : rvbl_false;
    }

    return result;
}

rvbl_result_t rvbl_imsic_hart_set_delivery(
    const rvbl_imsic_hart_mode_t mode, const rvbl_imsic_hart_delivery_t delivery
)
{
    return write_register(mode, imsic_csr_id_eidelivery, delivery);
}

rvbl_result_t rvbl_imsic_hart_get_delivery(
    const rvbl_imsic_hart_mode_t mode, rvbl_imsic_hart_delivery_t *const delivery
)
{
    return read_register(mode, imsic_csr_id_eidelivery, delivery);
}

rvbl_result_t rvbl_imsic_hart_set_threshold(
    const rvbl_imsic_hart_mode_t mode, const rvbl_imsic_interrupt_priority_t priority
)
{
    if (priority <= MAXIMUM_IDENTITIES) {
        return write_register(mode, imsic_csr_id_eithreshold, priority);
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_imsic_hart_get_threshold(
    const rvbl_imsic_hart_mode_t mode, rvbl_imsic_interrupt_priority_t *const priority
)
{
    return read_register(mode, imsic_csr_id_eithreshold, (rvbl_uint32_t *)priority);
}

rvbl_result_t rvbl_imsic_hart_set_interrupt_pending(
    const rvbl_imsic_hart_mode_t mode,
    const rvbl_imsic_interrupt_source_t source,
    const rvbl_bool_t pending
)
{
    if (source <= MAXIMUM_IDENTITIES) {
        const rvbl_uword_t word = source / 32, bit = source % 32;

        return set_register_bit(mode, imsic_csr_id_eip_base + word, bit, pending);
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_imsic_hart_get_interrupt_pending(
    const rvbl_imsic_hart_mode_t mode,
    const rvbl_imsic_interrupt_source_t source,
    rvbl_bool_t *pending
)
{
    if (source <= MAXIMUM_IDENTITIES) {
        const rvbl_uword_t word = source / 32, bit = source % 32;

        return get_register_bit(mode, imsic_csr_id_eip_base + word, bit, pending);
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_imsic_hart_set_interrupt_enable(
    const rvbl_imsic_hart_mode_t mode,
    const rvbl_imsic_interrupt_source_t source,
    const rvbl_bool_t enable
)
{
    if (source <= MAXIMUM_IDENTITIES) {
        const rvbl_uword_t word = source / 32, bit = source % 32;

        return set_register_bit(mode, imsic_csr_id_eie_base + word, bit, enable);
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_imsic_hart_get_interrupt_enable(
    const rvbl_imsic_hart_mode_t mode,
    const rvbl_imsic_interrupt_source_t source,
    rvbl_bool_t *enable
)
{
    if (source <= MAXIMUM_IDENTITIES) {
        const rvbl_uword_t word = source / 32, bit = source % 32;

        return get_register_bit(mode, imsic_csr_id_eie_base + word, bit, enable);
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_imsic_hart_get_top_interrupt(
    const rvbl_imsic_hart_mode_t mode, rvbl_imsic_interrupt_source_t *const source
)
{
    switch (mode) {
    case rvbl_imsic_mode_machine:
        *source = rvbl_mtopei_identity_read();
        break;
    case rvbl_imsic_mode_supervisor:
        *source = rvbl_stopei_identity_read();
        break;
    default:
        return rvbl_result_error_bounds;
    }

    return rvbl_result_success;
}
