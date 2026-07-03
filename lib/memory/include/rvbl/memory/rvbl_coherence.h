/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef RVBL_COHERENCE_H
#define RVBL_COHERENCE_H

#include "rvbl/compiler/rvbl_compiler.h"

/// == Function `rvbl_memory_coherence_instruction_fence`
/// Instruct the HART to synchronize instruction writes and fetches..
RVBL_INLINE() void rvbl_memory_coherence_instruction_fence(void) { __asm__ volatile("fence.i"); }

#endif
