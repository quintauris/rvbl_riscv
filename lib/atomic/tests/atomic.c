/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#include "rvbl/atomic/rvbl_atomic.h"
#include "rvbl/boot/rvbl_extension.h"
#include "rvbl/test/rvbl_test.h"

rvbl_bool_t is_bus_atomic_compatible(void)
{
    const rvbl_uint32_t PRORIETARY_SINGLECORE_RHX = 0x80000402;
    const rvbl_uint32_t ARCV_RHX_1_0 = 0x00020100;

    if ((rvbl_marchid_read() == PRORIETARY_SINGLECORE_RHX) &&
        (rvbl_mimpid_read() == ARCV_RHX_1_0)) {
        return rvbl_false;
    } else {
        return rvbl_true;
    }
}

int main(void)
{
    rvbl_hart_hang_if_not(0);
    rvbl_test_initialize();

    if (rvbl_extension_present(misa_extensions_values_Atomic) && is_bus_atomic_compatible()) {
#if !defined(__IAR_SYSTEMS_ICC__)
        volatile rvbl_word_t a_signed = 0xDEADFACE, b_signed = 0xFACEBEEF, c_signed;
        volatile rvbl_uword_t a_unsigned = 0xDEADFACE, b_unsigned = 0xFACEBEEF, c_unsigned;

        c_signed = a_signed;
        ASSERT_EQ(rvbl_atomic_fetch_add(&c_signed, b_signed), a_signed);
        ASSERT_EQ(c_signed, a_signed + b_signed);

        c_unsigned = a_unsigned;
        ASSERT_EQ(rvbl_atomic_fetch_and(&c_unsigned, b_unsigned), a_unsigned);
        ASSERT_EQ(c_unsigned, a_unsigned & b_unsigned);

        c_unsigned = a_unsigned;
        ASSERT_EQ(rvbl_atomic_fetch_or(&c_unsigned, b_unsigned), a_unsigned);
        ASSERT_EQ(c_unsigned, a_unsigned | b_unsigned);

        c_unsigned = a_unsigned;
        ASSERT_EQ(rvbl_atomic_fetch_xor(&c_unsigned, b_unsigned), a_unsigned);
        ASSERT_EQ(c_unsigned, a_unsigned ^ b_unsigned);

        c_signed = a_signed;
        ASSERT_EQ(rvbl_atomic_fetch_min(&c_signed, b_signed), a_signed);
        ASSERT_EQ(c_signed, a_signed);

        c_signed = a_signed;
        ASSERT_EQ(rvbl_atomic_fetch_max(&c_signed, b_signed), a_signed);
        ASSERT_EQ(c_signed, b_signed);

        c_unsigned = a_unsigned;
        ASSERT_EQ(rvbl_atomic_fetch_min_unsigned(&c_unsigned, b_unsigned), a_unsigned);
        ASSERT_EQ(c_unsigned, a_unsigned);

        c_unsigned = a_unsigned;
        ASSERT_EQ(rvbl_atomic_fetch_max_unsigned(&c_unsigned, b_unsigned), a_unsigned);
        ASSERT_EQ(c_unsigned, a_unsigned);

        PASS();
#else
        SKIP();
#endif
    } else {
        SKIP();
    }

    return 0;
}
