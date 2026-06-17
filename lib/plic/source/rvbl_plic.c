/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#include "rvbl/plic/rvbl_plic.h"

typedef rvbl_plic_interrupt_priority_t priority_t;

rvbl_result_t rvbl_plic_set_interrupt_priority(
    const struct rvbl_plic_t *const instance,
    const rvbl_plic_interrupt_source_t source,
    const rvbl_plic_interrupt_priority_t new_priority
)
{
    if ((source >= RVBL_PLIC_MIN_INTERRUPT_SOURCE_ID) &&
        (source <= RVBL_PLIC_MAX_INTERRUPT_SOURCE_ID) &&
        (new_priority <= instance->maximum_priority_parameter)) {
        rvbl_plic_priority_write(instance, source, new_priority);

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_plic_get_interrupt_priority(
    const struct rvbl_plic_t *const instance,
    const rvbl_plic_interrupt_source_t source,
    rvbl_plic_interrupt_priority_t *const target_priority
)
{
    if ((source >= RVBL_PLIC_MIN_INTERRUPT_SOURCE_ID) &&
        (source <= RVBL_PLIC_MAX_INTERRUPT_SOURCE_ID) && (target_priority != NULL)) {
        *target_priority = rvbl_plic_priority_read(instance, source);

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_plic_set_interrupt_pending(
    const struct rvbl_plic_t *const instance, const rvbl_plic_interrupt_source_t source
)
{
    if ((source >= RVBL_PLIC_MIN_INTERRUPT_SOURCE_ID) &&
        (source <= RVBL_PLIC_MAX_INTERRUPT_SOURCE_ID)) {
        const rvbl_uword_t word = source / 32, bit = source % 32, mask = (1 << bit);

        rvbl_plic_pending_write(instance, word, rvbl_plic_pending_read(instance, word) | mask);

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_plic_get_interrupt_pending(
    const struct rvbl_plic_t *const instance,
    const rvbl_plic_interrupt_source_t source,
    rvbl_bool_t *const target_pending
)
{
    if ((source >= RVBL_PLIC_MIN_INTERRUPT_SOURCE_ID) &&
        (source <= RVBL_PLIC_MAX_INTERRUPT_SOURCE_ID) && (target_pending != NULL)) {
        const rvbl_uword_t word = source / 32, bit = source % 32, mask = (1 << bit);

        *target_pending = (rvbl_plic_pending_read(instance, word) & mask) ? rvbl_true : rvbl_false;

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_plic_set_interrupt_enable(
    const struct rvbl_plic_t *const instance,
    const rvbl_plic_interrupt_source_t source,
    const rvbl_plic_interrupt_context_t context,
    const rvbl_bool_t new_enable
)
{
    if ((source >= RVBL_PLIC_MIN_INTERRUPT_SOURCE_ID) &&
        (source <= RVBL_PLIC_MAX_INTERRUPT_SOURCE_ID) && (context <= RVBL_PLIC_MAX_CONTEXT_ID) &&
        (context <= instance->maximum_context_parameter)) {
        const rvbl_uword_t word = source / 32, bit = source % 32;
        const rvbl_uint32_t mask = ((rvbl_uint32_t)1) << bit;
        const rvbl_uint32_t value = rvbl_plic_enable_read(instance, word);

        if (new_enable) {
            rvbl_plic_enable_write(instance, word, value | mask);
        } else {
            rvbl_plic_enable_write(instance, word, value & ~mask);
        }

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_plic_get_interrupt_enable(
    const struct rvbl_plic_t *const instance,
    const rvbl_plic_interrupt_source_t source,
    const rvbl_plic_interrupt_context_t context,
    rvbl_bool_t *const target_enable
)
{
    if ((source >= RVBL_PLIC_MIN_INTERRUPT_SOURCE_ID) &&
        (source <= RVBL_PLIC_MAX_INTERRUPT_SOURCE_ID) && (context <= RVBL_PLIC_MAX_CONTEXT_ID) &&
        (context <= instance->maximum_context_parameter) && (target_enable != NULL)) {
        const rvbl_uword_t word = source / 32, bit = source % 32;
        const rvbl_uword_t target = context * 32 + word;
        const rvbl_uint32_t mask = ((rvbl_uint32_t)1) << bit;

        *target_enable = (rvbl_plic_enable_read(instance, target) & mask) ? rvbl_true : rvbl_false;

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_plic_set_context_threshold(
    const struct rvbl_plic_t *const instance,
    const rvbl_plic_interrupt_context_t context,
    const rvbl_plic_interrupt_priority_t new_threshold
)
{
    if ((context <= RVBL_PLIC_MAX_CONTEXT_ID) && (context <= instance->maximum_context_parameter) &&
        (new_threshold <= instance->maximum_priority_parameter)) {
        rvbl_plic_threshold_write(instance, context, new_threshold);

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_plic_get_context_threshold(
    const struct rvbl_plic_t *const instance,
    const rvbl_plic_interrupt_context_t context,
    rvbl_plic_interrupt_priority_t *const target_threshold
)
{
    if ((context <= RVBL_PLIC_MAX_CONTEXT_ID) && (context <= instance->maximum_context_parameter) &&
        (target_threshold != NULL)) {
        *target_threshold = rvbl_plic_threshold_read(instance, context);

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_plic_claim_interrupt(
    const struct rvbl_plic_t *const instance,
    const rvbl_plic_interrupt_context_t context,
    rvbl_plic_interrupt_source_t *const claimed_source
)
{
    if ((context <= RVBL_PLIC_MAX_CONTEXT_ID) && (context <= instance->maximum_context_parameter) &&
        (claimed_source != NULL)) {
        *claimed_source = rvbl_plic_claim_complete_read(instance, context);

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_plic_complete_interrupt(
    const struct rvbl_plic_t *const instance,
    const rvbl_plic_interrupt_context_t context,
    const rvbl_plic_interrupt_source_t source
)
{
    if ((source >= RVBL_PLIC_MIN_INTERRUPT_SOURCE_ID) &&
        (source <= RVBL_PLIC_MAX_INTERRUPT_SOURCE_ID) && (context <= RVBL_PLIC_MAX_CONTEXT_ID) &&
        (context <= instance->maximum_context_parameter)) {
        rvbl_plic_claim_complete_write(instance, context, source);

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_uword_t rvbl_plic_get_maximum_context(const struct rvbl_plic_t *const instance)
{
    return instance->maximum_context_parameter;
}
