/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#include "rvbl/concurrency/rvbl_spinlock.h"
#include "rvbl/atomic/rvbl_atomic.h"
#include "rvbl/type/rvbl_types.h"

#define SPINLOCK_FREE ((rvbl_concurrency_spinlock)0)
#define SPINLOCK_BUSY ((rvbl_concurrency_spinlock)1)

void rvbl_concurrency_spinlock_acquire(rvbl_concurrency_spinlock *const spinlock)
{
    while (!rvbl_atomic_compare_and_set(spinlock, SPINLOCK_FREE, SPINLOCK_BUSY)) {
    }
}

rvbl_bool_t rvbl_concurrency_spinlock_try_acquire(
    rvbl_concurrency_spinlock *const spinlock, rvbl_uword_t attempts
)
{
    while ((attempts > 0) && !rvbl_atomic_compare_and_set(spinlock, SPINLOCK_FREE, SPINLOCK_BUSY)) {
        --attempts;
    }

    return (attempts > 0) ? rvbl_true : rvbl_false;
}

void rvbl_concurrency_spinlock_release(rvbl_concurrency_spinlock *const spinlock)
{
    rvbl_atomic_compare_and_set(spinlock, SPINLOCK_BUSY, SPINLOCK_FREE);
}
