/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef RVBL_IMSIC_FILE_H
#define RVBL_IMSIC_FILE_H

#include "rvbl/machine/rvbl_imsic.h"
#include "rvbl_common.h"

/// == Function `rvbl_imsic_file_set_interrupt_pending`
///
/// Sets Pending bit for Interrupt in extraneous HART.
///
/// === Parameters
///
/// `const struct rvbl_imsic_t*`:: IMSIC device to use when triggering interrupt.
/// `rvbl_imsic_hart_t`:: Target HART identifier.
/// `rvbl_imsic_hart_mode_t`:: Target HART mode.
/// `rvbl_imsic_interrupt_source_t`:: Target HART interupt identifier.
///
/// === Return Value
///
/// `rvbl_result_success`:: Triggering successful.
/// `rvbl_result_error_bounds`:: Hart or mode out of bounds.
rvbl_result_t rvbl_imsic_file_set_interrupt_pending(
    const struct rvbl_imsic_t *,
    rvbl_imsic_hart_t,
    rvbl_imsic_hart_mode_t,
    rvbl_imsic_interrupt_source_t
);

#endif
