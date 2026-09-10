/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#include "rvbl/atomic/rvbl_atomic.h"
#include "rvbl/boot/rvbl_extension.h"
#include "rvbl/machine/rvbl_configuration.h"
#include "rvbl/machine/rvbl_machine.h"
#include "rvbl/test/rvbl_test.h"

const rvbl_uword_t iterations = 256;
volatile rvbl_word_t completion_count = 0;
volatile rvbl_word_t counter = 0;
volatile rvbl_bool_t ready = rvbl_false;
extern void hang(void);

int main(void)
{
    if (RVBL_REGISTER_READ(riscv_hart, privileged, &rvbl_riscv_hart_instance_0, mhartid) == 0) {
        rvbl_test_initialize();
        ready = rvbl_true;
    } else {
        while (!ready) {
        }
    }

    if ((RVBL_CONFIGURATION_HART_COUNT > 1) &&
        rvbl_extension_present(rvbl_riscv_hart_privileged_misa_extensions_values_Atomic)) {
#if !defined(__IAR_SYSTEMS_ICC__)
        if (RVBL_REGISTER_READ(riscv_hart, privileged, &rvbl_riscv_hart_instance_0, mhartid) == 0) {
            while (completion_count < (((rvbl_word_t)RVBL_CONFIGURATION_HART_COUNT) - 1)) {
            }

            ASSERT_EQ(counter, iterations * (RVBL_CONFIGURATION_HART_COUNT - 1));
        } else {
            for (rvbl_uword_t i = 0; i < iterations; ++i) {
                rvbl_atomic_fetch_add(&counter, 1);
            }
            rvbl_atomic_fetch_add(&completion_count, 1);
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
