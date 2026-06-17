/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef RVBL_APLIC_H
#define RVBL_APLIC_H

/// = Advanced Platform-Level Interrrupt Controller (APLIC) Device Library
/// Quintauris GmbH
/// :toc: left
///
/// Implements access to an APLIC device.

#include "rvbl/machine/rvbl_aplic.h"
#include "rvbl/type/rvbl_types.h"

/// == Type `rvbl_aplic_interrupt_source_t`
///
/// Interrupt source (The RISC-V Advanced Interrupt Architecture, chapter 3.1).
typedef rvbl_uword_t rvbl_aplic_interrupt_source_t;

/// == Type `rvbl_aplic_interrupt_priority_t`
///
/// Interrupt priority (The RISC-V Advanced Interrupt Architecture, chapter 3.3).
typedef rvbl_uint32_t rvbl_aplic_interrupt_priority_t;

/// == Type `rvbl_aplic_hart_t`
///
/// RISC-V HART index.
typedef rvbl_uword_t rvbl_aplic_hart_t;

/// == Type `rvbl_aplic_interrupt_domain_t`
///
/// Interrupt domain (The RISC-V Advanced Interrupt Architecture, chapter 4.2).
typedef rvbl_uword_t rvbl_aplic_interrupt_domain_t;

/// == Type `rvbl_aplic_source_mode_t`
///
/// Interrupt domain (The RISC-V Advanced Interrupt Architecture, chapter 4.5.2, table 7).
typedef aplic_sourcecfg_source_mode_values rvbl_aplic_source_mode_t;

/// == Type `rvbl_aplic_source_configuration_t`
///
/// Interrupt source configuration (The RISC-V Advanced Interrupt Architecture, chapter 4.5.2).
typedef struct rvbl_aplic_source_configuration
{
    rvbl_bool_t delgated;
    union
    {
        rvbl_aplic_interrupt_domain_t domain;
        rvbl_aplic_source_mode_t source_mode;
    };
} rvbl_aplic_source_configuration_t;

/// == Type `rvbl_aplic_target_configuration_direct_t`
///
/// Interrupt target configuration (Direct, The RISC-V Advanced Interrupt Architecture,
/// chapter 4.5.16.1).
typedef struct rvbl_aplic_target_configuration_direct
{
    rvbl_aplic_hart_t hart;
    rvbl_aplic_interrupt_priority_t priority;
} rvbl_aplic_target_configuration_direct_t;

/// == Type `rvbl_aplic_target_configuration_msi_t`
///
/// Interrupt target configuration (MSI, The RISC-V Advanced Interrupt Architecture,
/// chapter 4.5.16.2).
typedef struct rvbl_aplic_target_configuration_msi
{
    rvbl_aplic_hart_t hart;
    rvbl_aplic_hart_t guest;
    rvbl_aplic_interrupt_source_t id;
} rvbl_aplic_target_configuration_msi_t;

/// == Macro `RVBL_APLIC_INVALID_INTERRUPT_SOURCE_ID`
///
/// Invalid interrupt source
/// https://github.com/riscv/riscv-aplic-spec/blob/master/riscv-aplic.adoc#4-interrupt-identifiers-ids[identification].
#define RVBL_APLIC_INVALID_INTERRUPT_SOURCE_ID (0)

/// == Macro `RVBL_APLIC_MIN_INTERRUPT_SOURCE_ID`
///
/// Minimum interrupt source identification.
#define RVBL_APLIC_MIN_INTERRUPT_SOURCE_ID (1)

/// == Macro `RVBL_APLIC_MAX_INTERRUPT_SOURCE_ID`
///
/// Maximum interrupt source identification.
#define RVBL_APLIC_MAX_INTERRUPT_SOURCE_ID (1023)

/// == Macro `RVBL_APLIC_MIN_INTERRUPT_SOURCE_ID`
///
/// Minimum interrupt target identification.
#define RVBL_APLIC_MIN_INTERRUPT_TARGET_ID (1)

/// == Macro `RVBL_APLIC_MAX_INTERRUPT_SOURCE_ID`
///
/// Maximum interrupt target identification.
#define RVBL_APLIC_MAX_INTERRUPT_TARGET_ID (1023)

/// == Function `rvbl_aplic_set_source_configuration`
///
/// Sets configuration of an interrupt source.
///
/// === Parameters
///
/// `const struct rvbl_aplic_t*`:: Device instance identification.
/// `rvbl_aplic_interrupt_source_t`:: Interrupt source identification.
/// `const rvbl_aplic_source_configuration_t*`:: New interrupt configuration.
///
/// === Return Value
///
/// `rvbl_result_success`:: Modification successful.
/// `rvbl_result_error_bounds`:: Source value out of bounds.
rvbl_result_t rvbl_aplic_set_source_configuration(
    const struct rvbl_aplic_t *,
    rvbl_aplic_interrupt_source_t,
    const rvbl_aplic_source_configuration_t *
);

/// == Function `rvbl_aplic_get_source_configuration`
///
/// Gets configuration of an interrupt source.
///
/// === Parameters
///
/// `const struct rvbl_aplic_t*`:: Device instance identification.
/// `rvbl_aplic_interrupt_source_t`:: Interrupt source identification.
/// `rvbl_aplic_source_configuration_t*`:: Current nterrupt priority.
///
/// === Return Value
///
/// `rvbl_result_success`:: Modification successful.
/// `rvbl_result_error_bounds`:: Source value out of bounds.
rvbl_result_t rvbl_aplic_get_source_configuration(
    const struct rvbl_aplic_t *, rvbl_aplic_interrupt_source_t, rvbl_aplic_source_configuration_t *
);

/// == Function `rvbl_aplic_set_interrupt_pending`
///
/// Sets an interrupt source to pending.
///
/// === Parameters
///
/// `const struct rvbl_aplic_t*`:: Device instance identification.
/// `rvbl_aplic_interrupt_source_t`:: Interrupt source identification.
///
/// === Return Value
///
/// `rvbl_result_success`:: Access successful.
/// `rvbl_result_error_bounds`:: Source out of bounds.
rvbl_result_t
rvbl_aplic_set_interrupt_pending(const struct rvbl_aplic_t *, rvbl_aplic_interrupt_source_t);

/// == Function `rvbl_aplic_get_interrupt_pending`
///
/// Gets whether an interrupt source is pending.
///
/// === Parameters
///
/// `const struct rvbl_aplic_t*`:: Device instance identification.
/// `rvbl_aplic_interrupt_source_t`:: Interrupt source identification.
/// `rvbl_bool_t*`:: Pending status destination.
///
/// === Return Value
///
/// `rvbl_result_success`:: Access successful.
/// `rvbl_result_error_bounds`:: Source out of bounds or status pointer NULL.
rvbl_result_t rvbl_aplic_get_interrupt_pending(
    const struct rvbl_aplic_t *, rvbl_aplic_interrupt_source_t, rvbl_bool_t *
);

/// == Function `rvbl_aplic_clear_interrupt_pending_rectified`
///
/// Sets an interrupt source to not pending.
///
/// === Parameters
///
/// `const struct rvbl_aplic_t*`:: Device instance identification.
/// `rvbl_aplic_interrupt_source_t`:: Interrupt source identification.
///
/// === Return Value
///
/// `rvbl_result_success`:: Access successful.
/// `rvbl_result_error_bounds`:: Source out of bounds.
rvbl_result_t
rvbl_aplic_clear_interrupt_pending(const struct rvbl_aplic_t *, rvbl_aplic_interrupt_source_t);

/// == Function `rvbl_aplic_get_interrupt_pending_rectified`
///
/// Gets whether an interrupt source is pending.
///
/// === Parameters
///
/// `const struct rvbl_aplic_t*`:: Device instance identification.
/// `rvbl_aplic_interrupt_source_t`:: Interrupt source identification.
/// `rvbl_bool_t*`:: Pending status destination.
///
/// === Return Value
///
/// `rvbl_result_success`:: Access successful.
/// `rvbl_result_error_bounds`:: Source out of bounds or status pointer NULL.
rvbl_result_t rvbl_aplic_get_interrupt_pending_rectified(
    const struct rvbl_aplic_t *, rvbl_aplic_interrupt_source_t, rvbl_bool_t *
);

/// == Function `rvbl_aplic_set_interrupt_enable`
///
/// Enables or disables an interrupt source.
///
/// === Parameters
///
/// `const struct rvbl_aplic_t*`:: Device instance identification.
/// `rvbl_aplic_interrupt_source_t`:: Interrupt source identification.
/// `rvbl_bool_t`:: Whether interrupt is enabled.
///
/// === Return Value
///
/// `rvbl_result_success`:: Modification successful.
/// `rvbl_result_error_bounds`:: Source or context out of bounds.
rvbl_result_t rvbl_aplic_set_interrupt_enable(
    const struct rvbl_aplic_t *, rvbl_aplic_interrupt_source_t, rvbl_bool_t
);

/// == Function `rvbl_aplic_get_interrupt_enable`
///
/// Gets whether an interrupt source is enabled or disabled.
///
/// === Parameters
///
/// `const struct rvbl_aplic_t*`:: Device instance identification.
/// `rvbl_aplic_interrupt_source_t`:: Interrupt source identification.
/// `rvbl_bool_t*`:: Enable status destination.
///
/// === Return Value
///
/// `rvbl_result_success`:: Access successful.
/// `rvbl_result_error_bounds`:: Source or context out of bounds, or status
/// pointer NULL.
rvbl_result_t rvbl_aplic_get_interrupt_enable(
    const struct rvbl_aplic_t *, rvbl_aplic_interrupt_source_t, rvbl_bool_t *
);

/// == Function `rvbl_aplic_generate`
///
/// Generates a Message-Signaled Interrupt (MSI).
///
/// === Parameters
///
/// `const struct rvbl_aplic_t*`:: Device instance identification.
/// `rvbl_aplic_hart_t`:: Target HART.
/// `rvbl_aplic_interrupt_source_t`:: Interrupt source identification.
///
/// === Return Value
///
/// `rvbl_result_success`:: Access successful.
/// `rvbl_result_error_bounds`:: Source out of bounds.
/// `rvbl_result_error_busy`:: `genmsi` registery `busy` bit is set.
rvbl_result_t
rvbl_aplic_generate(const struct rvbl_aplic_t *, rvbl_aplic_hart_t, rvbl_aplic_interrupt_source_t);

/// == Function `rvbl_aplic_set_target_configuration_direct`
///
/// Sets configuration of an interrupt target, direct mode.
///
/// === Parameters
///
/// `const struct rvbl_aplic_t*`:: Device instance identification.
/// `rvbl_aplic_interrupt_source_t`:: Interrupt source identification.
/// `const rvbl_aplic_source_configuration_direct_t*`:: New interrupt configuration.
///
/// === Return Value
///
/// `rvbl_result_success`:: Modification successful.
/// `rvbl_result_error_bounds`:: Source value out of bounds.
/// `rvbl_result_error_state`:: `dm` field of `domaincfg` is not "direct".
rvbl_result_t rvbl_aplic_set_target_configuration_direct(
    const struct rvbl_aplic_t *,
    rvbl_aplic_interrupt_source_t,
    const rvbl_aplic_target_configuration_direct_t *
);

/// == Function `rvbl_aplic_set_target_configuration_direct`
///
/// Gets configuration of an interrupt target, direct mode.
///
/// === Parameters
///
/// `const struct rvbl_aplic_t*`:: Device instance identification.
/// `rvbl_aplic_interrupt_source_t`:: Interrupt source identification.
/// `rvbl_aplic_source_configuration_direct_t*`:: Current interrupt configuration.
///
/// === Return Value
///
/// `rvbl_result_success`:: Modification successful.
/// `rvbl_result_error_bounds`:: Source value out of bounds.
/// `rvbl_result_error_state`:: `dm` field of `domaincfg` is not "direct".
rvbl_result_t rvbl_aplic_get_target_configuration_direct(
    const struct rvbl_aplic_t *,
    rvbl_aplic_interrupt_source_t,
    rvbl_aplic_target_configuration_direct_t *
);

/// == Function `rvbl_aplic_set_target_configuration_msi`
///
/// Sets configuration of an interrupt target, MSI mode.
///
/// === Parameters
///
/// `const struct rvbl_aplic_t*`:: Device instance identification.
/// `rvbl_aplic_interrupt_source_t`:: Interrupt source identification.
/// `const rvbl_aplic_source_configuration_msi_t*`:: New interrupt configuration.
///
/// === Return Value
///
/// `rvbl_result_success`:: Modification successful.
/// `rvbl_result_error_bounds`:: Source value out of bounds.
/// `rvbl_result_error_state`:: `dm` field of `domaincfg` is not "MSI".
rvbl_result_t rvbl_aplic_set_target_configuration_msi(
    const struct rvbl_aplic_t *,
    rvbl_aplic_interrupt_source_t,
    const rvbl_aplic_target_configuration_msi_t *
);

/// == Function `rvbl_aplic_set_target_configuration_msi`
///
/// Gets configuration of an interrupt target, MSI mode.
///
/// === Parameters
///
/// `const struct rvbl_aplic_t*`:: Device instance identification.
/// `rvbl_aplic_interrupt_source_t`:: Interrupt source identification.
/// `rvbl_aplic_source_configuration_msi_t*`:: Current interrupt configuration.
///
/// === Return Value
///
/// `rvbl_result_success`:: Modification successful.
/// `rvbl_result_error_bounds`:: Source value out of bounds.
/// `rvbl_result_error_state`:: `dm` field of `domaincfg` is not "MSI".
rvbl_result_t rvbl_aplic_get_target_configuration_msi(
    const struct rvbl_aplic_t *,
    rvbl_aplic_interrupt_source_t,
    rvbl_aplic_target_configuration_msi_t *
);

#endif
