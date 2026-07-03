/*
 * Copyright 2026 Quintauris GmbH
 * Licensed under the Apache License, Version 2.0 (the "License").
 * https://www.apache.org/licenses/LICENSE-2.0
 */

#include "rvbl/aplic/rvbl_aplic.h"
#include "rvbl/imsic/rvbl_hart.h"
#include "rvbl/interrupt/rvbl_hart.h"
#include "rvbl/machine/rvbl_aplic.h"
#include "rvbl/machine/rvbl_machine.h"
#include "rvbl/test/rvbl_test.h"
#include "rvbl/type/rvbl_types.h"

#if 0
static const rvbl_imsic_interrupt_source_t kEEID = 5, kIrq = 5;
static volatile rvbl_uint32_t count = 0;

void machine_irq_handler(volatile rvbl_interrupt_hart_context *context)
{
    rvbl_imsic_interrupt_source_t source;

    (void)context;

    rvbl_imsic_hart_get_top_interrupt(rvbl_imsic_mode_machine, &source);
    source &= 0x7FF;
    rvbl_imsic_hart_set_interrupt_pending(rvbl_imsic_mode_machine, source, rvbl_false);
    ASSERT_EQ(source, kIrq);
    ++count;
}
#endif

int main(void)
{
#if 0
    const rvbl_uint32_t PRORIETARY_SINGLECORE_RHX = 0x80000402;
    const rvbl_uint32_t ARCV_RHX_1_0 = 0x00020100;
#endif

    rvbl_hart_hang_if_not(0);
    rvbl_test_initialize();

    // This test works on RT-Europa Cl 2 (Synopsys VDK model) but not on other
    // machines like RT-Europa Cl 1, because those don't have a "test register"
    // at 0x00044000 that causes an external interrupt.
#if 0
    if ((rvbl_marchid_read() == PRORIETARY_SINGLECORE_RHX) &&
        (rvbl_mimpid_read() == ARCV_RHX_1_0)) {
        const rvbl_imsic_hart_delivery_t kDMSI = 0x0;
        const rvbl_uword_t LEVEL1 = 6, SM = LEVEL1;
        // 32-bit read this address twice to trigger interrupt kIrq
        volatile const rvbl_uint32_t *IRQ_TRIGGERING_DEVICE =
            (volatile const rvbl_uint32_t *)0x44000;
        rvbl_uint32_t read_a, read_b;
        const rvbl_aplic_source_configuration_t source_configuration = {
            .delgated = rvbl_false, .source_mode = aplic_sourcecfg_source_mode_values_level1
        };
        const rvbl_aplic_target_configuration_msi_t target_configuration = {
            .guest = 0, .hart = 0, .id = kEEID
        };

        ASSERT_EQ(rvbl_interrupt_hart_current_initialize_traps(NULL, NULL), rvbl_result_success);
        ASSERT_EQ(
            rvbl_interrupt_hart_current_set_trap_handler(
                rvbl_interrupt_hart_privilege_m,
                RVBL_INTERRUPT(mcause_code_values_Interrupt_MachineExternal),
                machine_irq_handler
            ),
            rvbl_result_success
        );

        rvbl_mstatus_fs_write(1);
        rvbl_mstatush_mdt_clear();
        rvbl_mstatus_mie_set();
        rvbl_mie_meie_set();

        ASSERT_EQ(
            rvbl_imsic_hart_set_delivery(
                rvbl_imsic_mode_machine,
                rvbl_imsic_delivery_enabled | rvbl_imsic_delivery_plic_or_aplic |
                    0x20000000 /* Proprietary Synopsys (0x3 - Enable both MMSI
                                  and DMSI) */
            ),
            rvbl_result_success
        );
        ASSERT_EQ(
            rvbl_imsic_hart_set_interrupt_enable(
                rvbl_imsic_mode_machine, kEEID, rvbl_true
            ),
            rvbl_result_success
        );

        // APLIC configuration necessary for IMSIC to deliver interrupt kIrq
        rvbl_aplic_domaincfg_dm_write(&rvbl_aplic_instance_m_domain, aplic_domaincfg_dm_values_msi);
        rvbl_aplic_domaincfg_ie_set(&rvbl_aplic_instance_m_domain);
        ASSERT_EQ(
            rvbl_aplic_set_source_configuration(
                &rvbl_aplic_instance_m_domain, kIrq, &source_configuration
            ),
            rvbl_result_success
        );
        ASSERT_EQ(
            rvbl_aplic_set_interrupt_enable(&rvbl_aplic_instance_m_domain, kIrq, rvbl_true),
            rvbl_result_success
        );
        ASSERT_EQ(
            rvbl_aplic_set_target_configuration_msi(
                &rvbl_aplic_instance_m_domain, kIrq, &target_configuration
            ),
            rvbl_result_success
        );

        ASSERT_EQ(count, 0);

        // Trigger external interrupt
        read_a = *IRQ_TRIGGERING_DEVICE;
        read_b = *IRQ_TRIGGERING_DEVICE;

        ASSERT_EQ(read_a, 0xDEADBEEF)
        ASSERT_EQ(read_b, 0)

        while (count == 0) {
            __asm__ volatile("wfi\n\t");
        }

        ASSERT_EQ(count, 1);
        PASS();
    }
    else
#endif
    {
        SKIP();
    }

    return 0;
}
