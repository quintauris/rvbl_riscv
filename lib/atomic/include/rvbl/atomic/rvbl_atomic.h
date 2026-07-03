/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef RVBL_ATOMIC_H
#define RVBL_ATOMIC_H

/// = RISC-V Base Layer Atomics Library
/// Quintauris GmbH
/// :toc: left
///
/// Implements RISC-V Atomic operations (LR/SC, addition, bitwise, max/min).

#include "rvbl/type/rvbl_types.h"

/// == Function `rvbl_atomic_load_reserved`
/// Atomically loads value from given address, putting a reservation on it.
///
/// === Parameters
/// `rvbl_uword_t*`:: Address to load/reserve from.
///
/// === Return value
/// `rvbl_uword_t`:: Current value on given address.
RVBL_INLINE() rvbl_uword_t rvbl_atomic_load_reserved(volatile rvbl_uword_t *const address)
{
    rvbl_uword_t result;

    __asm__ volatile("lr.w %0, 0(%1)\n\t" : "=r"(result) : "r"(address) :);

    return result;
}

/// == Function `rvbl_atomic_store_conditional`
/// Atomically stores value in given address, if reservation from a previous load
/// is kept.
///
/// === Parameters
/// `rvbl_uword_t*`:: Address to store value into.
/// `rvbl_uword_t`:: Value sto store into.
///
/// === Return value
/// `rvbl_bool_t`:: Whether store operation has been successful.
RVBL_INLINE() rvbl_bool_t
rvbl_atomic_store_conditional(volatile rvbl_uword_t *const address, const rvbl_uword_t value)
{
    rvbl_uword_t result;

    __asm__ volatile("sc.w %0, %1, 0(%2)\n\t" : "=r"(result) : "r"(value), "r"(address) :);

    return (result == 0) ? rvbl_true : rvbl_false;
}

/// == Function `rvbl_atomic_compare_and_swap`
/// Atomically compares value in given address, then sets a new value if equals.
///
/// === Parameters
/// `rvbl_uword_t*`:: Address to compare/store value.
/// `rvbl_uword_t`:: Expected value (to compare).
/// `rvbl_uword_t`:: Desired value (to store).
///
/// === Return value
/// `rvbl_bool_t`:: Whether compare/store operation has been successful.
rvbl_bool_t rvbl_atomic_compare_and_set(
    volatile rvbl_uword_t *const address, const rvbl_uword_t expected, const rvbl_uword_t desired
);

#define RVBL_ATOMIC_OPERATION(type, name, opcode)                                                  \
    RVBL_INLINE() type rvbl_atomic_##name(volatile type *const address, const type value)          \
    {                                                                                              \
        type result;                                                                               \
                                                                                                   \
        __asm__ volatile(#opcode " %0, %1, (%2)\n\t" : "=r"(result) : "r"(value), "r"(address) :); \
                                                                                                   \
        return result;                                                                             \
    }

/// == Function `rvbl_atomic_fetch_add`
/// Atomically reads/operates/stores value in given address.
///
/// === Parameters
/// `rvbl_uword_t*`:: Address of first input value and destination.
/// `rvbl_uword_t`:: Second input value.
///
/// === Return value
/// `rvbl_word_t`:: Value before operation.
RVBL_ATOMIC_OPERATION(rvbl_word_t, fetch_add, amoadd.w)

/// == Function `rvbl_atomic_fetch_and`
/// Atomically reads/operates/stores value in given address.
///
/// === Parameters
/// `rvbl_uword_t*`:: Address of first input value and destination.
/// `rvbl_uword_t`:: Second input value.
///
/// === Return value
/// `rvbl_word_t`:: Value before operation.
RVBL_ATOMIC_OPERATION(rvbl_uword_t, fetch_and, amoand.w)

/// == Function `rvbl_atomic_fetch_or`
/// Atomically reads/operates/stores value in given address.
///
/// === Parameters
/// `rvbl_uword_t*`:: Address of first input value and destination.
/// `rvbl_uword_t`:: Second input value.
///
/// === Return value
/// `rvbl_word_t`:: Value before operation.
RVBL_ATOMIC_OPERATION(rvbl_uword_t, fetch_or, amoor.w)

/// == Function `rvbl_atomic_fetch_xor`
/// Atomically reads/operates/stores value in given address.
///
/// === Parameters
/// `rvbl_uword_t*`:: Address of first input value and destination.
/// `rvbl_uword_t`:: Second input value.
///
/// === Return value
/// `rvbl_word_t`:: Value before operation.
RVBL_ATOMIC_OPERATION(rvbl_uword_t, fetch_xor, amoxor.w)

/// == Function `rvbl_atomic_fetch_min`
/// Atomically reads/operates/stores value in given address.
///
/// === Parameters
/// `rvbl_uword_t*`:: Address of first input value and destination.
/// `rvbl_uword_t`:: Second input value.
///
/// === Return value
/// `rvbl_word_t`:: Value before operation.
RVBL_ATOMIC_OPERATION(rvbl_word_t, fetch_min, amomin.w)

/// == Function `rvbl_atomic_fetch_max`
/// Atomically reads/operates/stores value in given address.
///
/// === Parameters
/// `rvbl_uword_t*`:: Address of first input value and destination.
/// `rvbl_uword_t`:: Second input value.
///
/// === Return value
/// `rvbl_word_t`:: Value before operation.
RVBL_ATOMIC_OPERATION(rvbl_word_t, fetch_max, amomax.w)

/// == Function `rvbl_atomic_fetch_min_unsigned`
/// Atomically reads/operates/stores value in given address.
///
/// === Parameters
/// `rvbl_uword_t*`:: Address of first input value and destination.
/// `rvbl_uword_t`:: Second input value.
///
/// === Return value
/// `rvbl_word_t`:: Value before operation.
RVBL_ATOMIC_OPERATION(rvbl_uword_t, fetch_min_unsigned, amominu.w)

/// == Function `rvbl_atomic_fetch_max_unsigned`
/// Atomically reads/operates/stores value in given address.
///
/// === Parameters
/// `rvbl_uword_t*`:: Address of first input value and destination.
/// `rvbl_uword_t`:: Second input value.
///
/// === Return value
/// `rvbl_word_t`:: Value before operation.
RVBL_ATOMIC_OPERATION(rvbl_uword_t, fetch_max_unsigned, amominu.w)

#endif
