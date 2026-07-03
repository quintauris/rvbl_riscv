/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#include "rvbl/machine/rvbl_configuration.h"
#include "rvbl/memory/rvbl_cache.h"
#include "rvbl/test/rvbl_test.h"
#include "rvbl/type/rvbl_types.h"

int main(void)
{
    rvbl_hart_hang_if_not(0);
    rvbl_test_initialize();

    if (RVBL_CONFIGURATION_CACHE_INSTRUCTIONS_SUPPORTED) {
#if !defined(__IAR_SYSTEMS_ICC__)
        rvbl_uword_t dummy = 0;

        rvbl_memory_cache_clean(&dummy);
        rvbl_memory_cache_flush(&dummy);
        rvbl_memory_cache_invalidate(&dummy);
        rvbl_memory_cache_zero(&dummy);
        rvbl_memory_cache_prefetch_instruction((void *)main);
        rvbl_memory_cache_prefetch_read(&dummy);
        rvbl_memory_cache_prefetch_write(&dummy);
        PASS();
#else
        SKIP();
#endif
    } else {
        SKIP();
    }

    return 0;
}
