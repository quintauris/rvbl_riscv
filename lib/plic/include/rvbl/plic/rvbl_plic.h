/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef RVBL_PLIC_H
#define RVBL_PLIC_H

/// = Platform-Level Interrrupt Controller (PLIC) Device Library
/// Quintauris GmbH
/// :toc: left
///
/// Implements access to a PLIC device.

#include "rvbl/machine/rvbl_plic.h"
#include "rvbl/type/rvbl_types.h"

/// == Type `rvbl_plic_interrupt_source_t`
///
/// Interrupt source
/// https://github.com/riscv/riscv-plic-spec/blob/master/riscv-plic.adoc#4-interrupt-identifiers-ids[identification].
typedef rvbl_uword_t rvbl_plic_interrupt_source_t;

/// == Type `rvbl_plic_interrupt_priority_t`
///
/// Interrupt
/// https://github.com/riscv/riscv-plic-spec/blob/master/riscv-plic.adoc#3-interrupt-priorities[priority].
typedef rvbl_uint32_t rvbl_plic_interrupt_priority_t;

/// == Type `rvbl_plic_interrupt_context_t`
///
/// Interrupt
/// https://github.com/riscv/riscv-plic-spec/blob/master/riscv-plic.adoc#1-interrupt-targets-and-hart-contexts[identification].
typedef rvbl_uword_t rvbl_plic_interrupt_context_t;

/// == Macro `RVBL_PLIC_INVALID_INTERRUPT_SOURCE_ID`
///
/// Invalid interrupt source
/// https://github.com/riscv/riscv-plic-spec/blob/master/riscv-plic.adoc#4-interrupt-identifiers-ids[identification].
#define RVBL_PLIC_INVALID_INTERRUPT_SOURCE_ID (0)

/// == Macro `RVBL_PLIC_MIN_INTERRUPT_SOURCE_ID`
///
/// Minimum interrupt source identification.
#define RVBL_PLIC_MIN_INTERRUPT_SOURCE_ID (1)

/// == Macro `RVBL_PLIC_MAX_INTERRUPT_SOURCE_ID`
///
/// Maximum interrupt source identification.
#define RVBL_PLIC_MAX_INTERRUPT_SOURCE_ID (1023)

/// == Macro `RVBL_PLIC_MIN_CONTEXT_ID`
///
/// Minimum context identification.
#define RVBL_PLIC_MIN_CONTEXT_ID (0)

/// == Macro `RVBL_PLIC_MAX_CONTEXT_ID`
///
/// Maximum context identification.
#define RVBL_PLIC_MAX_CONTEXT_ID (15871)

/// == Function `rvbl_plic_set_interrupt_priority`
///
/// Sets priority of an interrupt source.
///
/// === Parameters
///
/// `const struct rvbl_plic_t*`:: Device instance identification.
/// `rvbl_plic_interrupt_source_t`:: Interrupt identification.
/// `rvbl_plic_interrupt_priority_t`:: New interrupt priority.
///
/// === Return Value
///
/// `rvbl_result_success`:: Modification successful.
/// `rvbl_result_error_bounds`:: Source or priority values out of bounds.
rvbl_result_t rvbl_plic_set_interrupt_priority(
    const struct rvbl_plic_t *, rvbl_plic_interrupt_source_t, rvbl_plic_interrupt_priority_t
);

/// == Function `rvbl_plic_get_interrupt_priority`
///
/// Gets priority of an interrupt source.
///
/// === Parameters
///
/// `const struct rvbl_plic_t*`:: Device instance identification.
/// `rvbl_plic_interrupt_source_t`:: Interrupt identification.
/// `rvbl_plic_interrupt_priority_t*`:: Interrupt priority destination.
///
/// === Return Value
///
/// `rvbl_result_success`:: Access successful.
/// `rvbl_result_error_bounds`:: Source out of bounds, or priority pointer NULL.
rvbl_result_t rvbl_plic_get_interrupt_priority(
    const struct rvbl_plic_t *, rvbl_plic_interrupt_source_t, rvbl_plic_interrupt_priority_t *
);

/// == Function `rvbl_plic_set_interrupt_pending`
///
/// Sets whether an interrupt source is pending.
///
/// === Parameters
///
/// `const struct rvbl_plic_t*`:: Device instance identification.
/// `rvbl_plic_interrupt_source_t`:: Interrupt identification.
///
/// === Return Value
///
/// `rvbl_result_success`:: Access successful.
/// `rvbl_result_error_bounds`:: Source out of bounds.
rvbl_result_t
rvbl_plic_set_interrupt_pending(const struct rvbl_plic_t *, rvbl_plic_interrupt_source_t);

/// == Function `rvbl_plic_get_interrupt_pending`
///
/// Gets whether an interrupt source is pending.
///
/// === Parameters
///
/// `const struct rvbl_plic_t*`:: Device instance identification.
/// `rvbl_plic_interrupt_source_t`:: Interrupt identification.
/// `rvbl_bool_t*`:: Pending status destination.
///
/// === Return Value
///
/// `rvbl_result_success`:: Access successful.
/// `rvbl_result_error_bounds`:: Source out of bounds or status pointer NULL.
rvbl_result_t rvbl_plic_get_interrupt_pending(
    const struct rvbl_plic_t *, rvbl_plic_interrupt_source_t, rvbl_bool_t *
);

/// == Function `rvbl_plic_set_interrupt_enable`
///
/// Enables or disables an interrupt source.
///
/// === Parameters
///
/// `const struct rvbl_plic_t*`:: Device instance identification.
/// `rvbl_plic_interrupt_source_t`:: Interrupt identification.
/// `rvbl_bool_t`:: Whether interrupt is enabled.
///
/// === Return Value
///
/// `rvbl_result_success`:: Modification successful.
/// `rvbl_result_error_bounds`:: Source or context out of bounds.
rvbl_result_t rvbl_plic_set_interrupt_enable(
    const struct rvbl_plic_t *,
    rvbl_plic_interrupt_source_t,
    rvbl_plic_interrupt_context_t,
    rvbl_bool_t
);

/// == Function `rvbl_plic_get_interrupt_enable`
///
/// Gets whether an interrupt source is enabled or disabled.
///
/// === Parameters
///
/// `const struct rvbl_plic_t*`:: Device instance identification.
/// `rvbl_plic_interrupt_source_t`:: Interrupt identification.
/// `rvbl_bool_t*`:: Enable status destination.
///
/// === Return Value
///
/// `rvbl_result_success`:: Access successful.
/// `rvbl_result_error_bounds`:: Source or context out of bounds, or status
/// pointer NULL.
rvbl_result_t rvbl_plic_get_interrupt_enable(
    const struct rvbl_plic_t *,
    rvbl_plic_interrupt_source_t,
    rvbl_plic_interrupt_context_t,
    rvbl_bool_t *
);

/// == Function `rvbl_plic_set_context_threshold`
///
/// Sets interrupt priority threshold for a context.
///
/// === Parameters
///
/// `const struct rvbl_plic_t*`:: Device instance identification.
/// `rvbl_plic_interrupt_context_t`:: Interrupt context.
/// `rvbl_plic_interrupt_priority_t`:: New priority threshold.
///
/// === Return Value
///
/// `rvbl_result_success`:: Modification successful.
/// `rvbl_result_error_bounds`:: Context out of bounds.
rvbl_result_t rvbl_plic_set_context_threshold(
    const struct rvbl_plic_t *, rvbl_plic_interrupt_context_t, rvbl_plic_interrupt_priority_t
);

/// == Function `rvbl_plic_get_context_threshold`
///
/// Gets interrupt priority threshold for a context.
///
/// === Parameters
///
/// `const struct rvbl_plic_t*`:: Device instance identification.
/// `rvbl_plic_interrupt_context_t`:: Interrupt context.
/// `rvbl_plic_interrupt_priority_t*`:: Priority threshold destination.
///
/// === Return Value
///
/// `rvbl_result_success`:: Access successful.
/// `rvbl_result_error_bounds`:: Context out of bounds or status
/// pointer NULL.
rvbl_result_t rvbl_plic_get_context_threshold(
    const struct rvbl_plic_t *, rvbl_plic_interrupt_context_t, rvbl_plic_interrupt_priority_t *
);

/// == Function `rvbl_plic_claim_interrupt`
///
/// Claims the highest-priority interrupt in a given context.
///
/// === Parameters
///
/// `const struct rvbl_plic_t*`:: Device instance identification.
/// `rvbl_plic_interrupt_context_t`:: Interrupt context.
/// `rvbl_plic_interrupt_source_t*`:: Claimed interrupt identification destination.
///
/// === Return Value
///
/// `rvbl_result_success`:: Claim successful.
/// `rvbl_result_error_bounds`:: Context out of bounds, or destination pointer
/// NULL.
rvbl_result_t rvbl_plic_claim_interrupt(
    const struct rvbl_plic_t *, rvbl_plic_interrupt_context_t, rvbl_plic_interrupt_source_t *
);

/// == Function `rvbl_plic_complete_interrupt`
///
/// Informs PLIC of interrupt processing completion.
///
/// === Parameters
///
/// `const struct rvbl_plic_t*`:: Device instance identification.
/// `rvbl_plic_interrupt_context_t`:: Interrupt context.
/// `rvbl_plic_interrupt_source_t*`:: Identification of completed interrupt.
///
/// === Return Value
///
/// `rvbl_result_success`:: Complete successful.
/// `rvbl_result_error_bounds`:: Context or source out of bounds.
rvbl_result_t rvbl_plic_complete_interrupt(
    const struct rvbl_plic_t *, rvbl_plic_interrupt_context_t, rvbl_plic_interrupt_source_t
);

/// == Function `rvbl_plic_get_maximum_context`
///
/// Retrieves the maximum viable interrupt context value.
///
/// === Parameters
///
/// `const struct rvbl_plic_t*`:: Device instance identification.
///
/// === Return Value
///
/// `rvbl_uword_t`:: Maximum viable interrupt context value.
rvbl_uword_t rvbl_plic_get_maximum_context(const struct rvbl_plic_t *);

#endif
