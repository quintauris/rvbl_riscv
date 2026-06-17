/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef RVBL_IMSIC_COMMON_H
#define RVBL_IMSIC_COMMON_H

/// = Incoming Message-Signaled Interrupt Controller (IMSIC) Device Library
/// Quintauris GmbH
/// :toc: left
///
/// Implements access to the IMSIC device.

#include "rvbl/type/rvbl_types.h"

/// == Type `rvbl_imsic_hart_t`
///
/// RISC-V HART identifier.
typedef rvbl_uword_t rvbl_imsic_hart_t;

/// == Type `rvbl_imsic_interrupt_source_t`
///
/// RISC-V interrupt source identifier.
typedef rvbl_uword_t rvbl_imsic_interrupt_source_t;

/// == Type `rvbl_imsic_interrupt_priority_t`
///
/// RISC-V interrupt priority value (same as identifier).
typedef rvbl_imsic_interrupt_source_t rvbl_imsic_interrupt_priority_t;

/// == Enumeration `rvbl_imsic_hart_mode_t`
///
/// Supported privilege modes for RISC-V HARTs.
typedef enum rvbl_imsic_hart_mode_t
{
    /// `rvbl_imsic_mode_machine` :: Machine mode.
    rvbl_imsic_mode_machine,
    /// `rvbl_imsic_mode_supervisor` :: Supervisor mode.
    rvbl_imsic_mode_supervisor
} rvbl_imsic_hart_mode_t;

/// == Constant `RVBL_DEVICE_IMSIC_INVALID_INTERRUPT_SOURCE`
///
/// Invalid interrupt source value.
#define RVBL_DEVICE_IMSIC_INVALID_INTERRUPT_SOURCE ((rvbl_imsic_interrupt_source_t)0)

#endif
