/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef RVBL_HART_H
#define RVBL_HART_H

#include "rvbl/alloc/rvbl_alloc.h"
#include "rvbl/machine/rvbl_machine.h"
#include "rvbl/type/rvbl_types.h"

/// = RISC-V Base Layer Interrupt Library
/// Quintauris GmbH
/// :toc: left
///
/// Implements trap-related types and functions. Across this module, the generic
/// term _trap_ is used to refer to both RISC-V synchronous exceptions and
/// asynchrounous interrupts.

/// == Type `rvbl_interrupt_hart_context`
///
/// In-memory representation of a RISC-V HART context, including:
///
/// * All RISC-V architecture general-purpose registers (`x1`-`x31`) labeled
///   according to their ABI meanings
/// * Program Counter register (`m`/`sepc`)
/// * Interrupt Cause register (`m`/`scause`)
/// * Interrupt Value register (`m`/`sval`)
typedef struct rvbl_interrupt_hart_context
{
    rvbl_uword_t return_address;
    rvbl_uword_t stack_pointer;
    rvbl_uword_t global_pointer;
    rvbl_uword_t thread_pointer;
    rvbl_uword_t temporaries_top[3];
    rvbl_uword_t saved_top[2];
    rvbl_uword_t arguments[8];
    rvbl_uword_t saved_bottom[10];
    rvbl_uword_t temporaries_bottom[4];
    rvbl_uword_t program_counter;
    rvbl_uword_t cause;
    rvbl_uword_t value;
    rvbl_pointer_t extra;
} rvbl_interrupt_hart_context;

/// == Type `rvbl_interrupt_trap_handler`
///
/// Function type representing a trap handler.
///
/// === Parameters
/// `volatile rvbl_interrupt_hart_context *`:: HART context recorded at the
/// by the global trap handler.
///
/// IMPORTANT: the HART context is stored in the interrupt handling stack before
/// passing control to `rvbl_interrupt_trap_handler`, and restored from there
/// right after `rvbl_interrupt_trap_handler` returns. If the HART context
/// is to be saved or modified, it is the `rvbl_interrupt_trap_handler`'s
/// responsibility to do so.
typedef void (*rvbl_interrupt_trap_handler)(volatile rvbl_interrupt_hart_context *);

/// == Enumeration `rvbl_interrupt_hart_privilege`
///
/// RISC-V HART execution privilege levels.
typedef enum rvbl_interrupt_hart_privilege
{
    /// * `rvbl_interrupt_hart_privilege_u` = `0`
    rvbl_interrupt_hart_privilege_u = rvbl_riscv_hart_privileged_32_mstatus_mpp_values_user,
    /// * `rvbl_interrupt_hart_privilege_s` = `1`
    rvbl_interrupt_hart_privilege_s = rvbl_riscv_hart_privileged_32_mstatus_mpp_values_supervisor,
    /// * `rvbl_interrupt_hart_privilege_m` = `3`
    rvbl_interrupt_hart_privilege_m = rvbl_riscv_hart_privileged_32_mstatus_mpp_values_machine
} rvbl_interrupt_hart_privilege;

/// == Function `rvbl_interrupt_hart_current_initialize_traps`
///
/// Initializes trap-handling structures and registers.
///
/// === Parameters
/// `rvbl_alloc_allocator*`:: Pointer to allocator for internal structures.
/// `rvbl_uword_t`:: Number of platform (i.e. non-standard) interrupts to
///. support.
/// `rvbl_interrupt_trap_handler`:: Pointer to default trap handler.
/// `rvbl_pointer_t`:: Extra data to pass trap handlers via
/// `rvbl_interrupt_hart_context::extra`.
///
/// === Return Value
/// `rvbl_result_t`:: Always `rvbl_result_success`.
rvbl_result_t rvbl_interrupt_hart_current_initialize_traps(
    rvbl_alloc_allocator *, rvbl_uword_t, rvbl_interrupt_trap_handler, rvbl_pointer_t
);

/// == Function `rvbl_interrupt_hart_current_enable_traps`
///
/// Enables all Machine- and (if supported) Supervisor-level traps.
///
/// === Return Value
/// `rvbl_result_t`:: Always `rvbl_result_success`.
rvbl_result_t rvbl_interrupt_hart_current_enable_traps(void);

/// == Function `rvbl_interrupt_hart_current_disable_traps`
///
/// Disables all Machine- and (if supported) Supervisor-level traps.
///
/// === Return Value
/// `rvbl_result_t`:: Always `rvbl_result_success`.
rvbl_result_t rvbl_interrupt_hart_current_disable_traps(void);

/// == Function `rvbl_interrupt_hart_current_set_trap_handler`
///
/// Installs a trap handler for a given exception/interrupt.
///
/// === Parameters
/// `rvbl_interrupt_hart_privilege`:: Execution privilege level at which the
/// trap shall be handled.
/// `rvbl_uword_t`:: Trap identification (use anchor:rvbl_exception[] or
/// anchor:interrupt[] macros).
/// `rvbl_interrupt_trap_handler`:: Trap handler.
///
/// === Return Value
/// `rvbl_result_t`::
/// * `rvbl_result_success` if operation is successful.
/// * `rvbl_result_error_unsupported` if target_privilege value is unsupported.
rvbl_result_t rvbl_interrupt_hart_current_set_trap_handler(
    rvbl_interrupt_hart_privilege, rvbl_uword_t, rvbl_interrupt_trap_handler
);

/// == Function `rvbl_interrupt_hart_current_set_delegation`
///
/// Configure Machine- to Supervisor-level delegation of a given trap.
///
/// === Parameters
/// `rvbl_uword_t`:: Trap identification (use <<rvbl_exception>> or
/// <<rvbl_interrupt>> macros).
/// `rvbl_bool_t`:: Whether to delegate to Supervisor level.
///
/// === Return Value
/// `rvbl_result_t`::
/// * `rvbl_result_success` if operation is successful.
/// * `rvbl_result_error_unsupported` if supervisor mode is unsupported.
rvbl_result_t rvbl_interrupt_hart_current_set_delegation(rvbl_uword_t, rvbl_bool_t);

/// == Macro `RVBL_EXCEPTION`
///
/// [[rvbl_exception]] Formats a `rvbl_uword_t` to hold an exception code.
#define RVBL_EXCEPTION(x) ((rvbl_uword_t)x)

/// == Macro `RVBL_INTERRUPT`
///
/// [[rvbl_interrupt]] Formats a `rvbl_uword_t` to hold an interrupt code.
#define RVBL_INTERRUPT(x) (((rvbl_uword_t)1 << (XLEN - 1)) | (rvbl_uword_t)(x))

#endif
