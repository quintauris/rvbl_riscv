/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#include "rvbl/memory/rvbl_pmp.h"
#include "rvbl/test/rvbl_test.h"

static const rvbl_memory_pmp_region regions[] = {
    /* (0) Empty region between 0x00000000 and 0x80000000 */
    /* (1) 4-byte region starting at 0x80000000 */
    {.base = 0x80000000, .size = 4, .access = rvbl_memory_pmp_access_rx, .locked = rvbl_false},
    /* (2) 32-byte region starting at 0x80000004 */
    {.base = 0x80000004, .size = 32, .access = rvbl_memory_pmp_access_rwx, .locked = rvbl_true},
    /* (3) 60-byte region starting at 0x80000024 */
    {.base = 0x80000024, .size = 60, .access = rvbl_memory_pmp_access_x, .locked = rvbl_true},
    /* (4) Empty region between 0x80000060 and 0x90000000 */
    /* (5) 128MB region starting at 0x90000000 */
    {.base = 0x90000000,
     .size = 134217728 /* 128M */,
     .access = rvbl_memory_pmp_access_rw,
     .locked = rvbl_false}
};

typedef enum rvbl_memory_pmp_configuration_address
{
    rvbl_memory_pmp_configuration_address_null = 0x00,
    rvbl_memory_pmp_configuration_address_tor = 0x08,
    rvbl_memory_pmp_configuration_address_na4 = 0x10,
    rvbl_memory_pmp_configuration_address_napot = 0x18
} rvbl_memory_pmp_configuration_address;

typedef enum rvbl_memory_pmp_configuration_locking
{
    rvbl_memory_pmp_configuration_locking_unlocked = 0x00,
    rvbl_memory_pmp_configuration_locking_locked = 0x80
} rvbl_memory_pmp_configuration_locking;

int main(void)
{
    rvbl_memory_pmp_region_configuration_register configurations[8];
    rvbl_memory_pmp_region_address_register addresses[8];

    rvbl_hart_hang_if_not(0);
    rvbl_test_initialize();

    ASSERT_EQ(
        rvbl_result_success, rvbl_memory_pmp_generate(regions, 4, configurations, addresses, 8)
    );

    /* (0) Empty region between 0x00000000 and 0x80000000 */
    ASSERT_EQ(
        configurations[0],
        rvbl_memory_pmp_access_none | rvbl_memory_pmp_configuration_address_null |
            rvbl_memory_pmp_configuration_locking_unlocked
    );
    ASSERT_EQ(addresses[0], (regions[0].base >> 2));

    /* (1) 4-byte region starting at 0x80000000 */
    ASSERT_EQ(
        configurations[1],
        rvbl_memory_pmp_access_rx | rvbl_memory_pmp_configuration_address_na4 |
            rvbl_memory_pmp_configuration_locking_unlocked
    );
    ASSERT_EQ(addresses[1], (regions[0].base >> 2));

    /* (2) 32-byte region starting at 0x80000004 */
    ASSERT_EQ(
        configurations[2],
        rvbl_memory_pmp_access_rwx | rvbl_memory_pmp_configuration_address_napot |
            rvbl_memory_pmp_configuration_locking_locked
    );
    ASSERT_EQ(addresses[2], (regions[1].base >> 2) | 0x3 /* 32-byte NAPOT range */);

    /* (3) 60-byte region starting at 0x80000024 */
    ASSERT_EQ(
        configurations[3],
        rvbl_memory_pmp_access_x | rvbl_memory_pmp_configuration_address_tor |
            rvbl_memory_pmp_configuration_locking_locked
    );
    ASSERT_EQ(addresses[3], ((regions[2].base + regions[2].size) >> 2));

    /* (4) Empty region between 0x80000060 and 0x90000000 */
    ASSERT_EQ(
        configurations[4],
        rvbl_memory_pmp_access_none | rvbl_memory_pmp_configuration_address_null |
            rvbl_memory_pmp_configuration_locking_unlocked
    );
    ASSERT_EQ(addresses[4], (regions[3].base >> 2));

    /* (5) 128MB region starting at 0x90000000 */
    ASSERT_EQ(
        configurations[5],
        rvbl_memory_pmp_access_rw | rvbl_memory_pmp_configuration_address_napot |
            rvbl_memory_pmp_configuration_locking_unlocked
    );
    ASSERT_EQ(addresses[5], (regions[3].base >> 2) | 0xFFFFFF);

    PASS();

    return 0;
}
