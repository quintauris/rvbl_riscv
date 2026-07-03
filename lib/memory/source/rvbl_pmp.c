/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#include "rvbl/memory/rvbl_pmp.h"

#define PMP_REGION_COUNT 64

typedef enum rvbl_memory_pmp_configuration_address
{
    rvbl_memory_pmp_configuration_address_null = 0x00,
    rvbl_memory_pmp_configuration_address_tor = 0x08,
    rvbl_memory_pmp_configuration_address_na4 = 0x10,
    rvbl_memory_pmp_configuration_address_napot = 0x18
} rvbl_memory_pmp_configuration_address;

static rvbl_bool_t
contiguous_regions(const rvbl_memory_pmp_region *const a, const rvbl_memory_pmp_region *const b)
{
    return ((a->base + a->size) == b->base) ? rvbl_true : rvbl_false;
}

static rvbl_bool_t power_of_two(const rvbl_uword_t x)
{
    return ((x & (x - 1)) == 0) ? rvbl_true : rvbl_false;
}

rvbl_result_t rvbl_memory_pmp_generate(
    const rvbl_memory_pmp_region *const ranges,
    const rvbl_uword_t region_count,
    rvbl_memory_pmp_region_configuration_register *const configurations,
    rvbl_memory_pmp_region_address_register *const addresses,
    const rvbl_uword_t register_count
)
{
    if ((region_count < PMP_REGION_COUNT) && (register_count >= region_count) &&
        ((register_count % 4) == 0)) {
        const rvbl_memory_pmp_region top = {
            .base = 0x0, .size = 0, .access = rvbl_memory_pmp_access_none, .locked = rvbl_true
        };
        const rvbl_memory_pmp_region *current = ranges, *previous = &top;
        rvbl_uword_t i = 0;

        for (i = 0; (((rvbl_uword_t)(current - ranges)) < region_count) && (i < register_count);
             ++i, previous = current, ++current) {
            rvbl_memory_pmp_region_configuration_register configuration = 0;
            rvbl_memory_pmp_region_address_register address = 0;

            if (!contiguous_regions(previous, current)) {
                configurations[i] = 0;
                addresses[i] = current->base >> 2;
                ++i;
            }

            if (i < register_count) {
                configuration |= (rvbl_uword_t)current->access;

                if (power_of_two(current->size)) {
                    rvbl_uword_t mask = 0x0FFFFFFF, size = 0x80000000;

                    if (current->size == 4) {
                        configuration |= ((rvbl_uword_t)rvbl_memory_pmp_configuration_address_na4);
                        mask = 0;
                    } else {
                        configuration |=
                            ((rvbl_uword_t)rvbl_memory_pmp_configuration_address_napot);

                        while (size != current->size) {
                            size >>= 1;
                            mask >>= 1;
                        }
                    }

                    address = ((current->base) >> 2) | mask;
                } else {
                    configuration |= ((rvbl_uword_t)rvbl_memory_pmp_configuration_address_tor);
                    address = ((current->base + current->size) >> 2);
                }

                if (current->locked) {
                    configuration |= (1 << 7);
                }

                configurations[i] = configuration;
                addresses[i] = address;
            }
        }

        while (i < register_count) {
            configurations[i] = 0;
            addresses[i] = 0;
            ++i;
        }

        return (current == (ranges + region_count)) ? rvbl_result_success
                                                    : rvbl_result_error_bounds;
    } else {
        return rvbl_result_error_bounds;
    }
}
