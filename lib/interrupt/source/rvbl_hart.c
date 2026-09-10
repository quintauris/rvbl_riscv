/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#include "rvbl/interrupt/rvbl_hart.h"

#include "rvbl/abi/rvbl_abi.h"
#include "rvbl/alloc/rvbl_alloc.h"
#include "rvbl/boot/rvbl_extension.h"
#include "rvbl/compiler/rvbl_compiler.h"
#include "rvbl/hardware/rvbl_hardware.h"
#include "rvbl/type/rvbl_types.h"

typedef struct hart_jump_tables
{
    rvbl_interrupt_trap_handler *machine_interrupt_jump_table;
    rvbl_interrupt_trap_handler *machine_exception_jump_table;

    rvbl_interrupt_trap_handler *supervisor_interrupt_jump_table;
    rvbl_interrupt_trap_handler *supervisor_exception_jump_table;

    rvbl_interrupt_trap_handler *machine_jump_tables[2];
    rvbl_interrupt_trap_handler *supervisor_jump_tables[2];
} hart_jump_tables;

extern rvbl_uint32_t machine_trap_table;
extern rvbl_uint32_t supervisor_trap_table;

static rvbl_bool_t supervisor_extension_enabled;
static rvbl_pointer_t extra;

static void nop(volatile rvbl_interrupt_hart_context *context) { (void)context; }

rvbl_interrupt_trap_handler *rvbl_interrupt_hart_internal_select_trap_handler(
    const rvbl_interrupt_hart_privilege privilege,
    rvbl_uword_t interrupt_code,
    rvbl_interrupt_hart_context *context
)
{
    const rvbl_uint32_t interrupt_mask = 0x80000000, code_mask = 0x0000003F;
    hart_jump_tables *hart = rvbl_thread_pointer_register_read();
    rvbl_interrupt_trap_handler **jump_tables;
    rvbl_interrupt_trap_handler *jump_table;

    switch (privilege) {
    default:
    case rvbl_interrupt_hart_privilege_u:
        return NULL;
    case rvbl_interrupt_hart_privilege_s:
        if (supervisor_extension_enabled) {
            jump_tables = hart->supervisor_jump_tables;
        } else {
            return NULL;
        }
        break;
    case rvbl_interrupt_hart_privilege_m:
        jump_tables = hart->machine_jump_tables;
        break;
    }

    if (interrupt_code & interrupt_mask) {
        jump_table = jump_tables[0];
    } else {
        jump_table = jump_tables[1];
    }

    if (context != NULL) {
        context->extra = extra;
    }

    interrupt_code = interrupt_code & code_mask;

    return &jump_table[interrupt_code];
}

rvbl_result_t rvbl_interrupt_hart_current_initialize_traps(
    rvbl_alloc_allocator *allocator,
    const rvbl_uword_t platform_interrupts,
    rvbl_interrupt_trap_handler default_handler,
    const rvbl_pointer_t extra_
)
{
    const rvbl_uint32_t max_exceptions = 64, standard_interrupts = 16;
    hart_jump_tables *hart = rvbl_alloc(allocator, sizeof(hart_jump_tables));
    rvbl_uint32_t i;

    extra = extra_;
    supervisor_extension_enabled =
        rvbl_extension_present(rvbl_riscv_hart_privileged_misa_extensions_values_SupervisorMode);
    RVBL_REGISTER_FIELD_WRITE(
        riscv_hart,
        privileged,
        &rvbl_riscv_hart_instance_0,
        mtvec,
        base,
        ((rvbl_uword_t)&machine_trap_table) >> 2
    );

    if (supervisor_extension_enabled) {
        RVBL_REGISTER_WRITE(riscv_hart, privileged, &rvbl_riscv_hart_instance_0, mideleg, 0);
        RVBL_REGISTER_WRITE(riscv_hart, privileged_32, &rvbl_riscv_hart_instance_0, medeleg, 0);

#if defined(RVBL_CONFIGURATION_HAS_MODE_SUPERVISOR)
        RVBL_REGISTER_FIELD_WRITE(
            riscv_hart,
            privileged,
            &rvbl_riscv_hart_instance_0,
            stvec,
            base,
            ((rvbl_uword_t)&supervisor_trap_table) >> 2
        );
#endif
    }

    if (default_handler == NULL) {
        default_handler = nop;
    }

    hart->machine_interrupt_jump_table = rvbl_alloc(
        allocator, (standard_interrupts + platform_interrupts) * sizeof(rvbl_interrupt_trap_handler)
    );
    hart->machine_exception_jump_table =
        rvbl_alloc(allocator, max_exceptions * sizeof(rvbl_interrupt_trap_handler));

    hart->supervisor_interrupt_jump_table = rvbl_alloc(
        allocator, (standard_interrupts + platform_interrupts) * sizeof(rvbl_interrupt_trap_handler)
    );
    hart->supervisor_exception_jump_table =
        rvbl_alloc(allocator, max_exceptions * sizeof(rvbl_interrupt_trap_handler));

    hart->machine_jump_tables[0] = hart->machine_interrupt_jump_table;
    hart->machine_jump_tables[1] = hart->machine_exception_jump_table;

    hart->supervisor_jump_tables[0] = hart->supervisor_interrupt_jump_table;
    hart->supervisor_jump_tables[1] = hart->supervisor_exception_jump_table;

    for (i = 0; i < (standard_interrupts + platform_interrupts); ++i) {
        hart->machine_interrupt_jump_table[i] = default_handler;
        hart->supervisor_interrupt_jump_table[i] = default_handler;
    }

    for (i = 0; i < max_exceptions; ++i) {
        hart->machine_exception_jump_table[i] = default_handler;
        hart->supervisor_exception_jump_table[i] = default_handler;
    }

    rvbl_thread_pointer_register_write(hart);

    return rvbl_result_success;
}

rvbl_result_t rvbl_interrupt_hart_current_enable_traps(void)
{
    RVBL_REGISTER_FIELD_SET(riscv_hart, privileged_32, &rvbl_riscv_hart_instance_0, mstatus, mie);

    if (supervisor_extension_enabled) {
        RVBL_REGISTER_FIELD_SET(
            riscv_hart, privileged_32, &rvbl_riscv_hart_instance_0, mstatus, sie
        );
    }

    return rvbl_result_success;
}

rvbl_result_t rvbl_interrupt_hart_current_disable_traps(void)
{
    RVBL_REGISTER_FIELD_CLEAR(riscv_hart, privileged_32, &rvbl_riscv_hart_instance_0, mstatus, mie);

    if (supervisor_extension_enabled) {
        RVBL_REGISTER_FIELD_CLEAR(
            riscv_hart, privileged_32, &rvbl_riscv_hart_instance_0, mstatus, sie
        );
    }

    return rvbl_result_success;
}

rvbl_result_t rvbl_interrupt_hart_current_set_trap_handler(
    const rvbl_interrupt_hart_privilege privilege,
    rvbl_uword_t interrupt_code,
    const rvbl_interrupt_trap_handler handler
)
{
    rvbl_interrupt_trap_handler *target =
        rvbl_interrupt_hart_internal_select_trap_handler(privilege, interrupt_code, NULL);

    if (target == NULL) {
        return rvbl_result_error_unsupported;
    } else {
        *target = handler;
        return rvbl_result_success;
    }
}

rvbl_result_t
rvbl_interrupt_hart_current_set_delegation(rvbl_uword_t interrupt_code, const rvbl_bool_t delegate)
{
    if (supervisor_extension_enabled) {
        const rvbl_uint32_t mask = 0x80000000;

        if (interrupt_code & mask) {
            rvbl_riscv_hart_privileged_mideleg_t delegation =
                RVBL_REGISTER_READ(riscv_hart, privileged, &rvbl_riscv_hart_instance_0, mideleg);

            interrupt_code &= ~mask;

            if (delegate) {
                delegation |= (1 << interrupt_code);
            } else {
                delegation &= ~(1 << interrupt_code);
            }

            RVBL_REGISTER_WRITE(
                riscv_hart, privileged, &rvbl_riscv_hart_instance_0, mideleg, delegation
            );
        } else {
            rvbl_riscv_hart_privileged_32_medeleg_t delegation =
                RVBL_REGISTER_READ(riscv_hart, privileged_32, &rvbl_riscv_hart_instance_0, medeleg);

            if (delegate) {
                delegation |= (1 << interrupt_code);
            } else {
                delegation &= ~(1 << interrupt_code);
            }

            RVBL_REGISTER_WRITE(
                riscv_hart, privileged_32, &rvbl_riscv_hart_instance_0, medeleg, delegation
            );
        }

        return rvbl_result_success;
    } else {
        return rvbl_result_error_unsupported;
    }
}
