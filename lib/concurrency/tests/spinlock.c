/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#include "rvbl/atomic/rvbl_atomic.h"
#include "rvbl/boot/rvbl_extension.h"
#include "rvbl/concurrency/rvbl_spinlock.h"
#include "rvbl/machine/rvbl_configuration.h"
#include "rvbl/test/rvbl_test.h"

rvbl_concurrency_spinlock lock = RVBL_CONCURRENCY_SPINLOCK_INITIALIZER;
rvbl_concurrency_spinlock log_lock = RVBL_CONCURRENCY_SPINLOCK_INITIALIZER;
const rvbl_uword_t iterations = 1024, workers = RVBL_CONFIGURATION_HART_COUNT - 1;
volatile rvbl_word_t finished_count = 0, success_count = 0, error_count = 0;
volatile rvbl_word_t counter = 0;
volatile rvbl_bool_t ready = rvbl_false;

int main(void)
{
    if (RVBL_REGISTER_READ(riscv_hart, privileged, &rvbl_riscv_hart_instance_0, mhartid) == 0) {
        rvbl_test_initialize();
        ready = rvbl_true;
    } else {
        while (!ready) {
        }
    }

    if ((workers >= 1) &&
        rvbl_extension_present(rvbl_riscv_hart_privileged_misa_extensions_values_Atomic)) {
#if !defined(__IAR_SYSTEMS_ICC__)
        if (RVBL_REGISTER_READ(riscv_hart, privileged, &rvbl_riscv_hart_instance_0, mhartid) == 0) {
            rvbl_word_t timeout = RVBL_CONFIGURATION_MTIME_FREQUENCY;

            while ((finished_count < ((rvbl_word_t)workers)) && (timeout != 0)) {
                --timeout;
            }

            rvbl_concurrency_spinlock_acquire(&lock);
            ASSERT_EQ(success_count, workers);
            ASSERT_EQ(error_count, 0);
            ASSERT_EQ(counter, iterations * workers);
            rvbl_concurrency_spinlock_release(&lock);
        } else {
            rvbl_concurrency_spinlock_acquire(&lock);

            for (rvbl_uword_t i = 0; i < iterations; ++i) {
                ++counter;
            }

            rvbl_atomic_fetch_add(&success_count, 1);
            rvbl_atomic_fetch_add(&finished_count, 1);

            rvbl_concurrency_spinlock_release(&lock);
            rvbl_hang();
        }
        PASS();
#else
        SKIP();
#endif
    } else {
        SKIP();
    }

    return 0;
}
