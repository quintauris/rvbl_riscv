/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef RVBL_PMP_H
#define RVBL_PMP_H

#include "rvbl/type/rvbl_types.h"

/// == Enumeration `rvbl_memory_pmp_region_access`
///
/// RISC-V PMP valid access control policies.
typedef enum rvbl_memory_pmp_region_access
{
    /// * `rvbl_memory_pmp_access_rwx` = `7`
    rvbl_memory_pmp_access_rwx = 7,
    /// * `rvbl_memory_pmp_access_rw` = `3`
    rvbl_memory_pmp_access_rw = 3,
    /// * `rvbl_memory_pmp_access_r` = `1`
    rvbl_memory_pmp_access_r = 1,
    /// * `rvbl_memory_pmp_access_rx` = `5`
    rvbl_memory_pmp_access_rx = 5,
    /// * `rvbl_memory_pmp_access_x` = `4`
    rvbl_memory_pmp_access_x = 4,
    /// * `rvbl_memory_pmp_access_none` = `0`
    rvbl_memory_pmp_access_none = 0
} rvbl_memory_pmp_region_access;

/// == Type `rvbl_memory_pmp_region`
///
/// Memory region descriptor:
typedef struct rvbl_memory_pmp_region
{
    /// `rvbl_uword_t base`:: Memory region base address.
    rvbl_uword_t base;
    /// `rvbl_uword_t size`:: Memory region size, in bytes.
    rvbl_uword_t size;
    /// `rvbl_memory_pmp_region_access access`:: Memory region access policy.
    rvbl_memory_pmp_region_access access;

    /// `rvbl_bool_t locked`:: Whether memory control registers are locked.
    rvbl_bool_t locked;
} rvbl_memory_pmp_region;

/// == Type `rvbl_memory_pmp_region_configuration_register`
///
/// RISC-V `pmpcfg` memory region configuration type.
typedef rvbl_uint8_t rvbl_memory_pmp_region_configuration_register;

/// == Type `rvbl_memory_pmp_region_address_register`
///
/// RISC-V `pmpaddr` memory region address (and possibly size) type.
typedef rvbl_uword_t rvbl_memory_pmp_region_address_register;

/// == Function `rvbl_memory_pmp_generate`
///
/// Generates RISC-V PMP register sets from a given memory region configuration
/// list.
///
/// === Parameters
/// `const rvbl_memory_pmp_region*`:: Pointer to sequence of memory regions (
/// sorted from lower to higher addresses).
/// `rvbl_uword_t`:: Number of memory regions.
/// `rvbl_memory_pmp_region_configuration_register*`:: Pointer to array where
/// resulting `pmpcfgX` registers will be stored.
/// `rvbl_memory_pmp_region_address_register*`:: Pointer to array where
/// resulting `pmpaddrX` registers will be stored.
/// `rvbl_uword_t`:: Number of registers in each of the previous two parameters.
///
/// === Return value
/// `rvbl_result_t`::
/// * `rvbl_result_success` if operation is successful.
/// * `rvbl_result_error_unsupported` if supervisor mode is unsupported.
rvbl_result_t rvbl_memory_pmp_generate(
    const rvbl_memory_pmp_region *,
    rvbl_uword_t,
    rvbl_memory_pmp_region_configuration_register *,
    rvbl_memory_pmp_region_address_register *,
    rvbl_uword_t
);

#endif
