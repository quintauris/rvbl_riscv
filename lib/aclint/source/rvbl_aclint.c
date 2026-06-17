/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#include "rvbl/aclint/rvbl_aclint.h"

#include "rvbl/machine/rvbl_aclint.h"
#include "rvbl/type/rvbl_types.h"

rvbl_result_t rvbl_aclint_set_compare(
    const struct rvbl_aclint_t *const instance,
    const rvbl_aclint_hart_t hart,
    const rvbl_aclint_time_t time
)
{
    if ((hart <= RVBL_ACLINT_MAX_HART_ID) && (hart < instance->hart_count_parameter)) {
        rvbl_aclint_mtimecmp_write(instance, hart, time);

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_aclint_get_compare(
    const struct rvbl_aclint_t *const instance,
    const rvbl_aclint_hart_t hart,
    rvbl_aclint_time_t *time
)
{
    if ((hart <= RVBL_ACLINT_MAX_HART_ID) && (hart < instance->hart_count_parameter)) {
        *time = rvbl_aclint_mtimecmp_read(instance, hart);

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_aclint_machine_set_pending(
    const struct rvbl_aclint_t *const instance,
    const rvbl_aclint_hart_t hart,
    const rvbl_bool_t pending
)
{
    if ((hart <= RVBL_ACLINT_MAX_HART_ID) && (hart < instance->hart_count_parameter)) {
        rvbl_aclint_msip_write(instance, hart, pending ? 1 : 0);

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_aclint_supervisor_pending(
    const struct rvbl_aclint_t *const instance,
    const rvbl_aclint_hart_t hart,
    const rvbl_bool_t pending
)
{
    if ((hart <= RVBL_ACLINT_MAX_HART_ID) && (hart < instance->hart_count_parameter)) {
        rvbl_aclint_setssip_write(instance, hart, pending ? 1 : 0);

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}
