/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

/// = RISC-V Base Layer Memory library
/// Quintauris GmbH
/// :toc: left
///
/// Implements memory management functions.

#ifndef RVBL_CACHE_H
#define RVBL_CACHE_H

#include "rvbl/compiler/rvbl_compiler.h"
#include "rvbl/type/rvbl_types.h"

/// == Function `rvbl_memory_cache_clean`
/// Performs a clean operation on a cache block.
///
/// === Parameters
/// `rvbl_pointer_t*`:: Effective address of cache block to operate on.
RVBL_INLINE() void rvbl_memory_cache_clean(const rvbl_pointer_t address)
{
    __asm__ volatile("cbo.clean (%0)" : : "r"(address));
}

/// == Function `rvbl_memory_cache_flush`
/// Performs a flush operation on a cache block.
///
/// === Parameters
/// `rvbl_pointer_t*`:: Effective address of cache block to operate on.
RVBL_INLINE() void rvbl_memory_cache_flush(const rvbl_pointer_t address)
{
    __asm__ volatile("cbo.flush (%0)" : : "r"(address));
}

/// == Function `rvbl_memory_cache_clean`
/// Performs an invalidate operation on a cache block.
///
/// === Parameters
/// `rvbl_pointer_t*`:: Effective address of cache block to operate on.
RVBL_INLINE() void rvbl_memory_cache_invalidate(const rvbl_pointer_t address)
{
    __asm__ volatile("cbo.inval (%0)" : : "r"(address));
}

/// == Function `rvbl_memory_cache_clean`
/// Performs a zero operation on a cache block.
///
/// === Parameters
/// `rvbl_pointer_t*`:: Effective address of cache block to operate on.
RVBL_INLINE() void rvbl_memory_cache_zero(const rvbl_pointer_t address)
{
    __asm__ volatile("cbo.zero (%0)" : : "r"(address));
}

/// == Function `rvbl_memory_cache_prefetch_instruction`
/// Hints the hardware of an upcoming instruction fetch.
///
/// === Parameters
/// `rvbl_pointer_t*`:: Address to hint for prefetching.
RVBL_INLINE() void rvbl_memory_cache_prefetch_instruction(const rvbl_pointer_t address)
{
    __asm__ volatile("prefetch.i 0(%0)" : : "r"(address));
}

/// == Function `rvbl_memory_cache_prefetch_read`
/// Hints the hardware of an upcoming data read.
///
/// === Parameters
/// `rvbl_pointer_t*`:: Address to hint for prefetching.
RVBL_INLINE() void rvbl_memory_cache_prefetch_read(const rvbl_pointer_t address)
{
    __asm__ volatile("prefetch.r 0(%0)" : : "r"(address));
}

/// == Function `rvbl_memory_cache_prefetch_write`
/// Hints the hardware of an upcoming data write.
///
/// === Parameters
/// `rvbl_pointer_t*`:: Address to hint for prefetching.
RVBL_INLINE() void rvbl_memory_cache_prefetch_write(const rvbl_pointer_t address)
{
    __asm__ volatile("prefetch.w 0(%0)" : : "r"(address));
}

#endif
