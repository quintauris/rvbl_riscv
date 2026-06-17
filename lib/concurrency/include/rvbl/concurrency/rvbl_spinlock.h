/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef RVBL_SPINLOCK_H
#define RVBL_SPINLOCK_H

/// = RISC-V Base Layer Concurrency Library
/// Quintauris GmbH
/// :toc: left
///
/// Implements basic concurrency concepts.

#include "rvbl/type/rvbl_types.h"

/// == Type `rvbl_concurrency_spinlock`
/// Spinlock controlling access to a critical region.
typedef volatile rvbl_uword_t rvbl_concurrency_spinlock;

/// == Constant `RVBL_CONCURRENCY_SPINLOCK_INITIALIZER`
/// Safe initializer value for Spinlock-typed variables.
#define RVBL_CONCURRENCY_SPINLOCK_INITIALIZER ((rvbl_concurrency_spinlock)0)

/// == Function `rvbl_concurrency_spinlock_acquire`
///
/// Unconditionally blocks (spins) until the lock is acquired.
///
/// === Parameters
/// `rvbl_concurrency_spinlock*`:: Address of spinlock to acquire.
void rvbl_concurrency_spinlock_acquire(rvbl_concurrency_spinlock *);

/// == Function `rvbl_concurrency_spinlock_try_acquire`
///
/// Conditionally blocks (spins) until the lock is acquired.
///
/// === Parameters
/// `rvbl_concurrency_spinlock*`:: Address of spinlock to acquire.
/// `rvbl_uword_t`:: Number of attempts to acquire the lock before giving up.
///
/// === Return value
/// `rvbl_bool_t`:: Whether the lock has been successfully acquired.
rvbl_bool_t rvbl_concurrency_spinlock_try_acquire(rvbl_concurrency_spinlock *, rvbl_uword_t);

/// == Function `rvbl_concurrency_spinlock_release`
///
/// Releases a lock, potentially unblocking another thread blocked on it.
///
/// === Parameters
/// `rvbl_concurrency_spinlock*`:: Address of spinlock to release.
void rvbl_concurrency_spinlock_release(rvbl_concurrency_spinlock *);

#endif
