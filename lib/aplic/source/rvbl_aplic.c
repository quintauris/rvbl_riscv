/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#include "rvbl/aplic/rvbl_aplic.h"

#include "rvbl/compiler/rvbl_compiler.h"
#include "rvbl/machine/rvbl_aplic.h"
#include "rvbl/machine/rvbl_configuration.h"
#include "rvbl/type/rvbl_types.h"

rvbl_result_t rvbl_aplic_set_source_configuration(
    const struct rvbl_aplic_t *const device,
    const rvbl_aplic_interrupt_source_t source,
    const rvbl_aplic_source_configuration_t *const configuration
)
{
    if ((source >= RVBL_APLIC_MIN_INTERRUPT_SOURCE_ID) &&
        (source <= RVBL_APLIC_MAX_INTERRUPT_SOURCE_ID)) {
        if (configuration->delgated) {
            rvbl_aplic_sourcecfg_delegate_set(device, source);
            rvbl_aplic_sourcecfg_child_index_write(device, source, configuration->domain);
        } else {
            rvbl_aplic_sourcecfg_delegate_clear(device, source);
            rvbl_aplic_sourcecfg_source_mode_write(device, source, configuration->source_mode);
        }

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_aplic_get_source_configuration(
    const struct rvbl_aplic_t *const device,
    const rvbl_aplic_interrupt_source_t source,
    rvbl_aplic_source_configuration_t *const configuration
)
{
    if ((source >= RVBL_APLIC_MIN_INTERRUPT_SOURCE_ID) &&
        (source <= RVBL_APLIC_MAX_INTERRUPT_SOURCE_ID)) {
        configuration->delgated = rvbl_aplic_sourcecfg_delegate_read(device, source);

        if (configuration->delgated) {
            configuration->domain = rvbl_aplic_sourcecfg_child_index_read(device, source);
        } else {
            configuration->source_mode = rvbl_aplic_sourcecfg_source_mode_read(device, source);
        }

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_aplic_set_interrupt_pending(
    const struct rvbl_aplic_t *const device, const rvbl_aplic_interrupt_source_t source
)
{
    if ((source >= RVBL_APLIC_MIN_INTERRUPT_SOURCE_ID) &&
        (source <= RVBL_APLIC_MAX_INTERRUPT_SOURCE_ID)) {
        rvbl_aplic_setipnum_write(device, source);

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_aplic_clear_interrupt_pending(
    const struct rvbl_aplic_t *const device, const rvbl_aplic_interrupt_source_t source
)
{
    if ((source >= RVBL_APLIC_MIN_INTERRUPT_SOURCE_ID) &&
        (source <= RVBL_APLIC_MAX_INTERRUPT_SOURCE_ID)) {
        rvbl_aplic_clripnum_write(device, source);

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_aplic_get_interrupt_pending(
    const struct rvbl_aplic_t *const device,
    const rvbl_aplic_interrupt_source_t source,
    rvbl_bool_t *const pending
)
{
    if ((source >= RVBL_APLIC_MIN_INTERRUPT_SOURCE_ID) &&
        (source <= RVBL_APLIC_MAX_INTERRUPT_SOURCE_ID)) {
        const rvbl_uint32_t word = rvbl_aplic_setip_read(device, source / XLEN);
        const rvbl_uint32_t mask = 1U << (source % XLEN);

        *pending = (word & mask) ? rvbl_true : rvbl_false;

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_aplic_get_interrupt_pending_rectified(
    const struct rvbl_aplic_t *const device,
    const rvbl_aplic_interrupt_source_t source,
    rvbl_bool_t *const pending
)
{
    if ((source >= RVBL_APLIC_MIN_INTERRUPT_SOURCE_ID) &&
        (source <= RVBL_APLIC_MAX_INTERRUPT_SOURCE_ID)) {
        const rvbl_uint32_t word = rvbl_aplic_in_clrip_read(device, source / XLEN);
        const rvbl_uint32_t mask = 1U << (source % XLEN);

        *pending = (word & mask) ? rvbl_true : rvbl_false;

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_aplic_set_interrupt_enable(
    const struct rvbl_aplic_t *const device,
    const rvbl_aplic_interrupt_source_t source,
    const rvbl_bool_t enable
)
{
    if ((source >= RVBL_APLIC_MIN_INTERRUPT_SOURCE_ID) &&
        (source <= RVBL_APLIC_MAX_INTERRUPT_SOURCE_ID)) {
        const rvbl_uint32_t word = source / XLEN;
        const rvbl_uint32_t mask = 1 << (source % XLEN);

        if (enable) {
            rvbl_aplic_setie_write(device, word, mask);
        } else {
            rvbl_aplic_clrie_write(device, word, mask);
        }

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_aplic_get_interrupt_enable(
    const struct rvbl_aplic_t *const device,
    const rvbl_aplic_interrupt_source_t source,
    rvbl_bool_t *const enable
)
{
    if ((source >= RVBL_APLIC_MIN_INTERRUPT_SOURCE_ID) &&
        (source <= RVBL_APLIC_MAX_INTERRUPT_SOURCE_ID)) {
        const rvbl_uint32_t word = rvbl_aplic_setie_read(device, source / XLEN);
        const rvbl_uint32_t mask = 1U << (source % XLEN);

        *enable = (word & mask) ? rvbl_true : rvbl_false;

        return rvbl_result_success;
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_aplic_generate(
    const struct rvbl_aplic_t *device, rvbl_aplic_hart_t hart, rvbl_aplic_interrupt_source_t source
)
{
    if ((source >= RVBL_APLIC_MIN_INTERRUPT_SOURCE_ID) &&
        (source <= RVBL_APLIC_MAX_INTERRUPT_SOURCE_ID) && (hart < RVBL_CONFIGURATION_HART_COUNT)) {
        if (!rvbl_aplic_genmsi_busy(device)) {
            rvbl_aplic_genmsi_write(device, source | (hart << 18));

            return rvbl_result_success;
        } else {
            return rvbl_result_error_busy;
        }
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_aplic_set_target_configuration_direct(
    const struct rvbl_aplic_t *device,
    rvbl_aplic_interrupt_source_t source,
    const rvbl_aplic_target_configuration_direct_t *configuration
)
{
    if ((source >= RVBL_APLIC_MIN_INTERRUPT_SOURCE_ID) &&
        (source <= RVBL_APLIC_MAX_INTERRUPT_SOURCE_ID)) {
        switch (rvbl_aplic_domaincfg_dm_read(device)) {
        case aplic_domaincfg_dm_values_direct:
            rvbl_aplic_target_hart_index_write(device, source, configuration->hart);
            rvbl_aplic_target_iprio_write(device, source, configuration->priority);
            return rvbl_result_success;
        default:
            return rvbl_result_error_state;
        }
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_aplic_get_target_configuration_direct(
    const struct rvbl_aplic_t *device,
    rvbl_aplic_interrupt_source_t source,
    rvbl_aplic_target_configuration_direct_t *configuration
)
{
    if ((source >= RVBL_APLIC_MIN_INTERRUPT_SOURCE_ID) &&
        (source <= RVBL_APLIC_MAX_INTERRUPT_SOURCE_ID)) {
        switch (rvbl_aplic_domaincfg_dm_read(device)) {
        case aplic_domaincfg_dm_values_direct:
            configuration->hart = rvbl_aplic_target_hart_index_read(device, source);
            configuration->priority = rvbl_aplic_target_iprio_read(device, source);
            return rvbl_result_success;
        default:
            return rvbl_result_error_state;
        }
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_aplic_set_target_configuration_msi(
    const struct rvbl_aplic_t *device,
    rvbl_aplic_interrupt_source_t source,
    const rvbl_aplic_target_configuration_msi_t *configuration
)
{
    if ((source >= RVBL_APLIC_MIN_INTERRUPT_SOURCE_ID) &&
        (source <= RVBL_APLIC_MAX_INTERRUPT_SOURCE_ID)) {
        switch (rvbl_aplic_domaincfg_dm_read(device)) {
        case aplic_domaincfg_dm_values_msi:
            rvbl_aplic_target_hart_index_write(device, source, configuration->hart);
            rvbl_aplic_target_guest_index_write(device, source, configuration->guest);
            rvbl_aplic_target_eeid_write(device, source, configuration->id);
            return rvbl_result_success;
        default:
            return rvbl_result_error_state;
        }
    } else {
        return rvbl_result_error_bounds;
    }
}

rvbl_result_t rvbl_aplic_get_target_configuration_msi(
    const struct rvbl_aplic_t *device,
    rvbl_aplic_interrupt_source_t source,
    rvbl_aplic_target_configuration_msi_t *configuration
)
{
    if ((source >= RVBL_APLIC_MIN_INTERRUPT_SOURCE_ID) &&
        (source <= RVBL_APLIC_MAX_INTERRUPT_SOURCE_ID)) {
        switch (rvbl_aplic_domaincfg_dm_read(device)) {
        case aplic_domaincfg_dm_values_msi:
            configuration->hart = rvbl_aplic_target_hart_index_read(device, source);
            configuration->guest = rvbl_aplic_target_guest_index_read(device, source);
            configuration->id = rvbl_aplic_target_eeid_read(device, source);
            return rvbl_result_success;
        default:
            return rvbl_result_error_state;
        }
    } else {
        return rvbl_result_error_bounds;
    }
}
