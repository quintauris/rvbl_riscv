/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef RVBL_IMSIC_HART_H
#define RVBL_IMSIC_HART_H

#include "rvbl_common.h"

/// == Enumeration `rvbl_imsic_hart_delivery_t`
///
/// Supported IMSIC delivery modes for RISC-V HARTs.
typedef enum rvbl_imsic_hart_delivery_t
{
    /// `rvbl_imsic_delivery_disabled` :: Interrupt delivery disabled.
    rvbl_imsic_delivery_disabled = 0,
    /// `rvbl_imsic_delivery_enabled` :: Interrupt delivery enabled.
    rvbl_imsic_delivery_enabled = 1,
    /// `rvbl_imsic_delivery_plic_or_aplic` :: Interrupt delivery through PLIC or APLIC.
    rvbl_imsic_delivery_plic_or_aplic = 0x40000000,
} rvbl_imsic_hart_delivery_t;

/// == Function `rvbl_imsic_hart_set_delivery`
///
/// Sets HART interrupt delivery mode.
///
/// === Parameters
///
/// `rvbl_imsic_hart_mode_t`:: Privilege mode of interrupt delivery mode.
/// `rvbl_imsic_hart_delivery_t`:: New HART interrupt delivery mode.
///
/// === Return Value
///
/// `rvbl_result_success`:: Modification successful.
/// `rvbl_result_error_bounds`:: Mode or privilege out of bounds.
rvbl_result_t rvbl_imsic_hart_set_delivery(rvbl_imsic_hart_mode_t, rvbl_imsic_hart_delivery_t);

/// == Function `rvbl_imsic_hart_get_delivery`
///
/// Gets HART interrupt delivery mode.
///
/// === Parameters
///
/// `rvbl_imsic_hart_mode_t`:: Privilege mode of interrupt delivery mode.
/// `rvbl_imsic_hart_delivery_t*`:: Current HART interrupt delivery mode.
///
/// === Return Value
///
/// `rvbl_result_success`:: Access successful.
/// `rvbl_result_error_bounds`:: Mode or privilege out of bounds.
rvbl_result_t rvbl_imsic_hart_get_delivery(rvbl_imsic_hart_mode_t, rvbl_imsic_hart_delivery_t *);

/// == Function `rvbl_imsic_hart_set_threshold`
///
/// Sets HART interrupt delivery threshold.
///
/// === Parameters
///
/// `rvbl_imsic_hart_mode_t`:: Privilege mode of interrupt threshold.
/// `rvbl_imsic_interrupt_priority_t`:: New HART interrupt threshold.
///
/// === Return Value
///
/// `rvbl_result_success`:: Modification successful.
/// `rvbl_result_error_bounds`:: Mode out of bounds.
rvbl_result_t
    rvbl_imsic_hart_set_threshold(rvbl_imsic_hart_mode_t, rvbl_imsic_interrupt_priority_t);

/// == Function `rvbl_imsic_hart_get_threshold`
///
/// Gets HART interrupt delivery threshold.
///
/// === Parameters
///
/// `rvbl_imsic_hart_mode_t`:: Privilege mode of interrupt threshold.
/// `rvbl_imsic_interrupt_priority_t*`:: Current HART interrupt threshold.
///
/// === Return Value
///
/// `rvbl_result_success`:: Access successful.
/// `rvbl_result_error_bounds`:: Mode out of bounds.
rvbl_result_t
rvbl_imsic_hart_get_threshold(rvbl_imsic_hart_mode_t, rvbl_imsic_interrupt_priority_t *);

/// == Function `rvbl_imsic_hart_set_interrupt_pending`
///
/// Sets HART pending bit for interrupt.
///
/// === Parameters
///
/// `rvbl_imsic_hart_mode_t`:: Privilege mode of interrupt.
/// `rvbl_imsic_interrupt_source_t`:: Interrupt source.
/// `rvbl_bool_t`:: Whether to set or clear Interrupt Pending bit.
///
/// === Return Value
///
/// `rvbl_result_success`:: Modification successful.
/// `rvbl_result_error_bounds`:: Mode or interrupt source out of bounds.
rvbl_result_t rvbl_imsic_hart_set_interrupt_pending(
    rvbl_imsic_hart_mode_t, rvbl_imsic_interrupt_source_t, rvbl_bool_t
);

/// == Function `rvbl_imsic_hart_get_interrupt_pending`
///
/// Gets HART pending bit for interrupt.
///
/// === Parameters
///
/// `rvbl_imsic_hart_mode_t`:: Privilege mode of interrupt.
/// `rvbl_imsic_interrupt_source_t`:: Interrupt source.
/// `rvbl_bool_t*`:: Current Interrupt Pending status.
///
/// === Return Value
///
/// `rvbl_result_success`:: Access successful.
/// `rvbl_result_error_bounds`:: Mode or interrupt source out of bounds.
rvbl_result_t rvbl_imsic_hart_get_interrupt_pending(
    rvbl_imsic_hart_mode_t, rvbl_imsic_interrupt_source_t, rvbl_bool_t *
);

/// == Function `rvbl_imsic_hart_set_interrupt_enable`
///
/// Sets HART enable bit for interrupt.
///
/// === Parameters
///
/// `rvbl_imsic_hart_mode_t`:: Privilege mode of interrupt.
/// `rvbl_imsic_interrupt_source_t`:: Interrupt source.
/// `rvbl_bool_t`:: Whether to set or clear Interrupt Enable bit.
///
/// === Return Value
///
/// `rvbl_result_success`:: Modification successful.
/// `rvbl_result_error_bounds`:: Mode or interrupt source out of bounds.
rvbl_result_t rvbl_imsic_hart_set_interrupt_enable(
    rvbl_imsic_hart_mode_t, rvbl_imsic_interrupt_source_t, rvbl_bool_t
);

/// == Function `rvbl_imsic_hart_get_interrupt_enable`
///
/// Sets HART enable bit for interrupt.
///
/// === Parameters
///
/// `rvbl_imsic_hart_mode_t`:: Privilege mode of interrupt.
/// `rvbl_imsic_interrupt_source_t`:: Interrupt source.
/// `rvbl_bool_t*`:: Current Interrupt Enable status.
///
/// === Return Value
///
/// `rvbl_result_success`:: Access successful.
/// `rvbl_result_error_bounds`:: Mode or interrupt source out of bounds.
rvbl_result_t rvbl_imsic_hart_get_interrupt_enable(
    rvbl_imsic_hart_mode_t, rvbl_imsic_interrupt_source_t, rvbl_bool_t *
);

/// == Function `rvbl_imsic_hart_get_top_interrupt`
///
/// Obtains Top Pending Interrupt with highest Priority.
///
/// === Parameters
///
/// `rvbl_imsic_hart_mode_t`:: Privilege mode of interrupt.
/// `rvbl_imsic_interrupt_source_t`:: Interrupt source.
///
/// === Return Value
///
/// `rvbl_result_success`:: Access successful.
/// `rvbl_result_error_bounds`:: Mode out of bounds.
rvbl_result_t
rvbl_imsic_hart_get_top_interrupt(rvbl_imsic_hart_mode_t, rvbl_imsic_interrupt_source_t *);

#endif
