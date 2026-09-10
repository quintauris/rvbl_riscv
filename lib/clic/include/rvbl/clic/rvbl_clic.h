/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef RVBL_PLIC_H
#define RVBL_PLIC_H

/// = Core-Local Interrrupt Controller (CLIC) Device Library
/// Quintauris GmbH
/// :toc: left
///
/// Implements access to a CLIC device.

#include "rvbl/alloc/rvbl_alloc.h"
#include "rvbl/interrupt/rvbl_hart.h"

/// == Type `rvbl_clic_interrupt_input_t`
///
/// Interrupt input
typedef rvbl_uword_t rvbl_clic_interrupt_input_t;

/// == Type `rvbl_clic_interrupt_level_t`
///
/// Interrupt level
typedef rvbl_uint32_t rvbl_clic_interrupt_level_t;

/// == Type `rvbl_clic_interrupt_priority_t`
///
/// Interrupt priority
typedef rvbl_uint32_t rvbl_clic_interrupt_priority_t;

/// == Enumeration `rvbl_clic_mode`
typedef enum rvbl_clic_mode
{
    /// * `rvbl_clic_mode_user` = `0`
    rvbl_clic_mode_user = rvbl_clic_registers__int_attr_mode_values_user,
    /// * `rvbl_clic_mode_supervisor` = `1`
    rvbl_clic_mode_supervisor = rvbl_clic_registers__int_attr_mode_values_supervisor,
    /// * `rvbl_clic_mode_machine` = `3`
    rvbl_clic_mode_machine = rvbl_clic_registers__int_attr_mode_values_machine,
} rvbl_clic_mode_t;

/// == Enumeration `rvbl_clic_trigger`
typedef enum rvbl_clic_trigger
{
    /// * `rvbl_clic_trigger_positive_level` = `0`
    rvbl_clic_trigger_positive_level = rvbl_clic_registers__int_attr_trig_values_positive_level,
    /// * `rvbl_clic_trigger_positive_edge` = `1`
    rvbl_clic_trigger_positive_edge = rvbl_clic_registers__int_attr_trig_values_positive_edge,
    /// * `rvbl_clic_trigger_negative_level` = `2`
    rvbl_clic_trigger_negative_level = rvbl_clic_registers__int_attr_trig_values_negative_level,
    /// * `rvbl_clic_trigger_negative_edge` = `3`
    rvbl_clic_trigger_negative_edge = rvbl_clic_registers__int_attr_trig_values_negative_edge,
} rvbl_clic_trigger_t;

/// == Type `rvbl_clic_hart_context`
///
/// Alias for `interrupt`'s `rvbl_interrupt_hart_context`.
typedef rvbl_interrupt_hart_context rvbl_clic_hart_context;

/// == Type `rvbl_clic_trap_handler`
///
/// Alias for `interrupt`'s `rvbl_interrupt_trap_handler`.
typedef rvbl_interrupt_trap_handler rvbl_clic_trap_handler;

/// == Type `rvbl_clic_direct_interrupt_handler`
///
/// Function type representing a direct interrupt handler.
///
/// IMPORTANT: Direct interrupt handlers are called directly when the interrupt
/// happens, with register saving and other context-saving chores to be assumed
/// by the handler.
typedef void (*rvbl_clic_direct_interrupt_handler)(void);

/// == Macro `RVBL_CLIC_MIN_INTERRUPT_INPUT_ID`
///
/// Minimum interrupt input identification.
#define RVBL_CLIC_MIN_INTERRUPT_INPUT_ID (0)

/// == Macro `RVBL_CLIC_MAX_INTERRUPT_INPUT_ID`
///
/// Maximum interrupt input identification.
#define RVBL_CLIC_MAX_INTERRUPT_INPUT_ID (4096)

/// == Macro `RVBL_CLIC_MIN_INTERRUPT_LEVEL`
///
/// Minimum interrupt level.
#define RVBL_CLIC_MIN_INTERRUPT_LEVEL (0)

/// == Macro `RVBL_CLIC_MAX_INTERRUPT_LEVEL`
///
/// Maximum interrupt level.
#define RVBL_CLIC_MAX_INTERRUPT_LEVEL (255)

/// == Macro `RVBL_CLIC_MIN_INTERRUPT_PRIORITY`
///
/// Minimum interrupt priority.
#define RVBL_CLIC_MIN_INTERRUPT_PRIORITY (0)

/// == Macro `RVBL_CLIC_MAX_INTERRUPT_PRIORITY`
///
/// Maximum interrupt priority.
#define RVBL_CLIC_MAX_INTERRUPT_PRIORITY (255)

/// == Function `rvbl_clic_set_interrupt_priority`
///
/// Sets priority of an interrupt input.
///
/// === Parameters
///
/// `const struct rvbl_clic_t*`:: Device instance identification.
/// `rvbl_clic_interrupt_input_t`:: Interrupt identification.
/// `rvbl_clic_interrupt_level_t`:: New interrupt level.
/// `rvbl_clic_interrupt_priority_t`:: New interrupt priority.
///
/// === Return Value
///
/// `rvbl_result_success`:: Modification successful.
/// `rvbl_result_error_bounds`:: Input, level or priority values out of bounds.
rvbl_result_t rvbl_clic_set_interrupt_priority(
    const struct rvbl_clic_t *,
    rvbl_clic_interrupt_input_t,
    rvbl_clic_interrupt_level_t,
    rvbl_clic_interrupt_priority_t
);

/// == Function `rvbl_clic_get_interrupt_priority`
///
/// Gets priority of an interrupt input.
///
/// === Parameters
///
/// `const struct rvbl_clic_t*`:: Device instance identification.
/// `rvbl_clic_interrupt_input_t`:: Interrupt identification.
/// `rvbl_clic_interrupt_level_t*`:: Interrupt level destination.
/// `rvbl_clic_interrupt_priority_t*`:: Interrupt priority destination.
///
/// === Return Value
///
/// `rvbl_result_success`:: Access successful.
/// `rvbl_result_error_bounds`:: Input out of bounds, or level/priority pointers
/// NULL.
rvbl_result_t rvbl_clic_get_interrupt_priority(
    const struct rvbl_clic_t *,
    rvbl_clic_interrupt_input_t,
    rvbl_clic_interrupt_level_t *,
    rvbl_clic_interrupt_priority_t *
);

/// == Function `rvbl_clic_set_interrupt_pending`
///
/// Sets whether an interrupt input is pending.
///
/// === Parameters
///
/// `const struct rvbl_clic_t*`:: Device instance identification.
/// `rvbl_clic_interrupt_input_t`:: Interrupt identification.
/// `rvbl_bool_t`:: Pending status to set.
///
/// === Return Value
///
/// `rvbl_result_success`:: Access successful.
/// `rvbl_result_error_bounds`:: Input out of bounds.
rvbl_result_t rvbl_clic_set_interrupt_pending(
    const struct rvbl_clic_t *, rvbl_clic_interrupt_input_t, rvbl_bool_t
);

/// == Function `rvbl_clic_get_interrupt_pending`
///
/// Gets whether an interrupt input is pending.
///
/// === Parameters
///
/// `const struct rvbl_clic_t*`:: Device instance identification.
/// `rvbl_clic_interrupt_input_t`:: Interrupt identification.
/// `rvbl_bool_t*`:: Pending status destination.
///
/// === Return Value
///
/// `rvbl_result_success`:: Access successful.
/// `rvbl_result_error_bounds`:: Input out of bounds or status pointer NULL.
rvbl_result_t rvbl_clic_get_interrupt_pending(
    const struct rvbl_clic_t *, rvbl_clic_interrupt_input_t, rvbl_bool_t *
);

/// == Function `rvbl_clic_set_interrupt_enable`
///
/// Enables or disables an interrupt input.
///
/// === Parameters
///
/// `const struct rvbl_clic_t*`:: Device instance identification.
/// `rvbl_clic_interrupt_input_t`:: Interrupt identification.
/// `rvbl_bool_t`:: Whether interrupt is enabled.
///
/// === Return Value
///
/// `rvbl_result_success`:: Modification successful.
/// `rvbl_result_error_bounds`:: Input or context out of bounds.
rvbl_result_t rvbl_clic_set_interrupt_enable(
    const struct rvbl_clic_t *, rvbl_clic_interrupt_input_t, rvbl_bool_t
);

/// == Function `rvbl_clic_get_interrupt_enable`
///
/// Gets whether an interrupt input is enabled or disabled.
///
/// === Parameters
///
/// `const struct rvbl_clic_t*`:: Device instance identification.
/// `rvbl_clic_interrupt_input_t`:: Interrupt identification.
/// `rvbl_bool_t*`:: Enable status destination.
///
/// === Return Value
///
/// `rvbl_result_success`:: Access successful.
/// `rvbl_result_error_bounds`:: Input or context out of bounds, or status
/// pointer NULL.
rvbl_result_t rvbl_clic_get_interrupt_enable(
    const struct rvbl_clic_t *, rvbl_clic_interrupt_input_t, rvbl_bool_t *
);

/// == Function `rvbl_clic_set_interrupt_attributes`
///
/// Sets interrupt SHV, trigger and privilege attributes.
///
/// === Parameters
///
/// `const struct rvbl_clic_t*`:: Device instance identification.
/// `rvbl_clic_interrupt_input_t`:: Interrupt identification.
/// `rvbl_bool_t`:: Whether to enable Selective Hardware Vectorization (SHV).
/// `rvbl_clic_trigger_t`:: Interrupt trigger mode.
/// `rvbl_clic_mode_t`:: Interrupt privilege mode.
///
/// === Return Value
///
/// `rvbl_result_success`:: Modification successful.
/// `rvbl_result_error_bounds`:: Interrupt identification out of bounds.
rvbl_result_t rvbl_clic_set_interrupt_attributes(
    const struct rvbl_clic_t *,
    rvbl_clic_interrupt_input_t,
    rvbl_bool_t,
    rvbl_clic_trigger_t,
    rvbl_clic_mode_t
);

/// == Function `rvbl_clic_get_interrupt_attributes`
///
/// Gets interrupt SHV, trigger and privilege attributes.
///
/// === Parameters
///
/// `const struct rvbl_clic_t*`:: Device instance identification.
/// `rvbl_clic_interrupt_input_t`:: Interrupt identification.
/// `rvbl_bool_t*`:: Selective Hardware Vectorization (SHV) enable destination.
/// `rvbl_clic_trigger_t*`:: Interrupt trigger mode destination.
/// `rvbl_clic_mode_t*`:: Interrupt privilege mode destination.
///
/// === Return Value
///
/// `rvbl_result_success`:: Modification successful.
/// `rvbl_result_error_bounds`:: Interrupt identification out of bounds.
rvbl_result_t rvbl_clic_get_interrupt_attributes(
    const struct rvbl_clic_t *,
    rvbl_clic_interrupt_input_t,
    rvbl_bool_t *,
    rvbl_clic_trigger_t *,
    rvbl_clic_mode_t *
);

/// == Function `rvbl_clic_allocate`
///
/// Allocates interrupt-handling structures.
///
/// === Parameters
///
/// `rvbl_alloc_allocator*`:: Pointer to allocator for internal structures.
/// `rvbl_uword_t`:: Number of platform (i.e. non-standard) interrupts to
/// support.
///
/// === Return Value
///
/// `rvbl_result_t`:: Always `rvbl_result_success`.
rvbl_result_t rvbl_clic_allocate(rvbl_alloc_allocator *, rvbl_uword_t);

/// == Function `rvbl_clic_initialize`
///
/// Initializes interrupt-handling per-HART structures and registers.
///
/// === Parameters
///
/// `rvbl_alloc_allocator*`:: Pointer to allocator for internal structures.
/// `rvbl_clic_trap_handler`:: Pointer to default interrupt handler.
///
/// === Return Value
///
/// `rvbl_result_t`:: Always `rvbl_result_success`.
rvbl_result_t rvbl_clic_initialize(rvbl_alloc_allocator *, rvbl_clic_trap_handler);

/// == Function `rvbl_clic_set_interrupt_handler`
///
/// Assigns an interrupt handler to an interrupt.
///
/// === Parameters
/// `const struct rvbl_clic_t*`:: Device instance identification.
/// `rvbl_clic_interrupt_input_t`:: Interrupt identification (note first 16
/// interrupt numbers are reserved).
/// `rvbl_clic_trap_handler`:: Pointer to custom interrupt handler.
///
/// === Return Value
///
/// `rvbl_result_success`:: Modification successful.
/// `rvbl_result_error_bounds`:: Interrupt identification out of bounds.
rvbl_result_t rvbl_clic_set_interrupt_handler(
    const struct rvbl_clic_t *, rvbl_clic_interrupt_input_t, rvbl_clic_trap_handler
);

/// == Function `rvbl_clic_set_interrupt_handler`
///
/// Assigns a direct interrupt handler to an interrupt.
///
/// === Parameters
/// `const struct rvbl_clic_t*`:: Device instance identification.
/// `rvbl_clic_interrupt_input_t`:: Interrupt identification (note first 16
/// interrupt numbers are reserved).
/// `rvbl_clic_direct_interrupt_handler`:: Pointer to custom interrupt handler.
///
/// === Return Value
///
/// `rvbl_result_success`:: Modification successful.
/// `rvbl_result_error_bounds`:: Interrupt identification out of bounds.
rvbl_result_t rvbl_clic_set_interrupt_handler_direct(
    const struct rvbl_clic_t *instance,
    const rvbl_clic_interrupt_input_t input,
    const rvbl_clic_direct_interrupt_handler handler
);

#endif
