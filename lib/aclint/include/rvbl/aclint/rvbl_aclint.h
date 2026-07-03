/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef RVBL_ACLINT_H
#define RVBL_ACLINT_H

/// = Advanced Core-Local Interrruptor (ACLINT) Device Library
/// Quintauris GmbH
/// :toc: left
///
/// Implements access to the ACLINT device.

#include "rvbl/machine/rvbl_aclint.h"
#include "rvbl/type/rvbl_types.h"

/// == Type `rvbl_aclint_hart_t`
///
/// HART identification type.
typedef rvbl_uword_t rvbl_aclint_hart_t;

/// == Type `rvbl_aclint_time_t`
///
/// APLIC timer type.
typedef rvbl_uint64_t rvbl_aclint_time_t;

/// == Macro `RVBL_ACLINT_MAX_HART_ID`
///
/// Maximum supported
/// https://github.com/riscvarchive/riscv-aclint/blob/main/riscv-aclint.adoc#table_aclint_mtimer_time_register_list[HART
/// identification].
#define RVBL_ACLINT_MAX_HART_ID (4094)

/// == Function `rvbl_aclint_set_compare`
///
/// Set HART time comparator value.
///
/// === Parameters
///
/// `struct rvbl_aclint_t*`:: Device instance identification.
/// `rvbl_aclint_hart_t`:: HART identification.
/// `rvbl_aclint_time_t`:: New time comparator value.
///
/// === Return Value
///
/// `rvbl_result_success`:: Modification successful.
/// `rvbl_result_error_bounds`:: HART value out of bounds.
rvbl_result_t
rvbl_aclint_set_compare(const struct rvbl_aclint_t *, rvbl_aclint_hart_t, rvbl_aclint_time_t);

/// == Function `rvbl_aclint_get_compare`
///
/// Get HART time comparator value.
///
/// === Parameters
///
/// `struct rvbl_aclint_t*`:: Device instance identification.
/// `rvbl_aclint_hart_t`:: HART identification.
/// `rvbl_aclint_time_t*`:: Time comparator value target.
///
/// === Return Value
///
/// `rvbl_result_success`:: Access successful.
/// `rvbl_result_error_bounds`:: HART value out of bounds.
rvbl_result_t
rvbl_aclint_get_compare(const struct rvbl_aclint_t *, rvbl_aclint_hart_t, rvbl_aclint_time_t *);

/// == Function `rvbl_aclint_machine_set_pending`
///
/// Set HART Machine Software Interrupt Pending (MSIP).
///
/// === Parameters
///
/// `struct rvbl_aclint_t*`:: Device instance identification.
/// `rvbl_aclint_hart_t`:: HART identification.
/// `rvbl_bool_t`:: New pending value.
///
/// === Return Value
///
/// `rvbl_result_success`:: Modification successful.
/// `rvbl_result_error_bounds`:: HART value out of bounds.
rvbl_result_t
rvbl_aclint_machine_set_pending(const struct rvbl_aclint_t *, rvbl_aclint_hart_t, rvbl_bool_t);

/// == Function `rvbl_aclint_supervisor_set_pending`
///
/// Set HART Supervisor Software Interrupt Pending (SSIP).
///
/// === Parameters
///
/// `struct rvbl_aclint_t*`:: Device instance identification.
/// `rvbl_aclint_hart_t`:: HART identification.
/// `rvbl_bool_t`:: New pending value.
///
/// === Return Value
///
/// `rvbl_result_success`:: Modification successful.
/// `rvbl_result_error_bounds`:: HART value out of bounds.
rvbl_result_t
rvbl_aclint_supervisor_set_pending(const struct rvbl_aclint_t *, rvbl_aclint_hart_t, rvbl_bool_t);

#endif
