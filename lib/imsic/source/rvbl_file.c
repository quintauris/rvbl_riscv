/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#include "rvbl/imsic/rvbl_file.h"

#include "rvbl/imsic/rvbl_common.h"
#include "rvbl/machine/rvbl_configuration.h"
#include "rvbl/machine/rvbl_imsic.h"
#include "rvbl/type/rvbl_types.h"

struct interrupt_file
{
    rvbl_int32_t seteipnum_le;
    rvbl_int32_t seteipnum_be;
};

rvbl_result_t rvbl_imsic_file_set_interrupt_pending(
    const struct rvbl_imsic_t *device,
    const rvbl_imsic_hart_t hart,
    const rvbl_imsic_hart_mode_t mode,
    const rvbl_imsic_interrupt_source_t source
)
{
    if (hart < RVBL_CONFIGURATION_HART_COUNT) {
        switch (mode) {
        case rvbl_imsic_mode_machine:
            rvbl_imsic_mseteipnum_le_write(device, hart, source);
            break;
        case rvbl_imsic_mode_supervisor:
            rvbl_imsic_sseteipnum_le_write(device, hart, source);
            break;
        default:
            return rvbl_result_error_bounds;
        }

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}
